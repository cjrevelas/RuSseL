#ifndef EOS_HELFAND_H
#define EOS_HELFAND_H

#include "Eos.hpp"

namespace RusselNS {

class EosHelfand : public Eos {
 public:
   EosHelfand(const std::string &eosId, std::shared_ptr<class Russel> &russel);
   virtual ~EosHelfand() override;

   virtual double EnergyDensity(double) override;
   virtual double EnergyDensityDerivative(double) override;
   virtual double RhoBulk(double) override;
   virtual double Compressibility(double) override;

 private:
   virtual void ParseDerived1(const std::deque<std::string> &) override;
   virtual void ReportDerived1() override;

   double rho0_;
   double kappaHlf_;
   double rho0Inv_;
};

} // RusselNS


#endif // EOS_HELFAND_H
