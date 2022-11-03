#ifndef RUSSEL_H
#define RUSSEL_H

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


#endif
