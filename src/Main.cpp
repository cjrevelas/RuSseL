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

//#ifdef REPORT_MEMORY_STATUS
  std::cout << "Number of russel shared pointers [Main]: " << russel.use_count() << '\n';
//#endif

  ParseInput(inputFileName, russel);

  russel->memory_->InitializeArrays();
  russel->memory_->ReportArrays();

  // Start of iterative scheme
  SolverMumps(myId);
  // End of iterative scheme

  std::cout << "Number of mesh shared pointers: " << russel->memory_->mesh_.use_count() << '\n';
  russel->memory_->mesh_.reset();
  std::cout << "Number of mesh shared pointers: " << russel->memory_->mesh_.use_count() << '\n';

  std::cout << "Number of russel shared pointers: " << russel.use_count() << '\n';
  // FIXME: THE FOLLOWING DELETIONS MUST TAKE PLACE ONLY IF THE EOS EXISTS (FOR LOOP IN EOS MAP)
  russel->memory_->DeleteEos("EosId");
  russel->memory_->DeleteEos("EosId2");
  // FIXME: THE FOLLOWING DELETIONS MUST TAKE PLACE ONLY IF THE CONTOUR EXISTS (FOR LOOP IN CONTOUR MAP)
  russel->memory_->DeleteContour("ContourId1");
  russel->memory_->DeleteContour("ContourId2");
  russel->memory_->DeleteContour("ContourId3");
  russel->memory_->DeleteContour("ContourId4");
  russel.reset();
  std::cout << "Number of russel shared pointers: " << russel.use_count() << '\n';

  ierr = MPI_Finalize();

  return 0;
}
