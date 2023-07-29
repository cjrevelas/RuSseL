#include <iostream>
#include <memory.h>
#include <fstream>
#include <mpi.h>
#include <dmumps_c.h>

#include "Mesh.hpp"
#include "Parser.hpp"
#include "Russel.hpp"
#include "IOHelper.hpp"
#include "Defaults.hpp"
#include "ContourFlags.hpp"
#include "SolverMumps.hpp"
#include "MumpsFlags.hpp"

using namespace RusselNS;

int main(int argc, char **argv) {
  MUMPS_INT ierr, myId;
  ierr = MPI_Init(&argc, &argv);
  ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myId);

  std::string inputFileName = "in.input";
  for (int ii = 0; ii<argc; ++ii) {
    std::string argument(argv[ii]);
    if (argument == "-in") {
      inputFileName = argv[++ii];
    }
  }

  if (!FileExists(inputFileName)) {
    ExitProgram("Input file " + inputFileName + " does not exist!");
  }

  // Spawn an instance of the application itself
  std::shared_ptr<class Russel> russel{std::make_shared<Russel>()};

  std::cout << "Address of Russel instance in Main: " << russel << '\n';
  std::cout << "Address of Russel shared pointer in Main: " << &russel << '\n';

  ParseInput(inputFileName, russel);

  std::cout << '\n';

  russel->memory_->InitializeArrays();

  std::ofstream arrays{"o.arrays.txt"};

  if (!arrays) {
    std::cerr << "ERROR: o.arrays.txt file could not be opened for writing\n";
  } else {
    arrays << "ww      ww_new      ww_mixed      phi_grafted      phi_matrix      phi_total\n";

    for (int ii=0; ii<russel->memory_->mesh_->GetNumberOfNodes(); ++ii) {
      arrays << russel->memory_->wwField_[ii]      << ' ';
      arrays << russel->memory_->wwFieldNew_[ii]   << ' ';
      arrays << russel->memory_->wwFieldMixed_[ii] << ' ';
      arrays << russel->memory_->phiGrafted_[ii]   << ' ';
      arrays << russel->memory_->phiMatrix_[ii]    << ' ';
      arrays << russel->memory_->phiTotal_[ii]     << ' ';
      arrays << '\n';
    }
  }

  // Start of iterative scheme
  SolverMumps(myId);
  // End of iterative scheme


  ierr = MPI_Finalize();

  std::cout << "MPI ierr: " << ierr << '\n';

  return 0;
}
