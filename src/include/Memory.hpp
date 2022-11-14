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


    // Setters (obj)
    void SetVariable(const std::string &id, const std::string &type, const std::string &value, std::string expression);
    void SetVariable(const std::string &id, std::unique_ptr<const int> pcint);
    void SetVariable(const std::string &id, std::unique_ptr<const double> pcdouble);
    void SetVariable(const std::string &id, std::unique_ptr<std::string> pcstr);

    // Getters (obj)
    std::unique_ptr<Variable> GetVariable(const std::string &id);

  private:
    std::unordered_map<std::string, std::unique_ptr<Variable>> variableMap_;
};


} // RusselNS


#endif // MEMORY_HPP
