#include "Russel.hpp"

namespace RusselNS {

Russel::Russel() {
  PrintMessage("Create new Russel instance", 0);

  memory_ = std::make_unique<class Memory>();
}

Russel::~Russel() {
  PrintMessage("Delete Russel instance", 0);
}

} // RusselNS
