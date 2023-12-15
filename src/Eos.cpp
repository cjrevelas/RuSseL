#include "Eos.hpp"
#include "IOHelper.hpp"
#include "Constants.hpp"

using namespace RusselNS;

Eos::~Eos() {
  PrintMessage("Del Eos base class", 1);
  russel_->memory_->DeleteVariableWithTag(tag_);
  russel_.reset();
}

Eos::Eos(const std::string &eosId, std::shared_ptr<class Russel> &russel) {
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
  russel_->memory_->SetVariable(tag_ + "lengthOfMatrixChains", std::make_shared<double>(matrixLength_));

// TODO: IF REPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Eos]: ", russel.use_count(), "", 1);
// TODO: ENDIF REPORT_MEMORY_STATUS
}

void Eos::Parse(const std::deque<std::string> &deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-temp") {
      temperature_ = StringToNumber<double>(deqCoeffs[++ii]); // [K]
    }
    if (deqCoeffs[ii] == "-press") {
      pressure_ = StringToNumber<double>(deqCoeffs[++ii]); // [atm]
    }
    if (deqCoeffs[ii] == "-mass") {
      molarMass_ = StringToNumber<double>(deqCoeffs[++ii]); // [g/mol]
    }
    if (deqCoeffs[ii] == "-density") {
      rhoMassBulk_ = StringToNumber<double>(deqCoeffs[++ii]); // [g/cm3]
      rhoMolarBulk_ = rhoMassBulk_ / molarMass_ * m3_to_cm3;  // [mol/m3]
    }
    if (deqCoeffs[ii] == "-mx_length") {
      matrixLength_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }

  ParseDerived1(deqCoeffs);
}

void Eos::Report() {
  PrintVariable("Temperature", temperature_, "K", 2);
  PrintVariable("Pressure", pressure_, "atm", 2);
  PrintMessage("Bulk polymer properties:",2);
  PrintVariable("Monomer molar mass", molarMass_, "g/mol", 3);
  PrintVariable("Bulk mass density", rhoMassBulk_, "g/cm3", 3);
  PrintVariable("Bulk molar density", rhoMolarBulk_, "mol/m3", 3);
  PrintVariable("Length of matrix chains", matrixLength_, "carbon bonds", 3);

  ReportDerived1();
}
