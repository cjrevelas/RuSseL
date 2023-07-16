#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

#include "Variable.hpp"
#include "Eos.hpp"
#include "Mesh.hpp"

namespace RusselNS {

class Memory {
  public:
    Memory();
    virtual ~Memory();

    // TODO: void ResizeNodalArrays(ints dofs); // e.g., useful for periodic boundary conditions

    void InitializeArrays();

    // Setters (obj)
    void SetVariable(const std::string &id, const std::string &type, const std::string &value, std::string expression);
    void SetVariable(const std::string &id, std::shared_ptr<const int> pcint);
    void SetVariable(const std::string &id, std::shared_ptr<const double> pcdouble);
    void SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr);
    void SetEos(const std::string &id, std::shared_ptr<class Eos> eos);

    // Getters (obj)
    std::shared_ptr<class Variable> GetVariable(const std::string &id);
    std::shared_ptr<class Eos> GetEos(const std::string &id);

    // Checkers (obj)
    bool IsVariable(const std::string &id);
    bool IsEos(const std::string &id);

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

  private:
    std::unordered_map<std::string, std::shared_ptr<class Variable>> variableMap_;
    std::map<std::string, std::shared_ptr<class Eos>> eosMap_;
};

} // RusselNS

#endif // MEMORY_HPP
