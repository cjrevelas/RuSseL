#ifndef MESH_HPP
#define MESH_HPP

#include <memory>

#include "StringOperations.hpp"
#include "Fem.hpp"

namespace RusselNS {

class Mesh{
 private:
  std::string meshFileName_;
  int numElements_;
  int numNodes_;
  int nen_;
  int ndm_;
  std::unique_ptr<int []> ix;
  std::unique_ptr<double []> xc;

 public:
  Mesh(const std::string meshFileName);
  ~Mesh();

  void import();
  void elementsContainingNode(const int &gid);
  double computeElementVolume(const int &elemId);
  double computeMeshVolume();
  friend std::vector<std::string> tokenize(const std::string &input_string);
};

} // RusselNS

#endif // MESH_HPP
