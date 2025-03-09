#include "Chain.hpp"

namespace RusselNS {

Chain::Chain(const std::string &chainId, std::shared_ptr<class Russel> &russel) {
  russel_ = russel;
  id_     = chainId;

#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Chain]: ", russel_.use_count(), "", 1);
#endif
}

Chain::~Chain() {
  russel_.reset();

#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete Chain Instance",1);
#endif
}

void Chain::Parse(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-contourEdw") {
      std::string contourId = deqCoeffs[++ii];
      contourEdw_ = russel_->memory_->GetContour(contourId);
    }
    if (deqCoeffs[ii] == "-contourConv") {
      std::string contourId = deqCoeffs[++ii];
      contourConv_ = russel_->memory_->GetContour(contourId);
    }
  }

  int numNodes = russel_->memory_->mesh_->GetNumberOfNodes();

  int numStepsEdw  = contourEdw_->GetNumberOfSteps();
  int numStepsConv = contourConv_->GetNumberOfSteps();

  qqEdw_.Resize(numNodes, 2);
  qqEdwFinal_.Resize(numNodes, numStepsEdw);
  qqConv_.Resize(numNodes, numStepsConv);

  qqEdw_.Initialize();
  qqEdwFinal_.Initialize();
  qqConv_.Initialize();
}

void Chain::Report() {
  PrintVariable("Reporting details for chain with id: ", id_, "", 2);
  PrintVariable("Edwards discretization with contour: ", contourEdw_->GetId(), " ", 3);
  PrintVariable("Convolution discretization with contour: ", contourConv_->GetId(), " ", 3);

  logPropagatorEdw_.open("o.qq_"+id_+"_edw", std::ios::out);
  logPropagatorEdwFinal_.open("o.qq_"+id_+"_edw_final", std::ios::out);
  logPropagatorConv_.open("o.qq_"+id_+"_conv", std::ios::out);

  qqEdw_.Export(logPropagatorEdw_);
  qqEdwFinal_.Export(logPropagatorEdwFinal_);
  qqConv_.Export(logPropagatorConv_);

  logPropagatorEdw_.close();
  logPropagatorEdwFinal_.close();
  logPropagatorConv_.close();
}

} // RusselNS
