#ifndef FEM_HPP
#define FEM_HPP

namespace RusselNS {

class Fem {
 private:
  int numGaussPoints{11};
  int numparams{5};

 public:
  double *gp;

  Fem();
  ~Fem();

  double *gausspoints();
  double tetshp(int pp, double *xl);
};

} // RusselNS

#endif // FEM_HPP
