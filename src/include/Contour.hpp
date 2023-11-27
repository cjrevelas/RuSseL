#ifndef CONTOUR_HPP
#define CONTOUR_HPP

#include <memory>
#include <fstream>

#include "Russel.hpp"

namespace RusselNS {

class Contour {
 public:
   Contour(const std::string &contourId, std::shared_ptr<class Russel> &russel);
   virtual ~Contour();

   void Parse(const std::deque<std::string> &coeffs);
   void Report();

   virtual int GetNumberOfSteps() = 0;

 protected:
   virtual void ParseDerived1(const std::deque<std::string> &coeffs) = 0;
   virtual void ReportDerived1() = 0;
   virtual void Discretize() = 0;

   std::fstream logContour_;
   std::string logName_;
   std::string id_;
   std::shared_ptr<class Russel> russel_;
   std::shared_ptr<double []> xs_;
   std::shared_ptr<double []> ds_;
   std::shared_ptr<double []> coeff_;
   int ns_;
   double dsAve_;
   double length_;
};

} // RusselNS

#endif // CONTOUR_HPP
