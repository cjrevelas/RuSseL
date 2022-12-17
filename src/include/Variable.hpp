#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "EvalArg.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

class Variable {
  private:
    int int_;
    double dbl_;
    std::string str_;
    std::shared_ptr<const int> pcint_;
    std::shared_ptr<const double> pcdbl_;
    std::shared_ptr<const std::string> pcstr_;
    std::string type_;
    bool isExpression_;
    std::string stringExpressionPrefix_;
    std::list<std::unique_ptr<class EvalArg>> expressionPostfix_;

    void ClearExpression();

  public:
    Variable(std::string itype, std::string stringValue, std::string stringExpressionPrefix);
    // TODO: given the following overloadings, consider making Variable a template class
    Variable(int xx);
    Variable(double xx);
    Variable(std::string xx);
    Variable(std::shared_ptr<const int> xx);
    Variable(std::shared_ptr<const double> xx);
    Variable(std::shared_ptr<const std::string> xx);

    ~Variable();

    void Reset(std::string itype, std::string stringValue, std::string stringExpressionPrefix);

    // Getters
    inline std::string type() { return type_ ; }
    inline std::string getStringExpression() { return stringExpressionPrefix_; }
    inline int GetInteger() { return int_; }
    inline double GetDouble() { return dbl_; }
    inline std::string GetString() { return str_; }
    inline std::shared_ptr<const int> GetPointerToInteger() { return pcint_; }
    inline std::shared_ptr<const double> GetPointerToDouble() { return pcdbl_; }
    inline std::shared_ptr<const std::string> GetPointerToString() { return pcstr_; }
    int GetCastToInteger();
    double GetCastToDouble();
    std::string GetCastToString(int);
    inline int *GetReferenceToInteger() { return &int_; }
    inline double *GetReferenceToDouble() { return &dbl_; }
    inline std::string *GetReferenceToString() { return &str_; }

    // Setters
    inline void SetValue(int xx) {
      int_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "int") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"int\" variable.");
      }
#endif
    }

    inline void SetValue(double xx) {
      dbl_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "dbl") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"dbl\" variable.");
      }
#endif
    }

    inline void SetValue(std::string xx) {
      str_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "str") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"str\" variable.");
      }
#endif
    }

    inline void SetValue(std::shared_ptr<const int> xx) {
      pcint_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "pcint") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"pcint\" variable.");
      }
#endif
    }

    inline void SetValue(std::shared_ptr<const double> xx) {
      pcdbl_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "pcdbl") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"pcdbl\" variable.");
      }
#endif
    }

    inline void SetValue(std::shared_ptr<const std::string> xx) {
      pcstr_ = xx;
#ifdef SAFE_VARIABLE
      if (type_ != "pcstr") {
        ExitProgram("Attempted to set type " + type_ + " to a type \"pcstr\" variable.");
      }
#endif
    }
};

}  // RusseNS

#endif // VARIABLE_HPP
