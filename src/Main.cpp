#include <iostream>
#include <memory.h>
#include <fstream>
#include <mpi.h>
#include <dmumps_c.h>

#include "Parser.hpp"
#include "Russel.hpp"
#include "IOHelper.hpp"
#include "SolverMumps.hpp"

using namespace RusselNS;

int main(int argc, char **argv) {
  [[maybe_unused]] MUMPS_INT ierr;
  MUMPS_INT myId;
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

  russel->memory_->fem_ = std::make_shared<class Fem>();

#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Main]: ", russel.use_count(), "", 0);
#endif

  ParseInput(inputFileName, russel);

#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Main]: ", russel.use_count(), "", 1);
#endif

  russel->memory_->InitializeArrays();

  russel->memory_->ReportArrays();

  russel->memory_->fem_->Assembly(3., russel->memory_->wwField_, russel);

  // Start of iterative scheme
  for (int ii=0; ii<russel->memory_->mesh_->GetNumberOfNodes(); ++ii) {
    russel->memory_->GetChain("mx1")->qqEdw_(ii, 0) = 1.0;
    russel->memory_->GetChain("mx1")->qqEdwFinal_(ii, 0) = 1.0;
  }

  russel->memory_->GetChain("mx1")->Report();

  SolverMumps(myId);
  // End of iterative scheme

#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of mesh shared pointers [Main]: ", russel->memory_->mesh_.use_count(), "", 1);
#endif

  russel->memory_->mesh_.reset();
#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of mesh shared pointers [Main]: ", russel->memory_->mesh_.use_count(), "", 1);
#endif

  russel->memory_->DeleteEosMap();
  russel->memory_->DeleteContourMap();
  russel->memory_->DeleteChainMap();
#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Main]: ", russel.use_count(), "", 0);
#endif

  russel.reset();
#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Main]: ", russel.use_count(), "", 0);
#endif

  ierr = MPI_Finalize();

  return 0;
}
