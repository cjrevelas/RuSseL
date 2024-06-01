#ifndef MESH_HPP
#define MESH_HPP

#include <memory>
#include <fstream>
#include <unordered_map>
#include <map>
#include <utility>

#include "StringOperations.hpp"
#include "Fem.hpp"
#include "Matrix.hpp"

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
  Mesh(const std::string meshFileName);
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

  std::shared_ptr<double []> gg_;
  std::shared_ptr<double []> rh_;
  std::shared_ptr<double []> cc_;
  std::shared_ptr<double []> ww_;

  std::shared_ptr<int []> row_;
  std::shared_ptr<int []> col_;

  std::shared_ptr<bool []> isZero_;
};

} // RusselNS

#endif // MESH_HPP
