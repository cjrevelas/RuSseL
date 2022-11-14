#ifndef RUSSEL_HPP
#define RUSSEL_HPP

#include <iostream>

namespace RusselNS {
  class Russel {
    public:
     Russel() {
       std::cout << "A new instance of Russel has been created\n";
     }
     ~Russel(){}
  };
}  // RusselNS

#endif // RUSSEL_HPP
