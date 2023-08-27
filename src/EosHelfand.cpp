#include "EosHelfand.hpp"
#include "Constants.hpp"

namespace RusselNS {

EosHelfand::~EosHelfand() { PrintMessage("Del EosHelfand", 3); }

EosHelfand::EosHelfand(const std::string eosId, std::shared_ptr<class Russel> &russel)
  : Eos(eosId, russel) {
  kappaHlf_ = 0.0;

  PrintMessage("Add EosHelfand", 1);
}

void EosHelfand::ParseDerived1(std::deque<std::string> deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-coeffs") {
      kappaHlf_     = StringToNumber<double>(deqCoeffs[++ii]);
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

double EosHelfand::RhoBulk(double lengthBulk_) {
  lengthBulk_ = lengthBulk_;
  return rhoMolarBulk_;
}

double EosHelfand::Compressibility(double lengthBulk) {
  lengthBulk = lengthBulk;
  return kappaHlf_;
}

} // RusselNS
