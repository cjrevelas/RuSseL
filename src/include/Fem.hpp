#ifndef FEM_HPP
#define FEM_HPP

#include <memory>

namespace RusselNS {

class Fem {
 private:
  int numGaussPoints_{11};
  int numParams_{5};
  double shp_[4*4]; // TODO: considering making this an std::array<double>

 public:
  std::shared_ptr<double []> gaussPoint_;

  Fem();
  ~Fem();

  std::shared_ptr<double []> Gausspoints();
  double Tetshp(int pp, std::shared_ptr<double []> xl);

  void Assembly(const double rg2OfMonomer, std::shared_ptr<double []> ww, std::shared_ptr<class Russel> &russel);
  void AssignNonZeroEntries(std::shared_ptr<class Russel> &russel);
  void Edwards(std::shared_ptr<class Russel> &russel);
};

} // RusselNS

#endif // FEM_HPP
