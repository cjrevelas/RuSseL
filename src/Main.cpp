#include <iostream>
#include <mpi.h>
#include <memory.h>

#include "dmumps_c.h"
#include "Mesh.hpp"
#include "Parser.hpp"
#include "Russel.hpp"
#include "IOHelper.hpp"

int main(int argc, char **argv) {
  int gid = 4852;

  RusselNS::Mesh *mesh = new RusselNS::Mesh("in.mesh"); // TODO: use a smart pointer here

  mesh->elementsContainingNode(gid);
 // std::cout << mesh->computeMeshVolume() << '\n';

  delete mesh;
  mesh = NULL;

  std::string inputFileName = "in.input";
  for (int ii = 0; ii<argc; ++ii) {
    std::string argument(argv[ii]);
    if (argument == "-in") {
      inputFileName = argv[++ii];
    }
  }

  if (!RusselNS::FileExists(inputFileName)) {
    RusselNS::ExitProgram("Input file " + inputFileName + " does not exist!");
  }

  // Spawn an instance of the application itself
  std::shared_ptr<RusselNS::Russel> russel{std::make_shared<RusselNS::Russel>()};

  std::cout << "Address of Russel in Main: " << russel << '\n';
  std::cout << "Address of shared pointer in Main: " << &russel << '\n';

  ParseInput(inputFileName, russel);

  DMUMPS_STRUC_C id;
  MUMPS_INT nn = 2;
  MUMPS_INT nnz = 2;
  MUMPS_INT irn[] = {1,2};
  MUMPS_INT jcn[] = {1,2};

  double aa[2];
  double rhs[2];

  MUMPS_INT myId, ierr;

  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);

  aa[0] = 1.0;
  aa[1] = 2.0;

  rhs[0] = 1.0;
  rhs[1] = 4.0;

  constexpr int JOB_INIT = -1;
  constexpr int JOB_END  = -2;
  constexpr int USE_COMM_WORLD = -987654;

  // initialize a mumps instance
  id.comm_fortran=USE_COMM_WORLD;
  id.par=1;
  id.sym=0;
  id.job=JOB_INIT;
  dmumps_c(&id);

  // define the problem on the host
  if (myId == 0) {
    id.n = nn;
    id.nnz = nnz;
    id.irn = irn;
    id.jcn = jcn;
    id.rhs = rhs;
    id.a = aa;
  }

  int error = 0;

  //  call the mumps package (analyze, factorization, and solve)
  id.job = 6;
  dmumps_c(&id);

#define ICNTL(I) icntl[(I)-1]
  id.ICNTL(1) = -1;
  id.ICNTL(2) = -1;
  id.ICNTL(3) = -1;
  id.ICNTL(4) = 0;

  if (id.infog[0]<0) {
    std::cout << "something went really bad dood\n";
    error = 1;
  }

  // terminate mumps instance
  id.job = JOB_END;
  dmumps_c(&id);
  if (myId == 0) {
    if (!error) {
      std::cout << "solution is: " << rhs[0] << ' ' << rhs[1] << '\n';
    } else {
      std::cout << "an error has occured!\n";
    }
  }


  ierr = MPI_Finalize();
  return 0;
}
