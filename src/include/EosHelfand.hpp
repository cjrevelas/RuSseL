#ifndef EOS_HELFAND_H
#define EOS_HELFAND_H

#include "Eos.hpp"

namespace RusselNS {

class EosHelfand : public Eos {
 public:
   EosHelfand(const std::string eosId, std::shared_ptr<class Russel> &russel);
   ~EosHelfand() override;

   double EnergyDensity(double) override;
   double EnergyDensityDerivative(double) override;
   double RhoBulk() override;
   double Compressibility(double) override;

 private:
   void ParseDerived1(std::deque<std::string>) override;
   void ReportDerived1() override;

   double rho0_;
   double kappaHlf_;
   double rho0Inv_;
};

} // RusselNS


#endif // EOS_HELFAND_H
