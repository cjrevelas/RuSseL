#ifndef EOS_SANCHEZ_LACOMBE_HPP
#define EOS_SANCHEZ_LACOMBE_HPP

#include "Eos.hpp"

namespace RusselNS {

class EosSanchezLacombe : public Eos {
  public:
    EosSanchezLacombe(const std::string &eosId, std::shared_ptr<class Russel> &russel);
    ~EosSanchezLacombe() override;

    double EnergyDensity(double) override;
    double EnergyDensityDerivative(double) override;
    double RhoBulk(double) override;
    double Compressibility(double) override;

  private:
    void ParseDerived1(const std::deque<std::string> &) override;
    void ReportDerived1() override;

    double derPref1_;
    double derPref2_;
    double pressStar_;    // [Pa]
    double tempStar_;     // [K]
    double tempStarInv_;  // [K^-1]
    double tempTilde_;
    double pressTilde_;
    double rhoStar_;      // [kg/m3]
    double rhoStarInv_;   // [m^3/kg]
    double rhoTildeBulk_; // [kg/m3]
    double rhoTildeMax_;
    double rslN_;
};

} // RusselNS


#endif // EOS_SANCHEZ_LACOMBE_HPP
