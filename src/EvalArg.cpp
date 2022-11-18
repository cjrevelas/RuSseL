#include "EvalArg.hpp"
#include "StringOperations.hpp"

namespace RusselNS {

EvalArg::~EvalArg() {
#ifdef DEBUG
  std::cout << "Destroying argument with stringValue: " << stringValue_ << '\n';
#endif
}

EvalArg::EvalArg() {
  stringValue_ = "Base";
}

EvalArg::EvalArg(std::string argument, int order, int type, std::string associativity)
  : stringValue_(argument), order_(order), type_(type), associativity_(associativity), value_(0.0) {}

void Val::Operate() {
  value_ = StringToNumber<double>(stringValue_);
}

void PreVal::Operate() {
  std::string type = ""; // TODO: FIX THIS
}

void Add::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(valueLeft + valueRight);

#ifdef DEBUG
  std::cout << "ADD: " << valueLeft << " + " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif
}

} // RusselNS
