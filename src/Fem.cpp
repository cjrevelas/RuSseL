#include <iostream>
#include <array>
#include <iomanip>

#include "Fem.hpp"
#include "Russel.hpp"

namespace RusselNS {

Fem::Fem() { gaussPoint_ = Gausspoints(); }

Fem::~Fem() {}

std::shared_ptr<double []> Fem::Gausspoints() {
  gaussPoint_ = std::shared_ptr<double []>(new double[numGaussPoints_ * numParams_]);

  for (int ii=0; ii<3; ++ii) {
    for (int jj=0; jj<10; jj++) {
      gaussPoint_[numGaussPoints_ * ii + jj] = 0.0;
    }

    gaussPoint_[numGaussPoints_ * ii + ii]     = 1.0;
    gaussPoint_[numGaussPoints_ * ii + ii + 4] = 0.5;
    gaussPoint_[numGaussPoints_ * ii + ii + 7] = 0.5;
    gaussPoint_[numGaussPoints_ * ii + 10]     = 0.25;
  }

  gaussPoint_[numGaussPoints_ * 1 + 4] = 0.5;
  gaussPoint_[numGaussPoints_ * 2 + 5] = 0.5;
  gaussPoint_[numGaussPoints_ * 0 + 9] = 0.5;

  for (int jj=0; jj<4; ++jj) {
    gaussPoint_[4 * numGaussPoints_ + jj] = 1.0/360.0;
  }

  for (int jj=4; jj<10; ++jj) {
    gaussPoint_[4 * numGaussPoints_ + jj] = 1.0/90.0;
  }

  gaussPoint_[4 * numGaussPoints_ + 10] = 4.0/45.0;
  gaussPoint_[0 * numGaussPoints_ + 6]  = 0.5;
  gaussPoint_[0 * numGaussPoints_ + 9]  = 0.0;

  // Compute fourth point coordinates
  for (int jj=0; jj<numGaussPoints_; ++jj) {
    gaussPoint_[3 * numGaussPoints_ + jj] = 1.0 - (gaussPoint_[0 * numGaussPoints_ + jj] +
    gaussPoint_[1 * numGaussPoints_ + jj] +
    gaussPoint_[2 * numGaussPoints_ + jj] );
  }

  return gaussPoint_;
}


double Fem::Tetshp(int pp, std::shared_ptr<double []> xl) {

  shp_[3*4 + 0] = gaussPoint_[0 * numGaussPoints_ + pp];
  shp_[3*4 + 1] = gaussPoint_[1 * numGaussPoints_ + pp];
  shp_[3*4 + 2] = gaussPoint_[2 * numGaussPoints_ + pp];
  shp_[3*4 + 3] = gaussPoint_[3 * numGaussPoints_ + pp];

  double shpxi[3*4];

  shpxi[0*4 + 0] =  1.0;
  shpxi[0*4 + 1] =  0.0;
  shpxi[0*4 + 2] =  0.0;
  shpxi[0*4 + 3] = -1.0;

  shpxi[1*4 + 0] =  0.0;
  shpxi[1*4 + 1] =  1.0;
  shpxi[1*4 + 2] =  0.0;
  shpxi[1*4 + 3] = -1.0;

  shpxi[2*4 + 0] =  0.0;
  shpxi[2*4 + 1] =  0.0;
  shpxi[2*4 + 2] =  1.0;
  shpxi[2*4 + 3] = -1.0;

  double xs[3*3];

  for (int ii=0; ii<3; ++ii){
    for (int jj=0; jj<3; ++jj){
      xs[3*ii + jj] = 0.0;
      for (int kk=0; kk<4; ++kk){
        xs[3*ii + jj] -= shpxi[4*ii + kk] * xl[4*jj + kk];
      }
    }
  }

  double jacobDeterm = xs[0] * (xs[4]*xs[8] - xs[5]*xs[7]) + xs[1] * (xs[5]*xs[6] - xs[3]*xs[8]) + xs[2] * (xs[3]*xs[7] - xs[4]*xs[6]);

  double xsinv[3*3];

  xsinv[0] = (xs[4]*xs[8] - xs[7]*xs[5]) / jacobDeterm;
  xsinv[3] = (xs[7]*xs[2] - xs[1]*xs[8]) / jacobDeterm;
  xsinv[6] = (xs[1]*xs[5] - xs[4]*xs[2]) / jacobDeterm;

  xsinv[1] = (xs[6]*xs[5] - xs[3]*xs[8]) / jacobDeterm;
  xsinv[4] = (xs[0]*xs[8] - xs[6]*xs[2]) / jacobDeterm;
  xsinv[7] = (xs[3]*xs[2] - xs[0]*xs[5]) / jacobDeterm;

  xsinv[2] = (xs[3]*xs[7] - xs[6]*xs[4]) / jacobDeterm;
  xsinv[5] = (xs[6]*xs[1] - xs[0]*xs[7]) / jacobDeterm;
  xsinv[8] = (xs[0]*xs[4] - xs[3]*xs[1]) / jacobDeterm;

  for (int kk=0; kk<3; ++kk){
    for (int jj=0; jj<2; ++jj){
      for (int ii=0; ii<2; ++ii){
        shp_[4*jj + kk] += shpxi[4*ii + kk] * xsinv[3*ii + jj];
      }
    }
  }

  return jacobDeterm;
}

void Fem::Assembly(const double rg2OfMonomer, std::shared_ptr<double []> ww, std::shared_ptr<class Russel> &russel) {
  std::cout << "------------------------------------ START OF MATRIX ASSEMBLY ------------------------------------\n";

  std::array<int,4> globalIndex{{0,0,0,0}};

  Matrix<double> coord;
  coord.Resize(3,4);
  coord.Initialize();

  // Initialize linear system of equations
  for (int nodePair=0; nodePair<russel->memory_->mesh_->GetNumberOfBulkNodePairs(); ++nodePair) {
    russel->memory_->mesh_->lse_->cc[nodePair] = 0.0;
    russel->memory_->mesh_->lse_->kk[nodePair] = 0.0;
    russel->memory_->mesh_->lse_->ww[nodePair] = 0.0;
  }

  std::fstream assemblyFile1;
  std::fstream assemblyFile2;
  std::fstream assemblyFile3;
  assemblyFile1.open("o.assembly1",std::ios::out);
  assemblyFile2.open("o.assembly2",std::ios::out);
  assemblyFile3.open("o.assembly3",std::ios::out);

  // Loop through elements
  int pair = 0;

  for (int element=0; element < russel->memory_->mesh_->GetNumberOfElements(); ++element) {
    // Retrieve global index of each node in the element along with its coordinates
    for (int localNodeIndex=0; localNodeIndex < russel->memory_->mesh_->numNodesLocalTypeDomain_; ++localNodeIndex) {
      globalIndex[localNodeIndex] = russel->memory_->mesh_->globalNodeIdTypeDomain_(element,localNodeIndex);

      for (int axis=0; axis<2; ++axis) {
        coord(axis,localNodeIndex) = russel->memory_->mesh_->nodeCoord_(globalIndex[localNodeIndex],axis);
      }

      assemblyFile1 << "element: " << element << ",    local: " << localNodeIndex << ",    global: " << globalIndex[localNodeIndex] <<
      ",        x:    " << std::setprecision(12) << std::fixed << coord(0,localNodeIndex) <<
      "    y:    "      << std::setprecision(12) << std::fixed << coord(1,localNodeIndex) <<
      " z:    "         << std::setprecision(12) << std::fixed << coord(2,localNodeIndex) << '\n';
    }

    std::shared_ptr<double []> xl = std::shared_ptr<double []>(new double[russel->memory_->mesh_->numDimensions_ * russel->memory_->mesh_->numNodesLocalTypeDomain_]);

    for (int localNodeIndex=0; localNodeIndex<russel->memory_->mesh_->numNodesLocalTypeDomain_; ++localNodeIndex) {
      for (int axis=0; axis<russel->memory_->mesh_->numDimensions_; ++axis) {
        xl[russel->memory_->mesh_->numNodesLocalTypeDomain_ * axis + localNodeIndex] = coord(axis,localNodeIndex);

      }
      assemblyFile2 << "element: " << element << ",    local: " << localNodeIndex << ",    global: " << globalIndex[localNodeIndex] <<
      ",        x:    " << std::setprecision(12) << std::fixed << xl[russel->memory_->mesh_->numNodesLocalTypeDomain_ * 0 + localNodeIndex] <<
      "    y:    "      << std::setprecision(12) << std::fixed << xl[russel->memory_->mesh_->numNodesLocalTypeDomain_ * 1 + localNodeIndex] <<
      " z:    "         << std::setprecision(12) << std::fixed << xl[russel->memory_->mesh_->numNodesLocalTypeDomain_ * 2 + localNodeIndex] << '\n';
    }

    // CJR: Up to this point everything is fine: tests pass and assemblyFile1, assemblyFile2 have identical content


    // Set up for Gauss Quadradure inside the element
    for (int gaussPointIndex=0; gaussPointIndex<numGaussPoints_; ++gaussPointIndex) {

      pair = russel->memory_->mesh_->numNodesLocalTypeDomain_ * russel->memory_->mesh_->numNodesLocalTypeDomain_ * element;

      // Compute Jacobian determinant and values of shape functions
      double jacobDeterm = Tetshp(gaussPointIndex, xl); // Shape functions are members of the Fem class and thus modified internally

      jacobDeterm *= gaussPoint_[numGaussPoints_*4 + gaussPointIndex];  // TODO: check that the matrix of gauss points (5x11) is identical to the Fortran one

      for (int localNodeIndexOne=0; localNodeIndexOne<russel->memory_->mesh_->numNodesLocalTypeDomain_; ++localNodeIndexOne) {
        for (int localNodeIndexTwo=0; localNodeIndexTwo<russel->memory_->mesh_->numNodesLocalTypeDomain_; ++ localNodeIndexTwo) {
           ++pair;

           assemblyFile3 << "gausspoint: " << gaussPointIndex << ",    ";
           assemblyFile3 << "element: "    << element         << ",    ";
           assemblyFile3 << "pair:    "    << pair            << ",    node1: " << globalIndex[localNodeIndexOne] << ", node2: " << globalIndex[localNodeIndexTwo] << '\n';

           // std::cout << "&lse:    " << russel->memory_->mesh_->lse_ << '\n';

           int globalNodeIndex = globalIndex[localNodeIndexTwo];

           russel->memory_->mesh_->lse_->ww[pair] += shp_[3 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexOne] * // 3 * 4 + (0,3)
                                                     shp_[3 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexTwo] *
                                                     jacobDeterm * gaussPoint_[4*numGaussPoints_ + gaussPointIndex] * ww[globalNodeIndex];

           russel->memory_->mesh_->lse_->cc[pair] += shp_[3 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexOne] *
                                                     shp_[3 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexTwo] *
                                                     jacobDeterm * gaussPoint_[4 * numGaussPoints_ + gaussPointIndex];

           russel->memory_->mesh_->lse_->kk[pair] += rg2OfMonomer * ( (shp_[0 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexOne] *
                                                                       shp_[0 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexTwo] ) +
                                                                      (shp_[1 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexOne] *
                                                                       shp_[1 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexTwo] ) +
                                                                      (shp_[2 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexOne] *
                                                                       shp_[2 * russel->memory_->mesh_->numNodesLocalTypeDomain_ + localNodeIndexTwo]) );
        }
      }
    }
  }

  for (int nodePair=0; nodePair<russel->memory_->mesh_->GetNumberOfBulkNodePairs(); ++nodePair) {
    if (russel->memory_->mesh_->lse_->isZero[nodePair]) {
 //     russel->memory_->mesh_->lse_->kk[russel->memory_->mesh_->nodePairId_[nodePair]] += russel->memory_->mesh_->lse_->kk[nodePair];
 //     russel->memory_->mesh_->lse_->kk[nodePair] = 0;

 //     russel->memory_->mesh_->lse_->cc[russel->memory_->mesh_->nodePairId_[nodePair]] += russel->memory_->mesh_->lse_->cc[nodePair];
 //     russel->memory_->mesh_->lse_->cc[nodePair] = 0;

 //     russel->memory_->mesh_->lse_->ww[russel->memory_->mesh_->nodePairId_[nodePair]] += russel->memory_->mesh_->lse_->ww[nodePair];
 //     russel->memory_->mesh_->lse_->ww[nodePair] = 0;
 //  
      if (nodePair == russel->memory_->mesh_->GetNumberOfBulkNodePairs()) {
         std::cout << "number of bulk node pairs = " << nodePair << '\n';
      }
    }
  }

  std::cout << "------------------------------------  END OF MATRIX ASSEMBLY  ------------------------------------\n";
}

} // RusselNS
