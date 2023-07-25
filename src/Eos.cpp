#include "Eos.hpp"
#include "EosFlags.hpp"
#include "IOHelper.hpp"
#include "Constants.hpp"

using namespace RusselNS;

Eos::~Eos() {
  PrintMessage("Del Eos base class", 3);
  russel_->memory_->DeleteVariableWithTag(tag_);
}

Eos::Eos(std::string eosId, std::shared_ptr<class Russel> russel) {
  russel_       = russel;
  id_           = eosId;
  tag_          = "i_" + id_ + "_";
  kappa_        = 0.0;
  temperature_  = 0.0;
  pressure_     = 0.0;
  molarMass_    = 0.0;
  rhoMassBulk_  = 0.0;
  rhoMolarBulk_ = 0.0;
  matrixLength_ = 0.0;

  russel_->memory_->SetVariable(tag_ + "rhoMassBulk", std::make_shared<double>(rhoMassBulk_));
  russel_->memory_->SetVariable(tag_ + "rhoMolarBulk", std::make_shared<double>(rhoMolarBulk_));
  russel_->memory_->SetVariable(tag_ + "compressibility", std::make_shared<double>(kappa_));
  russel_->memory_->SetVariable(tag_ + "length of matrix chains", std::make_shared<double>(matrixLength_));
}

void Eos::Parse(std::deque<std::string> deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-temp") {
      temperature_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
    if (deqCoeffs[ii] == "-press") {
      pressure_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
    if (deqCoeffs[ii] == "-mass") {
      molarMass_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
    if (deqCoeffs[ii] == "-density") {
      rhoMassBulk_ = StringToNumber<double>(deqCoeffs[++ii]);
      rhoMolarBulk_ = rhoMassBulk_ / molarMass_ * gr_cm3_to_kg_m3;
    }
    if (deqCoeffs[ii] == "-mx_length") {
      matrixLength_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }

  ParseDerived1(deqCoeffs);
}

void Eos::Report() {
  PrintVariable("temperature", temperature_, "K", 1);
  PrintVariable("pressure", pressure_, "atm", 1);
  PrintMessage("Bulk polymer properties:",1);
  PrintVariable("Monomer molar mass", molarMass_, "g/mol", 2);
  PrintVariable("Bulk mass density", rhoMassBulk_, "kg/m3", 2);
  PrintVariable("Bulk molar density", rhoMolarBulk_, "mol/m3", 2);
  PrintVariable("Length of matrix chains", matrixLength_, "carbon bonds", 2);

  ReportDerived1();
}
