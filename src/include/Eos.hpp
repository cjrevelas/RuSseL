#ifndef EOS_HPP
#define EOS_HPP

#include <string>

#include "Russel.hpp"

namespace RusselNS {

class Eos {
  public:
    Eos(std::string eosId, Russel *russel);
    virtual ~Eos();

    void Parse(std::deque<std::string> coeffs);
    void Report();

    virtual double EnergyDensity(double, double) = 0;
    virtual double EnergyDensityDerivative(double, double) = 0;
    virtual double Compressibility(double, double) = 0;

    inline double GetTemperature() { return temperature_; }
    inline double GetPressure() { return pressure_; }
    inline double GetRhoBulk() { return rhoBulk_; }
    inline double GetMolarMass() { return molarMass_; }

  protected:
    virtual void ParseDerived1(std::deque<std::string> coeffs) = 0;
    virtual void ReportDerived1() = 0;

    std::string id_;
    std::string tag_;
    double temperature_;
    double pressure_;
    double rhoBulk_;
    double molarMass_;
    double kappaT_;
    class Russel *russel_;
};

} // RusselNS

#endif // EOS_HPP
