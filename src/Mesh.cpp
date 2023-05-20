#include <fstream>
#include <iostream>
#include "StringOperations.hpp"
#include "Fem.hpp"
#include "Mesh.hpp"

namespace RusselNS {

Mesh::Mesh(const std::string meshFileName)
: meshFileName_(meshFileName) {
  Import();
}

Mesh::~Mesh(){
  std::cout << "Deleting FEM mesh\n";
}

void Mesh::Import() {
  std::cout << "reading mesh from file: " << meshFileName_ << '\n';

  std::ifstream meshFile;

  meshFile.open(meshFileName_);

  std::string current_line;
  std::vector<std::string> tokens;

  while(meshFile.good()) {
    getline(meshFile, current_line);

    if (current_line.find("sdim") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      ndm_   = atoi(tokens[0].c_str());

      std::cout << "Number of dimensions = " << ndm_ << '\n';
    }

    if (current_line.find("number of mesh points") != std::string::npos) {
      tokens = RusselNS::GetVectorTokens(current_line);
      numNodes_ = atoi(tokens[0].c_str());

      std::cout << "Number of mesh points: " << numNodes_ << '\n';
      xc.resize(numNodes_,ndm_);
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
        std::cout << "Number of vertex elements: " << numElements_ << '\n';
      } else if (nen_ == 2) {
        std::cout << "Number of edge elements: " << numElements_ << '\n';
      } else if (nen_ == 3) {
        std::cout << "Number of face elements: " << numElements_ << '\n';
      } else if (nen_ == 4) {
        ix.resize(numElements_,nen_);

        getline(meshFile, current_line);

        for (int ii=0; ii<numElements_; ++ii) {
          getline(meshFile, current_line);
          tokens = RusselNS::GetVectorTokens(current_line);

          for (int jj=0; jj<nen_; ++jj) {
            ix(ii,jj) = atoi(tokens[jj].c_str());
          }
        }

        std::cout << "Number of domain elements: " << numElements_ << '\n';
      }
    }
  }
}

void Mesh::ElementsContainingNode(const int &gid){
  for (int ii=0; ii<numElements_; ++ii) {
    for (int jj=0; jj<nen_; ++jj) {
      if (ix(ii,jj) == gid) {
        std::cout << "Node " << gid << " is in element " << ii << " with volume " << ComputeElementVolume(ii) << '\n';
      }
    }
  }
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

double Mesh::ComputeMeshVolume(){
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

    std::cout << volel << '\n';

    vol += volel;
  }

  return vol;
}

} // RusselNS
