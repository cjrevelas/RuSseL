#include <fstream>
#include <iostream>
#include "StringOperations.hpp"
#include "Fem.hpp"
#include "Mesh.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

Mesh::Mesh(const std::string &meshFileName)
 : meshFileName_(meshFileName) {
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
      ndm_   = atoi(tokens[0].c_str());

      logMesh_ << "Number of dimensions = " << ndm_ << '\n';
    }

    if (current_line.find("number of mesh points") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      numNodes_ = atoi(tokens[0].c_str());

      logMesh_ << "Number of mesh points: " << numNodes_ << '\n';
      xc.Resize(numNodes_,ndm_);
    }

    if (current_line.find("Mesh point coordinates") != std::string::npos) {
      for (int ii=0; ii<numNodes_; ++ii) {
        getline(meshFile, current_line);
        tokens = RusselNS::GetVectorTokens(current_line);

        for (int jj=0; jj<ndm_; ++jj) {
          xc(ii,jj) = atof(tokens[jj].c_str());
        }
      }
    }

    if (current_line.find("number of nodes per element") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      nen_ = atoi(tokens[0].c_str());
      getline(meshFile, current_line);
      tokens = RusselNS::GetVectorTokens(current_line);
      numElements_ = atoi(tokens[0].c_str());

      if (nen_ == 1) {
        logMesh_ << "Number of vertex elements: " << numElements_ << '\n';
      } else if (nen_ == 2) {
        logMesh_ << "Number of edge elements: " << numElements_ << '\n';
      } else if (nen_ == 3) {
        logMesh_ << "Number of face elements: " << numElements_ << '\n';
      } else if (nen_ == 4) {
        ix.Resize(numElements_,nen_);

        getline(meshFile, current_line);

        for (int ii=0; ii<numElements_; ++ii) {
          getline(meshFile, current_line);
          tokens = RusselNS::GetVectorTokens(current_line);

          for (int jj=0; jj<nen_; ++jj) {
            ix(ii,jj) = atoi(tokens[jj].c_str());
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
    for (int jj=0; jj<nen_; ++jj) {
      if (ix(ii,jj) == gid) {
        logMesh_ << "Node " << gid << " is in element " << ii << " with volume " << ComputeElementVolume(ii) << '\n';
      }
    }
  }
  logMesh_ << '\n';
}

double Mesh::ComputeElementVolume(const int &elemId) {
  std::shared_ptr<double []> xl = std::shared_ptr<double []>(new double[ndm_ * nen_]);

  Fem fem;

  for (int ii=0; ii<nen_; ++ii) {
    int kk = ix(elemId,ii);
    for (int jj=0; jj<ndm_; ++jj) {
      xl[nen_ * jj + ii] = xc(kk,jj);
    }
  }

  double volel = 0.0;
  double xsj;

  for (int kk=0; kk<11; ++kk) {
    xsj = fem.Tetshp(kk, xl);
    xsj *= fem.gp[11*4+kk];

    volel += xsj;
  }

  return volel;
}

void Mesh::ComputeMeshVolume() {
  std::shared_ptr<double []> xl = std::shared_ptr<double []>(new double[ndm_ * nen_]);

  Fem fem;

  double vol = 0.0;

  for (int nn=0; nn<numElements_; ++nn) {
    for (int ii=0; ii<nen_; ++ii) {
      int kk = ix(nn,ii);
      for (int jj=0; jj<ndm_; ++jj) {
        xl[nen_ * jj + ii] = xc(kk,jj);
      }
    }

    double volel = 0.0;
    double xsj;

    for (int kk=0; kk<11; ++kk) {
      xsj = fem.Tetshp(kk, xl);
      xsj *= fem.gp[11 * 4 + kk];

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

  int elemconValue;

  numBulkNodePairs_ = nen_ * nen_ * numElements_;

  nodePairId_ = std::shared_ptr<int []>(new int[numBulkNodePairs_]);

  lse_.row = std::shared_ptr<int []>(new int[numBulkNodePairs_]);
  lse_.col = std::shared_ptr<int []>(new int[numBulkNodePairs_]);

  lse_.gg = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_.rh = std::shared_ptr<double []>(new double[numBulkNodePairs_]);
  lse_.ww = std::shared_ptr<double []>(new double[numBulkNodePairs_]);

  for (int element=0; element<numElements_; ++element) {
    for (int localNodeIndex1=0; localNodeIndex1<nen_; ++localNodeIndex1) {
      for (int localNodeIndex2=0; localNodeIndex2<nen_; ++localNodeIndex2) {
        ++nodePair;

        globalNodeId1 = ix(element, localNodeIndex1);
        globalNodeId2 = ix(element, localNodeIndex2);

	lse_.row[nodePair] = globalNodeId1;
	lse_.col[nodePair] = globalNodeId2;

        if (elemcon_.find(std::make_pair(globalNodeId1,globalNodeId2)) == elemcon_.end()) {
          elemcon_[std::make_pair(globalNodeId1,globalNodeId2)] = nodePair;
          nodePairId_[nodePair] = nodePair;
        } else {
          nodePairId_[nodePair] = elemcon_[std::make_pair(globalNodeId1, globalNodeId2)];
        }
      }
    }
  }

  // Export element connectivity hash
  for (auto const &item : elemcon_) {
    logNodeHash_ << "Key: (" << item.first.first << ", " << item.first.second << ") -> value: " << item.second << '\n';
  }

  // Export node pairs data
  for (int pairId=0; pairId<numBulkNodePairs_; ++pairId) {
    logNodePairs_ << pairId+1 << "  " << lse_.row[pairId]+1 << "  " << lse_.col[pairId]+1 << "  " << nodePairId_[pairId] << '\n';
  }
}

} // RusselNS
