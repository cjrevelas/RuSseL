#ifndef MESH_HPP
#define MESH_HPP

#include <memory>

#include "StringOperations.hpp"
#include "Fem.hpp"
#include "Matrix.hpp"

namespace RusselNS {

class Mesh{
 private:
  std::string meshFileName_;
  int numElements_;
  int numNodes_;
  int nen_;
  int ndm_;
  Matrix<int> ix;
  Matrix<double> xc;

 public:
  Mesh(const std::string meshFileName);
  ~Mesh();

  void Import();
  void ElementsContainingNode(const int &gid);
  double ComputeElementVolume(const int &elemId);
  double ComputeMeshVolume();
  int GetNumberOfNodes() const { return numNodes_; };
  friend std::vector<std::string> Tokenize(const std::string &input_string);

};

} // RusselNS

#endif // MESH_HPP
