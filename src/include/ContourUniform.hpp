#ifndef CONTOUR_UNIFORM_HPP
#define CONTOUR_UNIFORM_HPP

#include "Contour.hpp"

namespace RusselNS {

class ContourUniform : public Contour {
  public:
    ContourUniform(const std::string &contourId, std::shared_ptr<class Russel> &russel);
    virtual ~ContourUniform();

    virtual int GetNumberOfSteps() override;

  private:
    virtual void ParseDerived1(const std::deque<std::string> &deqCoeffs) override;
    virtual void ReportDerived1() override;
    virtual void Discretize() override;
};

} // RusselNS

#endif // CONTOUR_UNIFORM_HPP
