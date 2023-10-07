#ifndef CONTOUR_SYMMETRIC_HPP
#define CONTOUR_SYMMETRIC_HPP

#include "Contour.hpp"

namespace RusselNS {

class ContourSymmetric : public Contour {
  public:
    ContourSymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel);
    virtual ~ContourSymmetric();

    virtual int GetNumberOfSteps() override;

  private:
    virtual void ParseDerived1(const std::deque<std::string> &deqCoeffs) override;
    virtual void ReportDerived1() override;
    virtual void Discretize() override;
};

} // RusselNS

#endif // CONTOUR_SYMMETRIC_HPP
