#ifndef FEM_HPP
#define FEM_HPP

#include <memory>

namespace RusselNS {

class Fem {
 private:
  int numGaussPoints{11};
  int numparams{5};

 public:
  std::shared_ptr<double []> gp;

  Fem();
  ~Fem();

  std::shared_ptr<double []> Gausspoints();
  double Tetshp(int pp, std::shared_ptr<double []> xl);

  void Assembly(const double rg2OfMonomer, std::shared_ptr<double []> ww);
};

} // RusselNS

#endif // FEM_HPP
