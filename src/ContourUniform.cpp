#include "Contour.hpp"
#include "ContourUniform.hpp"

namespace RusselNS {

ContourUniform::ContourUniform(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

  logName_ = "o.contour_uniform";
  logContour_.open(logName_, std::ios::out);
}

ContourUniform::~ContourUniform() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete ContourUniform", 1);
#endif
}

int ContourUniform::GetNumberOfSteps() { return ns_; }

void ContourUniform::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  Discretize();
}

void ContourUniform::ReportDerived1() {
  PrintMessage("Reporting details for uniform chain contour discretization", 2);
  PrintVariable("Average contour step size: ", dsAve_, " ", 3);
  PrintVariable("Number of contour steps: ", ns_, " ", 3);

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
  for (int ii=0; ii<ns_; ++ii) {
    ds_[ii] = dsAve_;
  }

  for (int ii=1; ii<ns_; ++ii) {
    xs_[ii] = xs_[ii-1] + dsAve_;
  }
}

} // RusselNS
