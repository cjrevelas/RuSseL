#include <fstream>
#include <iostream>
#include "StringOperations.hpp"
#include "Fem.hpp"
#include "Mesh.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

Mesh::Mesh(const std::string &meshFileName) : meshFileName_(meshFileName) {
  logMesh_.open("o.mesh", std::ios::out);
  logNodeHash_.open("o.node_hash", std::ios::out);
  logNodePairs_.open("o.node_pairs", std::ios::out);
  Import();
}

Mesh::~Mesh() {
  logMesh_.close();
  logNodeHash_.close();

#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete Mesh instance",1);
#endif
}

void Mesh::Import() {
  logMesh_ << "FEM mesh was imported from file: " << meshFileName_ << "\n\n";

  std::ifstream meshFile;

  meshFile.open(meshFileName_);

  std::string current_line;
  std::vector<std::string> tokens;

  while(meshFile.good()) {
    getline(meshFile, current_line);

    if (current_line.find("sdim") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      numDimensions_   = atoi(tokens[0].c_str());

      logMesh_ << "Number of dimensions = " << numDimensions_ << '\n';
    }

    if (current_line.find("number of mesh points") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      numNodes_ = atoi(tokens[0].c_str());

      logMesh_ << "Number of mesh points: " << numNodes_ << '\n';
      nodeCoord_.Resize(numNodes_,numDimensions_);
    }

    if (current_line.find("Mesh point coordinates") != std::string::npos) {
      for (int ii=0; ii<numNodes_; ++ii) {
        getline(meshFile, current_line);
        tokens = RusselNS::GetVectorTokens(current_line);

        for (int jj=0; jj<numDimensions_; ++jj) {
          nodeCoord_(ii,jj) = atof(tokens[jj].c_str());
        }
      }
    }

    if (current_line.find("number of nodes per element") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      numNodesLocalTypeDomain_ = atoi(tokens[0].c_str());
      getline(meshFile, current_line);
      tokens = RusselNS::GetVectorTokens(current_line);
      numElements_ = atoi(tokens[0].c_str());

      if (numNodesLocalTypeDomain_ == 1) {
        logMesh_ << "Number of vertex elements: " << numElements_ << '\n';
      } else if (numNodesLocalTypeDomain_ == 2) {
        logMesh_ << "Number of edge elements: " << numElements_ << '\n';
      } else if (numNodesLocalTypeDomain_ == 3) {
        logMesh_ << "Number of face elements: " << numElements_ << '\n';
      } else if (numNodesLocalTypeDomain_ == 4) {
        globalNodeIdTypeDomain_.Resize(numElements_,numNodesLocalTypeDomain_);

        getline(meshFile, current_line);

        for (int ii=0; ii<numElements_; ++ii) {
          getline(meshFile, current_line);
          tokens = RusselNS::GetVectorTokens(current_line);

          for (int jj=0; jj<numNodesLocalTypeDomain_; ++jj) {
            globalNodeIdTypeDomain_(ii,jj) = atoi(tokens[jj].c_str());
          }
        }

        logMesh_ << "Number of domain elements: " << numElements_ << '\n';
      }
    }
  }
}

void Mesh::ElementsContainingNode(const int &gid){
  logMesh_ << '\n';
  for (int ii=0; ii<numElements_; ++ii) {
    for (int jj=0; jj<numNodesLocalTypeDomain_; ++jj) {
      if (globalNodeIdTypeDomain_(ii,jj) == gid) {
        logMesh_ << "Node " << gid << " is in element " << ii << " with volume " << ComputeElementVolume(ii) << '\n';
      }
    }
  }
  logMesh_ << '\n';
}

double Mesh::ComputeElementVolume(const int &elemId) {
  std::shared_ptr<double []> xl = std::shared_ptr<double []>(new double[numDimensions_ * numNodesLocalTypeDomain_]);

  Fem fem;

  for (int ii=0; ii<numNodesLocalTypeDomain_; ++ii) {
    int kk = globalNodeIdTypeDomain_(elemId,ii);
    for (int jj=0; jj<numDimensions_; ++jj) {
      xl[numNodesLocalTypeDomain_ * jj + ii] = nodeCoord_(kk,jj);
    }
  }

  double volel = 0.0;
  double xsj;

  for (int kk=0; kk<11; ++kk) {
    xsj = fem.Tetshp(kk, xl);
    xsj *= fem.gaussPoint_[11*4+kk];

    volel += xsj;
  }

  return volel;
}

void Mesh::ComputeMeshVolume() {
  std::shared_ptr<double []> xl = std::shared_ptr<double []>(new double[numDimensions_ * numNodesLocalTypeDomain_]);

  Fem fem;

  double vol = 0.0;

  for (int nn=0; nn<numElements_; ++nn) {
    for (int ii=0; ii<numNodesLocalTypeDomain_; ++ii) {
      int kk = globalNodeIdTypeDomain_(nn,ii);
      for (int jj=0; jj<numDimensions_; ++jj) {
        xl[numNodesLocalTypeDomain_ * jj + ii] = nodeCoord_(kk,jj);
      }
    }

    double volel = 0.0;
    double xsj;

    for (int kk=0; kk<11; ++kk) {
      xsj = fem.Tetshp(kk, xl);
      xsj *= fem.gaussPoint_[11 * 4 + kk];

      volel += xsj;
    }

    vol += volel;
  }

  logMesh_ << "Meshed domain volume: " << vol;
}

void Mesh::ComputeBulkNodePairs() {
  int nodePair = 0;

  int globalNodeId1 = 0;
  int globalNodeId2 = 0;

  // int elemconValue;

  numBulkNodePairs_ = numNodesLocalTypeDomain_ * numNodesLocalTypeDomain_ * numElements_;

  nodePairId_ = std::shared_ptr<int []>(new int[numBulkNodePairs_]);

  lse_ = std::make_shared<struct LinearSystemOfEquations>();

  lse_->row = std::shared_ptr<int []>(new int[numBulkNodePairs_]);
  lse_->col = std::shared_ptr<int []>(new int[numBulkNodePairs_]);

  lse_->gg = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_->rh = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_->ww = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_->kk = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_->cc = std::shared_ptr<double []>(new double[numBulkNodePairs_]);

  lse_->isZero = std::shared_ptr<bool []>(new bool[numBulkNodePairs_]);

  // Initialize lse arrays
  for (int pair=0; pair<numBulkNodePairs_; ++pair) {
    lse_->row[pair] = 0;
    lse_->col[pair] = 0;

    lse_->gg[pair] = 0.0;
    lse_->rh[pair] = 0.0;
    lse_->ww[pair] = 0.0;
    lse_->kk[pair] = 0.0;
    lse_->cc[pair] = 0.0;

    lse_->isZero[pair] = true;
  }

  // RuSseL3D -> MeshBulkNodePairs
  for (int element=0; element<numElements_; ++element) {
    for (int localNodeIndex1=0; localNodeIndex1<numNodesLocalTypeDomain_; ++localNodeIndex1) {
      for (int localNodeIndex2=0; localNodeIndex2<numNodesLocalTypeDomain_; ++localNodeIndex2) {
        ++nodePair;

        globalNodeId1 = globalNodeIdTypeDomain_(element, localNodeIndex1);
        globalNodeId2 = globalNodeIdTypeDomain_(element, localNodeIndex2);

        lse_->row[nodePair] = globalNodeId1;
        lse_->col[nodePair] = globalNodeId2;

        if (elemcon_.find(std::make_pair(globalNodeId1,globalNodeId2)) == elemcon_.end()) {
          elemcon_[std::make_pair(globalNodeId1,globalNodeId2)] = nodePair;
          nodePairId_[nodePair] = nodePair;
        } else {
          nodePairId_[nodePair] = elemcon_[std::make_pair(globalNodeId1, globalNodeId2)];
        }
      }
    }
  }

  // Calculate isZero array entries
  for (int pairId=0; pairId<GetNumberOfBulkNodePairs(); ++pairId) {
    lse_->isZero[pairId] = ( nodePairId_[pairId] != pairId );
  }

  // Export element connectivity hash
  for (auto const &item : elemcon_) {
    logNodeHash_ << "Key: (" << item.first.first << ", " << item.first.second << ") -> value: " << item.second << '\n';
  }

  // Export node pairs data
  for (int pairId=0; pairId<numBulkNodePairs_; ++pairId) {
    logNodePairs_ << pairId+1 << "  " << lse_->row[pairId]+1 << "  " << lse_->col[pairId]+1 << "  " << nodePairId_[pairId]+1 << "  " << lse_->isZero[pairId] << '\n';
  }
}

} // RusselNS
