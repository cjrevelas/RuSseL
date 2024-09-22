#include <cmath>

#include "Contour.hpp"
#include "ContourHybrid.hpp"
#include "StringOperations.hpp"

namespace RusselNS {

ContourHybrid::ContourHybrid(const std::string &contourId, std::shared_ptr<class Russel> &russel)
  : Contour(contourId, russel) {

  logName_ = "o.contour_hybrid";
  logContour_.open(logName_, std::ios::out);

  nsPartOne_ = 0;
  xsCrit_    = 0.0;
}

ContourHybrid::~ContourHybrid() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete ContouHybrid", 1);
#endif
}

int ContourHybrid::GetNumberOfSteps() {
  nsPartOne_ = 2 * static_cast<int>(std::round(0.5 * xsCrit_ / dsAve_));

  if (length_ > xsCrit_) {
    dsMax_ = (xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(nsPartOne_+1)) / (static_cast<double>(nsPartOne_) * 2.0) ))) -
             (xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(nsPartOne_  )) / (static_cast<double>(nsPartOne_) * 2.0) )));

    nsPartTwo_ = 2 * static_cast<int>(std::round(0.5 * (length_ - xsCrit_) / dsMax_));

    ns_ = nsPartOne_ + nsPartTwo_;
  } else {
    ns_ = std::ceil( 1.0 + 2.0 * xsCrit_ / (dsAve_ * M_PI) * acos(1.0 - length_/xsCrit_) );

    dsMax_ = (xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ns_+1)) / (static_cast<double>(nsPartOne_) * 2.0) ))) -
             (xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ns_  )) / (static_cast<double>(nsPartOne_) * 2.0) )));
  }

  return ns_ + 1 + 1; // CJR: we add two to get the same exact discretization with the Fortran code
}

void ContourHybrid::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-crit") {
      xsCrit_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }

  Discretize();
}

void ContourHybrid::ReportDerived1() {
  PrintMessage("Reporting details for hybrid chain contour discretization", 2);
  PrintVariable("Number of steps before critical contour point: ", nsPartOne_, " ", 3);
  PrintVariable("Number of steps after  critical contour point: ", nsPartTwo_, " ", 3);
  PrintVariable("Total number of steps (+2):                    ", ns_,        " ", 3);
  PrintVariable("Maximum step size:                             ", dsMax_,     " ", 3);
  PrintVariable("Average contour step size:                     ", dsAve_,     " ", 3);

  if (!logContour_) {
    std::cerr << "ERROR: " << logName_ << " file could not be opened for writing\n";
  } else {
    logContour_ << "step  ds  xs\n";

    for (int ii=0; ii<ns_; ++ii) {
      logContour_ << ii << "  ";
      logContour_ << std::setprecision(10) << ds_[ii] << "  ";
      logContour_ << std::setprecision(10) << xs_[ii] << "  ";
      logContour_ << '\n';
    }
  }

  logContour_.close();
}

void ContourHybrid::Discretize() {
  ds_[0] = 0.0;

  ns_ = GetNumberOfSteps();

  if (xsCrit_ < length_) {
    for (int ii=1; ii<nsPartOne_+1; ++ii) {
      xs_[ii] = xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ii)) / (static_cast<double>(nsPartOne_) * 2.0) ));
      ds_[ii] = xs_[ii] - xs_[ii-1];
    }


    for (int ii=nsPartOne_+1; ii<ns_; ++ii) {
      xs_[ii] = xs_[ii-1] + ds_[nsPartOne_];
      ds_[ii] = xs_[ii]   - xs_[ii-1];
    }
  } else {
    for (int ii=1; ii<ns_; ++ii) {
      xs_[ii] = xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ii)) / (static_cast<double>(nsPartOne_) * 2.0) ));
      ds_[ii] = xs_[ii] - xs_[ii-1];
    }
  }
}

} // RusselNS
