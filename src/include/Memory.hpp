#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

#include "Variable.hpp"
#include "Eos.hpp"
#include "Contour.hpp"
#include "Chain.hpp"
#include "Mesh.hpp"

namespace RusselNS {

class Memory {
  public:
    Memory();
    ~Memory();

    // TODO: void ResizeNodalArrays(ints dofs); // e.g., useful for periodic boundary conditions

    void InitializeArrays();
    void ReportArrays();

    // Setters (obj)
    void SetVariable(const std::string &id, const std::string &type, const std::string &value, std::string expression);
    void SetVariable(const std::string &id, std::shared_ptr<const int> pcint);
    void SetVariable(const std::string &id, std::shared_ptr<const double> pcdouble);
    void SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr);
    void SetEos(const std::string &id, std::shared_ptr<class Eos> &eos);
    void SetContour(const std::string &id, std::shared_ptr<class Contour> &contour);
    void SetChain(const std::string &id, std::shared_ptr<class Chain> &chain);

    // Getters (obj)
    std::shared_ptr<class Variable> GetVariable(const std::string &id);
    std::shared_ptr<class Eos> GetEos(const std::string &id);
    std::shared_ptr<class Contour> GetContour(const std::string &id);
    std::shared_ptr<class Chain> GetChain(const std::string &id);

    // Checkers (obj)
    bool IsVariable(const std::string &id);
    bool IsEos(const std::string &id);
    bool IsContour(const std::string &id);
    bool IsChain(const std::string &id);

    std::shared_ptr<Mesh> mesh_;

    std::shared_ptr<double []> wwField_;
    std::shared_ptr<double []> wwFieldNew_;
    std::shared_ptr<double []> wwFieldMixed_;
    std::shared_ptr<double []> phiGrafted_;
    std::shared_ptr<double []> phiMatrix_;
    std::shared_ptr<double []> phiTotal_;

    // Deleters
    void DeleteVariableWithTag(const std::string &tag);
    void DeleteVariable(const std::string &id);
    void DeleteEos(const std::string &id);
    void DeleteContour(const std::string &id);
    void DeleteChain(const std::string &id);

    void DeleteEosMap();
    void DeleteContourMap();
    void DeleteChainMap();

  private:
    std::unordered_map<std::string, std::shared_ptr<class Variable>> variableMap_;
    std::map<std::string, std::shared_ptr<class Eos>> eosMap_;
    std::map<std::string, std::shared_ptr<class Contour>> contourMap_;
    std::map<std::string, std::shared_ptr<class Chain>> chainMap_;

    std::fstream logArrays_;
};

} // RusselNS

#endif // MEMORY_HPP
