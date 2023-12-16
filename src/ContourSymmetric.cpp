#include <cmath>

#include "Contour.hpp"
#include "ContourSymmetric.hpp"

namespace RusselNS {

ContourSymmetric::ContourSymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

  logName_ = "o.contour_symmetric";
  logContour_.open(logName_, std::ios::out);
}

ContourSymmetric::~ContourSymmetric() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete ContourSymmetric", 1);
#endif
}

int ContourSymmetric::GetNumberOfSteps() { return ns_; }

void ContourSymmetric::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  Discretize();
}

void ContourSymmetric::ReportDerived1() {
  PrintMessage("Reporting details for symmetric chain contour discretization", 2);
  PrintVariable("Average contour step size: ", dsAve_, " ", 3);
  PrintVariable("Number of contour steps: ", ns_, " ", 3);

  if (!logContour_) {
    std::cerr << "ERROR: " << logName_ << "file could not be opened for writing\n";
  } else {
    logContour_ << "step   ds   xs\n";

    for (int ii=0; ii<ns_; ++ii) {
      logContour_ << ii << "   ";
      logContour_ << std::setprecision(3) << ds_[ii] << "   ";
      logContour_ << std::setprecision(3) << xs_[ii] << "   ";
      logContour_ << '\n';
    }
  }

  logContour_.close();
}

void ContourSymmetric::Discretize() {
  ds_[0] = 0.0;

  for (int ii=1; ii<ns_; ++ii) {
    xs_[ii] = length_ * 0.5 * (1.0 - cos(M_PI * (static_cast<double>(ii) - 1.0) / (static_cast<double>(ns_)) ));
    ds_[ii] = xs_[ii] - xs_[ii-1];
  }
}

} // RusselNS
