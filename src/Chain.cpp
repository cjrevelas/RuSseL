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

void Chain::Parse(const std::deque<std::string> &Coeffs) {
  std::cout << "Parsing arguments for Chain class\n";
}

void Chain::Report() {
  std::cout << "Reporting for chain with id: " << id_;
}

Chain::~Chain() {
  russel_.reset();
  PrintMessage("Delete Chain Instance",0);
}

} // RusselNS
