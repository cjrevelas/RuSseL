#ifndef CONTOUR_ASYMMETRIC_HPP
#define CONTOUR_ASYMMETRIC_HPP

#include "Contour.hpp"

namespace RusselNS {

class ContourAsymmetric : public Contour {
  public:
    ContourAsymmetric(const std::string &contourId, std::shared_ptr<class Russel> &russel);
    virtual ~ContourAsymmetric() override;

    virtual int GetNumberOfSteps() override;

  private:
    virtual void ParseDerived1(const std::deque<std::string> &deqCoeffs) override;
    virtual void ReportDerived1() override;
    virtual void Discretize() override;
};

} // RusselNS

#endif // CONTOUR_ASYMMETRIC_HPP
