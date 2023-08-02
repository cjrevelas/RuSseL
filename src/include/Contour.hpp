#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <memory>
#include <fstream>

#include "Russel.hpp" // add russel instance in constructor of Contour
#include "ContourFlags.hpp"

namespace RusselNS {

class Contour {
 private:
   std::fstream logContour_;
   std::shared_ptr<class Russel> russel_;
   std::shared_ptr<double []> xs_;
   std::shared_ptr<double []> ds_;
   std::shared_ptr<double []> coeff_;
   int ns_;
   double xsCrit_;
   double dsAve_;
   ContourDiscretization discr_;

 public:
   Contour(ContourDiscretization discr, std::shared_ptr<class Russel> &russel);

   ~Contour();
};

} // RusselNS

#endif // CONTOUR_HPP
