#ifndef FEM_HPP
#define FEM_HPP

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

#endif
