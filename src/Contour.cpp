#include <fstream>

#include "ContourFlags.hpp"
#include "Contour.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

Contour::Contour(ContourDiscretization discr, std::shared_ptr<class Russel> &russel)
    : discr_(discr) {
  logContour_.open("o.contour", std::ios::out);
  PrintMessage("Create new Contour instance",0);

  russel_ = russel;

// TODO: IF MEMORY_REPORT_STATUS
  std::cout << "Number of russel shared pointers [Contour]: " << russel_.use_count() << '\n';
// TODO: ENDIF MEMORY_REPORT_STATUS 

  if (discr_ == ContourDiscretization::uniform) {
    PrintMessage("Uniform chain contour discretization",0);
  } else if (discr_ == ContourDiscretization::symmetric) {
    PrintMessage("Symmetric chain contour discretization",0);
  } else if (discr_ == ContourDiscretization::asymmetric) {
    PrintMessage("Asymmetric chain contour discretization",0);
  } else if (discr_ == ContourDiscretization::hybrid) {
    PrintMessage("Hybrid chain contour discretization",0);
  }
}

Contour::~Contour() {
  logContour_.close();
  russel_.reset();
  PrintMessage("Delete Contour Instance",0);
}

} // RusselNS
