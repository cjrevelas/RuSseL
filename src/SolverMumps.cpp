#include <iostream>
#include <mpi.h>
#include <dmumps_c.h>

#include "SolverMumps.hpp"

void SolverMumps(MUMPS_INT myId) {
  DMUMPS_STRUC_C id;
  MUMPS_INT nn = 2;
  MUMPS_INT nnz = 2;
  MUMPS_INT irn[] = {1,2};
  MUMPS_INT jcn[] = {1,2};

  double aa[2];
  double rhs[2];

  aa[0] = 1.0;
  aa[1] = 2.0;

  rhs[0] = 1.0;
  rhs[1] = 4.0;

  constexpr int JOB_INIT = -1;
  constexpr int JOB_END  = -2;
  constexpr int USE_COMM_WORLD = -987654;

  // Initialize a mumps instance
  id.comm_fortran=USE_COMM_WORLD;
  id.par=1;
  id.sym=0;
  id.job=JOB_INIT;
  dmumps_c(&id);

  // Define the problem on the host
  if (myId == 0) {
    id.n = nn;
    id.nnz = nnz;
    id.irn = irn;
    id.jcn = jcn;
    id.rhs = rhs;
    id.a = aa;
  }

  int error = 0;

  // Solve
  id.job = 6;
  dmumps_c(&id);

#define ICNTL(I) icntl[(I)-1]
  id.ICNTL(1) = -1;
  id.ICNTL(2) = -1;
  id.ICNTL(3) = -1;
  id.ICNTL(4) = 0;

  if (id.infog[0]<0) {
    std::cout << "ERROR DURING MUMPS SOLUTION PROCESS: STAGE 1\n";
    error = 1;
  }

  // terminate mumps instance
  id.job = JOB_END;
  dmumps_c(&id);
  if (myId == 0) {
    if (!error) {
      std::cout << "Solution is: " << rhs[0] << ' ' << rhs[1] << '\n';
    } else {
      std::cout << "ERROR DURING MUMPS SOLUTION PROCESS: STAGE 2\n";
    }
  }
}
