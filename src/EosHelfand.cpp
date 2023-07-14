#include "EosHelfand.hpp"
#include "Constants.hpp"

namespace RusselNS {

EosHelfand::~EosHelfand() { PrintMessage("Del EosHelfand", 3); }

EosHelfand::EosHelfand(const std::string eosId, Russel *russel)
  : Eos(eosId, russel) {
  kappaHlf_ = 0.0;

  PrintMessage("Add EosHelfand", 3);
}

void EosHelfand::ParseDerived1(std::deque<std::string> deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-coeffs") {
      rhoMassBulk_  = StringToNumber<double>(deqCoeffs[++ii]);
      kappaHlf_     = StringToNumber<double>(deqCoeffs[++ii]);
      // rho0Inv_      = 1.0/rho0_; // FIXME
    }
  }
}

void EosHelfand::ReportDerived1() {
  PrintMessage("Helfand coefficients", 1);
  PrintVariable("Molar bulk density", rhoMolarBulk_, "kg/m3", 2);
  PrintVariable("Compressibility", kappaHlf_, "Pa^-1", 2);
}

double EosHelfand::EnergyDensity(double phi) {
  return (0.5 / kappaHlf_ * (phi - 1) * (phi - 1));
}

double EosHelfand::EnergyDensityDerivative(double phi) {
  return ((phi - 1.0)/(kappaHlf_ * rhoMolarBulk_ * n_avog));
}

double EosHelfand::RhoBulk() {
  return rhoMolarBulk_;
}

double EosHelfand::Compressibility(double lengthBulk) {
  return kappaHlf_;
}

} // RusselNS
