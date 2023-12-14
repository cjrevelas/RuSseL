#include "Chain.hpp"

namespace RusselNS {

Chain::Chain(const std::string &chainId, std::shared_ptr<class Russel> &russel) {
  PrintMessage("Create new Chain instance",0);

  russel_ = russel;
  id_     = chainId;

// TODO: IF MEMORY_EXPORT_STATUS
  std::cout << "Number of russel shared pointers [Chain]: " << russel_.use_count() << '\n';
// TODO: ENDIF MEMORY_EXPORT_STATUS
}

void Chain::Parse(const std::deque<std::string> &deqCoeffs) {
  PrintMessage("Parsing arguments for Chain class",1);
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-contour") {
      std::string contourId = deqCoeffs[++ii];
      contourEdw_ = russel_->memory_->GetContour(contourId);
    }
  }
}

void Chain::Report() {
  PrintVariable("Reporting for chain with id: ", id_, " ", 2);
  PrintVariable("chain is discretized for Edwards with contour: ", contourEdw_->GetId() ," ", 2);
}

Chain::~Chain() {
  russel_.reset();
  PrintMessage("Delete Chain Instance",0);
}

} // RusselNS
