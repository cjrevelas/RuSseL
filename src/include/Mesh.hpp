#ifndef MESH_HPP
#define MESH_HPP

#include <memory>
#include <fstream>
#include <map>
#include <utility>
#include <vector>

#include "Matrix.hpp"
#include "LinearSystem.hpp"

namespace RusselNS {

class Mesh {
 private:
  std::string meshFileName_;
  std::fstream logMesh_;
  std::fstream logNodeHash_;
  std::fstream logNodePairs_;
  int numElements_;
  int numNodes_;
  int nen_;
  int ndm_;
  int numTotalNodePairs_;
  int numBulkNodePairs_;
  int numPeriodicNodePairs_;
  Matrix<int> ix;
  Matrix<double> xc;

 public:
  Mesh(const std::string &meshFileName);
  ~Mesh();

  void Import();
  void ElementsContainingNode(const int &gid);
  double ComputeElementVolume(const int &elemId);
  void ComputeMeshVolume();
  void ComputeBulkNodePairs();
  int GetNumberOfNodes() const { return numNodes_; };
  friend std::vector<std::string> Tokenize(const std::string &input_string);

  std::map<std::pair<int,int>, int> elemcon_;
  std::shared_ptr<int []> nodePairId_;

  struct LinearSystemOfEquations lse_;
};

} // RusselNS

#endif // MESH_HPP
