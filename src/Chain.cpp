#include "Chain.hpp"

namespace RusselNS {

Chain::Chain(const std::string &chainId, std::shared_ptr<class Russel> &russel) {
  russel_ = russel;
  id_     = chainId;

// TODO: IF MEMORY_EXPORT_STATUS
  PrintVariable("Number of russel shared pointers [Chain]: ", russel.use_count(), "", 1);
// TODO: ENDIF MEMORY_EXPORT_STATUS
}

void Chain::Parse(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-contour") {
      std::string contourId = deqCoeffs[++ii];
      contourEdw_ = russel_->memory_->GetContour(contourId);
    }
  }
}

void Chain::Report() {
  PrintVariable("Reporting details for chain with id: ", id_, "", 2);
  PrintVariable("Edwards discretization with contour: ", contourEdw_->GetId() ," ", 3);
}

Chain::~Chain() {
  russel_.reset();
  PrintMessage("Delete Chain Instance",1);
}

} // RusselNS
