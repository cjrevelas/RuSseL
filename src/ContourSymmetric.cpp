#include "Contour.hpp"
#include "ContourSymmetric.hpp"

namespace RusselNS {

ContourSymmetric::~ContourSymmetric() { PrintMessage("Del ContourSymmetric", 3); }

ContourSymmetric::ContourSymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

    PrintMessage("Add ContourSymmetric", 1);
  }

int ContourSymmetric::GetNumberOfSteps() { return ns_; }

void ContourSymmetric::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  PrintMessage("Parsing arguments for symmetric chain contour discretization", 1);
}

void ContourSymmetric::ReportDerived1() {
  PrintMessage("Reporting details for symmetric chain contour discretization", 1);
}

void ContourSymmetric::Discretize() {
  PrintMessage("Performing symmetric chain discretization", 1);
}

} // RusselNS
