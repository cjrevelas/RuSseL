#ifndef EVAL_ARG_HPP
#define EVAL_ARG_HPP

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
class Val     : public EvalArg { public: Val    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Val()     {} virtual void Operate();};
class Ref     : public EvalArg { public: Ref    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Ref()     {} virtual void Operate();};
class PreVal  : public EvalArg { public: PreVal (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~PreVal()  {} virtual void Operate();};
class Array1D : public EvalArg { public: Array1D(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Array1D() {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it);};
class Array2D : public EvalArg { public: Array2D(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Array2D() {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it);};

// Numerical operators and functions
class Add: public EvalArg { public: Add(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Add(){} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it); };
class Sub: public EvalArg { public: Sub(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Sub(){} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it); };

// Brackets and separators
class LeftBracket:  public EvalArg { public: LeftBracket (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~LeftBracket(){}  virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it); };
class RightBracket: public EvalArg { public: RightBracket(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~RightBracket(){} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it); };

} // RusselNS


#endif // EVAL_ARG_HPP
