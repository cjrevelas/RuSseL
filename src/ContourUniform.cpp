#include "Contour.hpp"
#include "ContourUniform.hpp"

namespace RusselNS {

ContourUniform::~ContourUniform() { PrintMessage("Del ContourUniform", 3); }

ContourUniform::ContourUniform(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

  PrintMessage("Add ContourUniform", 1);

  logName_ = "o.contour_uniform";
  logContour_.open(logName_, std::ios::out);
}

int ContourUniform::GetNumberOfSteps() { return ns_; }

void ContourUniform::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  Discretize();
}

void ContourUniform::ReportDerived1() {
  PrintMessage("Reporting details for uniform chain contour discretization", 1);

  if (!logContour_) {
    std::cerr << "ERROR: o.contour file could not be opened for writing\n";
  } else {
    logContour_ << "step   ds   xs\n";

    for (int ii=0; ii<ns_; ++ii) {
      logContour_ << ii << "   ";
      logContour_ << std::setprecision(3) << ds_[ii] << "   ";
      logContour_ << std::setprecision(3) << xs_[ii] << "   ";
      logContour_ << '\n';
    }

    logContour_.close();
  }
}

void ContourUniform::Discretize() {
  PrintMessage("Performing uniform chain discretization", 1);

  for (int ii=0; ii<ns_; ++ii) {
    ds_[ii] = dsAve_;
  }

  for (int ii=1; ii<ns_; ++ii) {
    xs_[ii] = xs_[ii-1] + dsAve_;
  }
}

} // RusselNS
