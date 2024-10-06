#ifndef LINEAR_SYSTEM_HPP
#define LINEAR_SYSTEM_HPP

#include <memory>

struct LinearSystemOfEquations {
  std::shared_ptr<double []> gg;
  std::shared_ptr<double []> rh;
  std::shared_ptr<double []> cc;
  std::shared_ptr<double []> kk;
  std::shared_ptr<double []> ww;

  std::shared_ptr<int []> row;
  std::shared_ptr<int []> col;

  std::shared_ptr<bool []> isZero;
};

#endif //LINEAR_SYSTEM_HPP
