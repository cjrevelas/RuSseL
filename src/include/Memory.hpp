#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>

#include "Variable.hpp"

namespace RusselNS {

class Memory {
  public:
    Memory();
    virtual ~Memory();

    // TODO: void ResizeNodalArrays(ints dofs);
    // This function will be useful for implementation of periodic boundary conditions

    // Setters (obj)
    void SetVariable(const std::string &id, const std::string &type, const std::string &value, std::string expression);
    void SetVariable(const std::string &id, std::shared_ptr<const int> pcint);       // CHECK: pointer to heap here?
    void SetVariable(const std::string &id, std::shared_ptr<const double> pcdouble); // CHECK: pointer to heap here?
    void SetVariable(const std::string &id, std::shared_ptr<std::string> pcstr);     // CHECK: pointer to heap here?

    // Getters (obj)
    std::shared_ptr<Variable> GetVariable(const std::string &id); // CJR: shared pointer for heap allocated memory

    // Checkers (obj)
    bool IsVariable(const std::string &id);

  private:
    std::unordered_map<std::string, std::shared_ptr<Variable>> variableMap_; // CJR: shared pointer for heap allocated memory
};


} // RusselNS


#endif // MEMORY_HPP
