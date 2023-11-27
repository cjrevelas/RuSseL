#include <cmath>

#include "Contour.hpp"
#include "ContourHybrid.hpp"

namespace RusselNS {
  ContourHybrid::~ContourHybrid() { PrintMessage("Del ContouHybrid", 3); }

  ContourHybrid::ContourHybrid(const std::string &contourId, std::shared_ptr<class Russel> &russel)
    : Contour(contourId, russel) {

    PrintMessage("Add ContourHybrid", 1);

    logName_ = "o.contour_hybrid";
    logContour_.open(logName_, std::ios::out);

    nsPartOne_ = 0;
    xsCrit_    = 0.0;
  }

  int ContourHybrid::GetNumberOfSteps() { return ns_; } // FIXME

  void ContourHybrid::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
    for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
      if (deqCoeffs[ii] == "-crit") {
        xsCrit_ = StringToNumber<double>(deqCoeffs[++ii]);
      }
    }

    Discretize();
  }

  void ContourHybrid::ReportDerived1() { // CJR: consider making this a virtual function to avoid code duplication
    PrintMessage("Reporting details for hybrid chain contour discretization", 1);

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
    PrintMessage("Performing hybrid chain discretization", 1);

    ds_[0] = 0.0;

    nsPartOne_ = 2 * static_cast<int>(0.5 * xsCrit_ / dsAve_); // CJR: check that this gives the same result with fortran NINT function

    if (xsCrit_ < length_) {
      for (int ii=1; ii<nsPartOne_+1; ++ii) {
        xs_[ii] = xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ii)) / (static_cast<double>(nsPartOne_) * 2.0) ));
        ds_[ii] = xs_[ii] - xs_[ii-1];
      }


      for (int ii=nsPartOne_+1; ii<ns_; ++ii) {
        xs_[ii] = xs_[ii-1] + ds_[nsPartOne_];
        ds_[ii] = xs_[ii] - xs_[ii-1];
      }
    } else {
      for (int ii=1; ii<ns_; ++ii) {
        xs_[ii] = xsCrit_ * (1.0 - cos(M_PI * (static_cast<double>(ii)) / (static_cast<double>(nsPartOne_) * 2.0) ));
        ds_[ii] = xs_[ii] - xs_[ii-1];
      }
    }
  }
} // RusselNS
