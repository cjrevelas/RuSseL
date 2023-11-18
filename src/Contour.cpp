#include <fstream>

#include "Contour.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

Contour::Contour(const std::string &contourId, std::shared_ptr<class Russel> &russel) {
  PrintMessage("Create new Contour instance",0);

  russel_ = russel;
  id_     = contourId;

  ns_        = 0;
  nsPartOne_ = 0;

  dsAve_  = 0.0;

// TODO: IF MEMORY_REPORT_STATUS
  std::cout << "Number of russel shared pointers [Contour]: " << russel_.use_count() << '\n';
// TODO: ENDIF MEMORY_REPORT_STATUS

}

void Contour::Parse(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-step") {
      dsAve_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }

  length_ = static_cast<int>(russel_->memory_->GetEos("EosId")->GetMatrixLength());

  ns_ = length_ / dsAve_;

  ds_ = std::shared_ptr<double []>(new double[ns_]);
  xs_ = std::shared_ptr<double []>(new double[ns_]);

  for (int ii=0; ii<ns_; ++ii) {
    ds_[ii] = 0.0;
    xs_[ii] = 0.0;
  }

  ParseDerived1(deqCoeffs);
}

void Contour::Report() {
  PrintVariable("Average contour step size: ", dsAve_, " ", 2);
  PrintVariable("Number of contour steps: ", ns_, " ", 2);

  ReportDerived1();
}

Contour::~Contour() {
  russel_.reset();
  PrintMessage("Delete Contour Instance",0);
}

} // RusselNS
