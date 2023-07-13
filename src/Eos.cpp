#include "Eos.hpp"
#include "EosFlags.hpp"
#include "IOHelper.hpp"

using namespace RusselNS;

Eos::~Eos() {
  PrintMessage("Del Eos base class", 3);
  russel_->memory_->DeleteVariableWithTag(tag_);
}

Eos::Eos(std::string eosId, Russel *russel) {
  russel_      = russel;
  id_          = eosId;
  tag_         = "i_" + id_ + "_";
  kappa_       = 0.0;
  temperature_ = 0.0;
  pressure_    = 0.0;
  molarMass_   = 0.0;

  russel_->memory_->SetVariable(tag_ + "rhoBulk", std::make_shared<double>(rhoBulk_));
  russel_->memory_->SetVariable(tag_ + "compressibility", std::make_shared<double>(kappa_));
}

void Eos::Parse(std::deque<std::string> deqCoeffs) {
  for (int ii = 0; ii<static_cast<int>(deqCoeffs.size()); ++ii) {
    if (deqCoeffs[ii] == "-temp") {
      temperature_ = StringToNumber<double>(deqCoeffs[++ii]);
    }

    if (deqCoeffs[ii] == "-press") {
      pressure_ = StringToNumber<double>(deqCoeffs[++ii]);
    }

    if (deqCoeffs[ii] == "-compress") {
      kappa_ = StringToNumber<double>(deqCoeffs[++ii]);
    }

    if (deqCoeffs[ii] == "-density") {
      rhoBulk_ = StringToNumber<double>(deqCoeffs[++ii]);
    }
  }

  ParseDerived1(deqCoeffs);
}

void Eos::Report() {
  PrintVariable("temperature", temperature_, "K", 1);
  PrintVariable("pressure", pressure_, "atm", 1);
  PrintMessage("Bulk polymer properties:",1);
  PrintVariable("Density", rhoBulk_, "kg/m3", 2);
  PrintVariable("Compressibility", kappa_, "Pa^-1", 2);

  ReportDerived1();
}
