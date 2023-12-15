#include <cmath>

#include "Contour.hpp"
#include "ContourAsymmetric.hpp"

namespace RusselNS {

ContourAsymmetric::~ContourAsymmetric() { PrintMessage("Del ContourAsymmetric", 1); }

ContourAsymmetric::ContourAsymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

  logName_ = "o.contour_asymmetric";
  logContour_.open(logName_, std::ios::out);
}

int ContourAsymmetric::GetNumberOfSteps() { return ns_; }

void ContourAsymmetric::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  Discretize();
}

void ContourAsymmetric::ReportDerived1() {
  PrintMessage("Reporting details for asymmetric chain contour discretization", 2);
  PrintVariable("Average contour step size: ", dsAve_, " ", 3);
  PrintVariable("Number of contour steps: ", ns_, " ", 3);

  if (!logContour_) {
    std::cerr << "ERROR: " << logName_ << " file could not be opened for writing\n";
  } else {
    logContour_ << "step  ds  xs\n";

    for (int ii=0; ii<ns_; ++ii) {
      logContour_ << ii << "  ";
      logContour_ << std::setprecision(3) << ds_[ii] << "  ";
      logContour_ << std::setprecision(3) << xs_[ii] << "  ";
      logContour_ << '\n';
    }
  }

  logContour_.close();
}

void ContourAsymmetric::Discretize() {
  ds_[0] = 0.0;

  for (int ii=1; ii<ns_; ++ii) {
    xs_[ii] = length_ * (1.0 - cos(M_PI * (static_cast<double>(ii) - 1.0) / (static_cast<double>(ns_) * 2.0) ));
    ds_[ii] = xs_[ii] - xs_[ii-1];
  }
}

} // RusselNS
