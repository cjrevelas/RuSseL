#include "Russel.hpp"

namespace RusselNS {

Russel::Russel() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Create new Russel instance", 0);
#endif

  memory_ = std::make_unique<class Memory>();
}

Russel::~Russel() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete Russel instance", 0);
#endif
}

} // RusselNS
