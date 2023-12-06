#ifndef CONTOUR_HYBRID_HPP
#define CONTOUR_HYBRID_HPP

#include "Contour.hpp"

namespace RusselNS {

class ContourHybrid : public Contour {
  public:
    ContourHybrid(const std::string &contourId, std::shared_ptr<class Russel> &russel);
    virtual ~ContourHybrid();

    virtual int GetNumberOfSteps() override;

  private:
    virtual void ParseDerived1(const std::deque<std::string> &deqCoeffs) override;
    virtual void ReportDerived1() override;
    virtual void Discretize() override;

    double xsCrit_;
    double dsMax_;

    int nsPartOne_;
    int nsPartTwo_;
};

} // RusselNS

#endif // CONTOUR_HYBRID_HPP
