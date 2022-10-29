#ifndef MESH_HPP
#define MESH_HPP

#include "StringHelper.hpp"
#include "Fem.hpp"

class Mesh{
 private:
  std::string meshFileName_;
  int numElements_;
  int numNodes_;
  int nen_;
  int ndm_;
  int *ix;
  double *xc;

 public:
  Mesh(const std::string meshFileName);
  ~Mesh();

  void import();
  void elementsContainingNode(const int &gid);
  double computeElementVolume(const int &elemId);
  double computeMeshVolume();
  friend std::vector<std::string> tokenize(const std::string &input_string);
};

#endif