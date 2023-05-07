#include <iostream>
#include "Fem.hpp"

namespace RusselNS {

Fem::Fem() { gp = Gausspoints(); }

Fem::~Fem(){
  delete[] gp;
  gp  = NULL;
}

double *Fem::Gausspoints(){
  gp = new double[numGaussPoints * numparams];

  for (int ii=0; ii<3; ++ii){
    for (int jj=0; jj<10; jj++){
      gp[numGaussPoints * ii + jj] = 0.0;
    }

    gp[numGaussPoints * ii + ii]     = 1.0;
    gp[numGaussPoints * ii + ii + 4] = 0.5;
    gp[numGaussPoints * ii + ii + 7] = 0.5;
    gp[numGaussPoints * ii + 10]     = 0.25;
  }

  gp[numGaussPoints * 1 + 4] = 0.5;
  gp[numGaussPoints * 2 + 5] = 0.5;
  gp[numGaussPoints * 0 + 9] = 0.5;

  for (int jj=0; jj<4; ++jj){
    gp[4 * numGaussPoints + jj] = 1.0/360.0;
  }

  for (int jj=4; jj<10; ++jj){
    gp[4 * numGaussPoints + jj] = 1.0/90.0;
  }

  gp[4 * numGaussPoints + 10] = 4.0/45.0;
  gp[0 * numGaussPoints + 6]  = 0.5;
  gp[0 * numGaussPoints + 9]  = 0.0;

  // Compute fourth point coordinates
  for (int jj=0; jj<numGaussPoints; ++jj){
    gp[3 * numGaussPoints + jj] = 1.0 - (gp[0 * numGaussPoints + jj] + gp[1 * numGaussPoints + jj] + gp[2 * numGaussPoints + jj]);
  }

  return gp;
}


double Fem::Tetshp(int pp, double *xl) {
  double shp[4*4];

  shp[3*4 + 0] = gp[0 * numGaussPoints + pp];
  shp[3*4 + 1] = gp[1 * numGaussPoints + pp];
  shp[3*4 + 2] = gp[2 * numGaussPoints + pp];
  shp[3*4 + 3] = gp[3 * numGaussPoints + pp];

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

  double xsj = xs[0] * (xs[4]*xs[8] - xs[5]*xs[7]) + xs[1] * (xs[5]*xs[6] - xs[3]*xs[8]) + xs[2] * (xs[3]*xs[7] - xs[4]*xs[6]);

  double xsinv[3*3];

  xsinv[0] = (xs[4]*xs[8] - xs[7]*xs[5]) / xsj;
  xsinv[3] = (xs[7]*xs[2] - xs[1]*xs[8]) / xsj;
  xsinv[6] = (xs[1]*xs[5] - xs[4]*xs[2]) / xsj;

  xsinv[1] = (xs[6]*xs[5] - xs[3]*xs[8]) / xsj;
  xsinv[4] = (xs[0]*xs[8] - xs[6]*xs[2]) / xsj;
  xsinv[7] = (xs[3]*xs[2] - xs[0]*xs[5]) / xsj;

  xsinv[2] = (xs[3]*xs[7] - xs[6]*xs[4]) / xsj;
  xsinv[5] = (xs[6]*xs[1] - xs[0]*xs[7]) / xsj;
  xsinv[8] = (xs[0]*xs[4] - xs[3]*xs[1]) / xsj;

  for (int kk=0; kk<3; ++kk){
    for (int jj=0; jj<2; ++jj){
      for (int ii=0; ii<2; ++ii){
        shp[4*jj + kk] += shpxi[4*ii + kk] * xsinv[3*ii + jj];
      }
    }
  }

  return xsj;
}

} // RusselNS
