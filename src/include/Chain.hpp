#ifndef CHAIN_HPP
#define CHAIN_HPP

#include <string>
#include <memory>
#include <deque>

#include "Contour.hpp"
#include "Matrix.hpp"

namespace RusselNS {

class Chain {
  public:
    Chain(const std::string &chainId, std::shared_ptr<class Russel> &russel);
    ~Chain(); // CJR:: destructor will become virtual

    void Parse(const std::deque<std::string> &coeffs);
    void Report();

  protected:
    std::string id_;
    Matrix<double> qq_;
    std::shared_ptr<class Russel> russel_;
    std::shared_ptr<class Contour> contourEdw_;
    std::shared_ptr<class Contour> contourConv_;

    void ComputePropagator();

    void SetInitialConditions(); // CJR: this will become virtual to distinguish matrix from grafted chains

    void SetBoundaryConditions();
};

} // RusselNS

#endif // CHAIN_HPP
