#ifndef EOS_HPP
#define EOS_HPP

#include <string>
#include <deque>

#include "Russel.hpp"

namespace RusselNS {

class Eos {
  public:
    Eos(const std::string &eosId, std::shared_ptr<class Russel> &russel);
    virtual ~Eos();

    void Parse(const std::deque<std::string> &coeffs);
    void Report();

    virtual double EnergyDensity(double) = 0;
    virtual double EnergyDensityDerivative(double) = 0;
    virtual double RhoBulk(double) = 0;
    virtual double Compressibility(double) = 0;

    inline double GetTemperature()  const { return temperature_; }
    inline double GetPressure()     const { return pressure_; }
    inline double GetRhoMassBulk()  const { return rhoMassBulk_; }
    inline double GetRhoMolarBulk() const { return rhoMolarBulk_; }
    inline double GetMolarMass()    const { return molarMass_; }
    inline double GetMatrixLength() const { return matrixLength_; }

  protected:
    virtual void ParseDerived1(const std::deque<std::string> &coeffs) = 0;
    virtual void ReportDerived1() = 0;

    std::string id_;
    std::string tag_;
    double temperature_;
    double pressure_;
    double rhoMassBulk_;
    double rhoMolarBulk_;
    double rhoMax_;
    double molarMass_;
    double kappa_;
    double matrixLength_;
    std::shared_ptr<class Russel> russel_;
};

} // RusselNS

#endif // EOS_HPP
