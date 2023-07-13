#include "EosHelfand.hpp"

namespace RusselNS {

  EosHelfand::~EosHelfand() { PrintMessage("Del EosHelfand", 3); }

  EosHelfand::EosHelfand(const std::string eosId, Russel *russel)
    : Eos(eosId, russel) {
    rho0_     = 0.0;
    kappaHlf_ = 0.0;

    PrintMessage("Add EosHelfand", 3);
  }

  void EosHelfand::ParseDerived1(std::deque<std::string> deqCoeffs) {
    for (int ii = 0; ii<deqCoeffs.size(); ++ii) {
      if (deqCoeffs[ii] == "-coeffs") {
        rho0_     = StringToNumber<double>(deqCoeffs[++ii]);
        kappaHlf_ = StringToNumber<double>(deqCoeffs[++ii]);
        rho0Inv_  = 1.0/rho0_;
      }
    }
  }

  void EosHelfand::ReportDerived1() {
    PrintMessage("Helfand coefficients", 1);
    PrintVariable("rho0", rho0_, "kg/m3", 2);
    PrintVariable("kappaT", kappaHlf_, "Pa^-1", 2);
  }

  double EosHelfand::EnergyDensity(double rho, double temp) {
    return (0.5 / kappaHlf_ * (rho * rho0Inv_ - 1) * (rho * rho0Inv_ - 1));
  }

  double EosHelfand::EnergyDensityDerivative(double rho, double temp) {
    return ((rho * rho0Inv_ - 1.0)/(kappaHlf_ * rho0_));
  }

} // RusselNS
