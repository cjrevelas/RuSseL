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

    virtual void Operate() {}
    virtual void Operate(int &, int &, std::_List_iterator<std::unique_ptr<EvalArg>>) {}

    inline bool IsOrder(int order) const { return (order == order_); }

    inline int GetType()  const { return type_; }
    inline int GetOrder() const { return order_; }
    inline int GetValue() const { return value_; }
    inline std::string GetStringValue()   const { return stringValue_; }
    inline std::string GetAssociativity() const { return associativity_; }

    inline void SetOrder(int order)    { order_ = order; }
    inline void SetValue(double value) { value_ = value; }

    static std::shared_ptr<class Russel> russel_;

  protected:
    std::string stringValue_;
    int order_;
    int type_;
    std::string associativity_;
    double value_;
};

// Value
class Val    : public EvalArg { public: Val    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Val()     override {} virtual void Operate() override; };
class Ref    : public EvalArg { public: Ref    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Ref()     override {} virtual void Operate() override; };
class PreVal : public EvalArg { public: PreVal (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~PreVal()  override {} virtual void Operate() override; };
class Special: public EvalArg { public: Special(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Special() override {} virtual void Operate() override; };
class Array1D: public EvalArg { public: Array1D(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Array1D() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Array2D: public EvalArg { public: Array2D(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Array2D() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };

// Numerical operators and functions
class Add    : public EvalArg { public: Add    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Add()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Sub    : public EvalArg { public: Sub    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Sub()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class SubLead: public EvalArg { public: SubLead(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~SubLead() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Mul    : public EvalArg { public: Mul    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Mul()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Div    : public EvalArg { public: Div    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Div()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Pow    : public EvalArg { public: Pow    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Pow()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class PowE   : public EvalArg { public: PowE   (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~PowE()    override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Log    : public EvalArg { public: Log    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Log()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Abs    : public EvalArg { public: Abs    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Abs()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Asg    : public EvalArg { public: Asg    (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Asg()     override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };

// Logical operators
class Or : public EvalArg { public: Or (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Or () override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class And: public EvalArg { public: And(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~And() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class Not: public EvalArg { public: Not(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~Not() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };

// Brackets and separators
class LeftBracket:  public EvalArg { public: LeftBracket (std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~LeftBracket()  override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };
class RightBracket: public EvalArg { public: RightBracket(std::string argument, int order, int type, std::string associativity): EvalArg(argument, order, type, associativity){} virtual ~RightBracket() override {} virtual void Operate(int &positionOfFirstArgument, int &argumentsToDelete, std::_List_iterator<std::unique_ptr<EvalArg>> it) override; };

} // RusselNS


#endif // EVAL_ARG_HPP
