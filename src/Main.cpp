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

// TODO: IF REPORT_MEMORY_STATUS
  std::cout << "Number of russel shared pointers [Main]: " << russel.use_count() << '\n';
// TODO: ENDIF REPORT_MEMORY_STATUS

  ParseInput(inputFileName, russel);

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

  std::cout << "Number of mesh shared pointers: " << russel->memory_->mesh_.use_count() << '\n';
  russel->memory_->mesh_.reset();
  std::cout << "Number of mesh shared pointers: " << russel->memory_->mesh_.use_count() << '\n';

  std::cout << "Number of russel shared pointers: " << russel.use_count() << '\n';
  // FIXME: THE FOLLOWING DELETIONS MUST TAKE PLACE ONLY IF THE EOS EXISTS (E.G., USE A PARSER FLAG IN INPUT FILE)
  russel->memory_->DeleteEos("EosId");
  russel->memory_->DeleteEos("EosId2");
  russel.reset();
  std::cout << "Number of russel shared pointers: " << russel.use_count() << '\n';

  ierr = MPI_Finalize();

  return 0;
}
