#include <cmath>

#include "EosSanchezLacombe.hpp"
#include "Constants.hpp"

namespace RusselNS {

EosSanchezLacombe::~EosSanchezLacombe() { PrintMessage("Del EosSanchezLacombe", 1); }

EosSanchezLacombe::EosSanchezLacombe(const std::string &eosId, std::shared_ptr<class Russel> &russel)
  : Eos(eosId, russel) {
    derPref1_     = 0.0;
    derPref2_     = 0.0;
    pressStar_    = 0.0;
    tempStar_     = 0.0;
    tempStarInv_  = 0.0;
    rhoStar_      = 0.0;
    rhoStarInv_   = 0.0;
    rhoTildeMax_  = 0.0;
}

void EosSanchezLacombe::ParseDerived1(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-coeffs") {
      tempStar_  = StringToNumber<double>(deqCoeffs[++ii]);
      pressStar_ = StringToNumber<double>(deqCoeffs[++ii]);
      rhoStar_   = StringToNumber<double>(deqCoeffs[++ii]);

      rslN_ = (molarMass_ * pressStar_) / (rhoStar_ * kg_m3_to_gr_m3 * boltz_const_Joule_molK * tempStar_);

      tempTilde_   = temperature_ / tempStar_;
      pressTilde_  = pressure_ / pressStar_;
      tempStarInv_ = 1.0 / tempStar_;
      rhoStarInv_  = 1.0 / rhoStar_;

      derPref1_ = -2.0 * pressStar_ / (rhoStar_ * rhoStar_);
      derPref2_ = -pressStar_ / rhoStar_;
    }

    if (deqCoeffs[ii] == "-rhoTol") {
      rhoTildeMax_ = StringToNumber<double>(deqCoeffs[++ii]);
    }

  }

  rhoTildeBulk_ = RhoBulk(matrixLength_);
}

void EosSanchezLacombe::ReportDerived1() {
  PrintMessage("Sanchez-Lacombe coefficients", 2);
  PrintVariable("Characteristic temperature", tempStar_,    "K"      , 3);
  PrintVariable("Characteristic pressure   ", pressStar_,   "Pa"     , 3);
  PrintVariable("Characteristic density    ", rhoStar_,     "kg/m^3" , 3);
  PrintVariable("Characteristic rsl ratio  ", rslN_,        "-"      , 3);
  PrintVariable("Maximum allowed density   ", rhoTildeMax_, "kg/m3"  , 3);
  PrintVariable("Compressibility           ", Compressibility(matrixLength_), "Pa^-1", 3);
  PrintVariable("Temp tilde:               ", tempTilde_,    "", 3);
  PrintVariable("Press tilde:              ", pressTilde_,   "", 3);
  PrintVariable("rslN:                     ", rslN_,         "",3);
  PrintVariable("rhoTildeBulk:             ", rhoTildeBulk_, "", 3);
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

double EosSanchezLacombe::RhoBulk(double lengthBulk) {
  double rhoTildeZeroInit = 0.8;
  double rhoTildeZeroNew  = 0.0;
  double rhoTildeZero     = rhoTildeZeroInit;
  double rhoTildeMax      = 0.98;

  double rsl = rslN_ * lengthBulk;

  double ff = 0.0;
  double df = 0.0;

  int numIter    = 0;
  int maxNumIter = 1000;

  double tolerance = 1.0e-10;
  double errorNorm = 2.0e1;

  std::cout << "      iteration      " << "rhoTilde     " << "error\n";
  while ((numIter < maxNumIter) && (errorNorm > tolerance)) {
    std::cout << "      " << numIter << "      " << rhoTildeZero << "      " << errorNorm << '\n';
    numIter += 1;

    ff = rhoTildeZero * rhoTildeZero + pressTilde_ + tempTilde_ * ( std::log(1.0-rhoTildeZero) + (1.0-1.0/rsl)*rhoTildeZero );

    df = 2.0 * rhoTildeZero + tempTilde_ * ( (1.0-1.0/rsl) - 1.0/(1.0-rhoTildeZero) );

    rhoTildeZeroNew = rhoTildeZero - ff / df;

    errorNorm = std::abs(rhoTildeZeroNew-rhoTildeZero);

    rhoTildeZero = rhoTildeZeroNew;
  }

  return rhoTildeZero;
}

double EosSanchezLacombe::Compressibility(double lengthBulk) {
  double aux = tempTilde_ * pressStar_ * rhoTildeBulk_ * rhoTildeBulk_ *
               (1.0/(1.0-rhoTildeBulk_) + 1.0/(rhoTildeBulk_ * rslN_ * lengthBulk)
               -2.0/tempTilde_);

  return 1.0/aux;
}

} // RusselNS
