#ifndef RUSSEL_HPP
#define RUSSEL_HPP

#include <iostream>
#include <memory>

#include "Memory.hpp"
#include "Mesh.hpp"

namespace RusselNS {

  class Russel {
    public:
     Russel();

     ~Russel();

     std::unique_ptr<class Memory> memory_;
  };

}  // RusselNS

#endif // RUSSEL_HPP
