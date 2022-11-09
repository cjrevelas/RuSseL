#ifndef EVAL_ARG_H
#define EVAL_ARG_H

#include <string>
#include <memory>
#include <list>

#include "Russel.hpp"

namespace RusselNS {

class EvalArg {
  public:
    EvalArg();
    EvalArg(std::string argument, int order, int type, std::string associativity);

    virtual ~EvalArg();

    virtual void Operate() {};
    virtual void Operate(int &positionOfFirstArgument, int &positionOfSecondArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) {}

    inline bool IsOrder(int order) { return (order == order_); }

    inline int GetType() const { return type_; }
    inline int GetOrder() const { return order_; }
    inline int GetValue() const { return value_; }
    inline std::string GetStringValue() const { return stringValue_; }
    inline std::string GetAssociativity() const { return associativity_; }

    inline void SetOrder(int order) { order_ = order; }
    inline void SetValue(double value) { value_ = value; }

    static std::unique_ptr<Russel> russel_;

  protected:
    std::string stringValue_;
    int order_;
    int type_;
    std::string associativity_;
    double value_;
};

// Value
class Val : public EvalArg { public: Val(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Val(){} virtual void Operate();};

// Numerical operators and functions
class Add: public EvalArg { public: Add(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Add(){} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it); };
}


#endif
