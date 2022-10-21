#include <fstream>
#include <iostream>
#include "string_helper.hpp"
#include "fem.hpp"
#include "mesh.hpp"

Mesh::Mesh(const std::string meshFileName)
: meshFileName_(meshFileName) {
  import();
}

Mesh::~Mesh(){
  delete[] ix;
  delete[] xc;

  ix = NULL;
  xc = NULL;
}

void Mesh::import() {
  std::cout << "reading mesh from file: " << meshFileName_ << '\n';

  std::ifstream meshFile;

  meshFile.open(meshFileName_);

  std::string current_line;
  std::vector<std::string> tokens;

  while(meshFile.good()) {
    getline(meshFile, current_line);

    if (current_line.find("sdim") != std::string::npos) {
      tokens = tokenize(current_line);
      ndm_   = atoi(tokens[0].c_str());

      std::cout << "Number of dimensions = " << ndm_ << '\n';
    }

    if (current_line.find("number of mesh points") != std::string::npos) {
      tokens = tokenize(current_line);
      numNodes_ = atoi(tokens[0].c_str());

      std::cout << "Number of mesh points: " << numNodes_ << '\n';
      xc = new double[ndm_ * numNodes_];
    }

    if (current_line.find("Mesh point coordinates") != std::string::npos) {
      for (int ii=0; ii<numNodes_; ++ii) {
        getline(meshFile, current_line);
        tokens = tokenize(current_line);

        for (int jj=0; jj<ndm_; ++jj) {
          xc[ii * ndm_ + jj] = atof(tokens[jj].c_str());
        }
      }
    }

    if (current_line.find("number of nodes per element") != std::string::npos) {
      tokens = tokenize(current_line);
      nen_ = atoi(tokens[0].c_str());
      getline(meshFile, current_line);
      tokens = tokenize(current_line);
      numElements_ = atoi(tokens[0].c_str());

      if (nen_ == 1) {
        std::cout << "Number of vertex elements: " << numElements_ << '\n';
      } else if (nen_ == 2) {
        std::cout << "Number of edge elements: " << numElements_ << '\n';
      } else if (nen_ == 3) {
        std::cout << "Number of face elements: " << numElements_ << '\n';
      } else if (nen_ == 4) {
        ix = new int[nen_ * numElements_];

        getline(meshFile, current_line);

        for (int ii=0; ii<numElements_; ++ii) {
          getline(meshFile, current_line);
          tokens = tokenize(current_line);

          for (int jj=0; jj<nen_; ++jj) {
            ix[ii * nen_ + jj] = atoi(tokens[jj].c_str());
          }
        }

        std::cout << "Number of domain elements: " << numElements_ << '\n';
      }
    }
  }
}

void Mesh::elementsContainingNode(const int &gid){
  for (int ii=0; ii<numElements_; ++ii) {
    for (int jj=0; jj<nen_; ++jj) {
      if (ix[ii * nen_ + jj] == gid) {
        std::cout << "Node " << gid << " is in element " << ii << " with volume " << computeElementVolume(ii) << '\n';
      }
    }
  }
}

double Mesh::computeElementVolume(const int &elemId) {
  double *xl = new double[ndm_ * nen_];

  Fem fem;

  for (int ii=0; ii<nen_; ++ii) {
    int kk = ix[elemId * nen_ + ii];
    for (int jj=0; jj<ndm_; ++jj) {
      xl[nen_ * jj + ii] = xc[ndm_ * kk + jj];
    }
  }

  double volel = 0.0;
  double xsj;

  for (int kk=0; kk<11; ++kk) {
    xsj = fem.tetshp(kk, xl);
    xsj *= fem.gp[11*4+kk];

    volel += xsj;
  }

  delete[] xl;
  xl = NULL;

  return volel;
}

double Mesh::computeMeshVolume(){
  double *xl  = new double[ndm_ * nen_];

  Fem fem;

  double vol = 0.0;

  for (int nn=0; nn<numElements_; ++nn) {
    for (int ii=0; ii<nen_; ++ii) {
      int kk = ix[nn * nen_ + ii];
      for (int jj=0; jj<ndm_; ++jj) {
        xl[nen_ * jj + ii] = xc[ndm_ * kk + jj];
      }
    }

    double volel = 0.0;
    double xsj;

    for (int kk=0; kk<11; ++kk) {
      xsj = fem.tetshp(kk, xl);
      xsj *= fem.gp[11 * 4 + kk];

      volel += xsj;
    }

    std::cout << volel << '\n';

    vol += volel;
  }

  delete[] xl;
  xl  = NULL;

  return vol;
}
