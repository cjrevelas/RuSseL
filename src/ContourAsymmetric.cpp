#include "Contour.hpp"
#include "ContourAsymmetric.hpp"

namespace RusselNS {

ContourAsymmetric::~ContourAsymmetric() { PrintMessage("Del ContourAsymmetric", 3); }

ContourAsymmetric::ContourAsymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

    PrintMessage("Add ContourAsymmetric", 1);
  }

int ContourAsymmetric::GetNumberOfSteps() { return ns_; }

void ContourAsymmetric::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  PrintMessage("Parsing arguments for asymmetric chain contour discretization", 1);
}

void ContourAsymmetric::ReportDerived1() {
  PrintMessage("Reporting details for asymmetric chain contour discretization", 1);
}

void ContourAsymmetric::Discretize() {
  PrintMessage("Performing asymmetric chain discretization", 1);
}

} // RusselNS
