#include <cmath>

#include "EosSanchezLacombe.hpp"
#include "Constants.hpp"

namespace RusselNS {

EosSanchezLacombe::~EosSanchezLacombe() { PrintMessage("Del EosSanchezLacombe", 3); }

EosSanchezLacombe::EosSanchezLacombe(const std::string &eosId, Russel *russel)
  : Eos(eosId, russel) {
    derPref1_     = 0.0;
    derPref2_     = 0.0;
    pressStar_    = 0.0;
    tempStar_     = 0.0;
    tempStarInv_  = 0.0;
    rhoStar_      = 0.0;
    rhoStarInv_   = 0.0;
    rhoTildeMax_  = 0.0;
    rhoTildeBulk_ = RhoBulk();



    PrintMessage("Add EosSanchezLacombe", 3);
}

void EosSanchezLacombe::ParseDerived1(std::deque<std::string> deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "coeffs") {
      tempStar_  = StringToNumber<double>(deqCoeffs[++ii]);
      pressStar_ = StringToNumber<double>(deqCoeffs[++ii]);
      rhoStar_   = StringToNumber<double>(deqCoeffs[++ii]);
      rslN_      = StringToNumber<double>(deqCoeffs[++ii]);

      tempTilde_   = temperature_ / tempStar_;
      tempStarInv_ = 1.0 / tempStar_;
      rhoStarInv_  = 1.0 / rhoStar_;

      derPref1_ = -2.0 * pressStar_ / (rhoStar_ * rhoStar_);
      derPref2_ = -pressStar_ / rhoStar_;
    }

    if (deqCoeffs[ii] == "-rhoTol") {
      rhoTildeMax_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }
}

void EosSanchezLacombe::ReportDerived1() {
  PrintMessage("Sanchez-Lacombe coefficients", 1);
  PrintVariable("Characteristic temperature", tempStar_,  "K"     , 2);
  PrintVariable("Characteristic pressure   ", pressStar_, "Pa"    , 2);
  PrintVariable("Characteristic density    ", rhoStar_,  "kg/m^3", 2);
  PrintVariable("Maximum allowed density   ", rhoTildeMax_, "kg/m3", 2);
}

double EosSanchezLacombe::EnergyDensity(double phi) {
  double rhoTilde  = rhoTildeBulk_ * phi;

  if (rhoTilde > rhoTildeMax_) { rhoTilde = rhoTildeMax_; }

  return pressStar_ * (tempTilde_ * rhoTilde - rhoTilde * rhoTilde + tempTilde_ * (1.0 - rhoTilde) * std::log(1.0-rhoTilde));
}

double EosSanchezLacombe::EnergyDensityDerivative(double phi) {
  double rhoTilde = rhoTildeBulk_ * phi;

  if (rhoTilde > rhoTildeMax_) { rhoTilde = rhoTildeMax_; }

  return boltz_const_Joule_K * tempStar_ * (-2.0 * rslN_ * rhoTilde - tempTilde_ * rslN_ * std::log(1.0 - rhoTilde));
}

double EosSanchezLacombe::RhoBulk() {
  return -1.0; // FIXME
}

double EosSanchezLacombe::Compressibility(double lengthBulk) {
  double aux = tempTilde_ * pressStar_ * rhoTildeBulk_ * rhoTildeBulk_ *
               (1.0/(1.0-rhoTildeBulk_)) + 1.0/(rhoTildeBulk_ * rslN_ * lengthBulk)
               -2.0/tempTilde_;

  return 1.0/aux; // FIXME
}

} // RusselNS
