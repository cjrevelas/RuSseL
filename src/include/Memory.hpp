#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>

#include "Variable.hpp"
#include "Mesh.hpp"

namespace RusselNS {

class Memory {
  public:
    Memory();
    virtual ~Memory();

    // TODO: void ResizeNodalArrays(ints dofs);
    // This function will be useful for implementation of periodic boundary conditions

    // Setters (obj)
    void SetVariable(const std::string &id, const std::string &type, const std::string &value, std::string expression);
    void SetVariable(const std::string &id, std::shared_ptr<const int> pcint);
    void SetVariable(const std::string &id, std::shared_ptr<const double> pcdouble);
    void SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr);

    // Getters (obj)
    std::shared_ptr<class Variable> GetVariable(const std::string &id);


    // Checkers (obj)
    bool IsVariable(const std::string &id);

  private:
    class Mesh *mesh;
    std::unordered_map<std::string, std::shared_ptr<class Variable>> variableMap_;
};

} // RusselNS

#endif // MEMORY_HPP
