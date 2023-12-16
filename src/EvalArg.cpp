#include <iostream>

#include "EvalArg.hpp"
#include "StringOperations.hpp"

namespace RusselNS {

std::shared_ptr<class Russel> EvalArg::russel_ = nullptr;

EvalArg::EvalArg() {
  std::cout << "Calling EvalArg constructor\n";
  stringValue_ = "Base";
}

EvalArg::EvalArg(std::string argument, int order, int type, std::string associativity)
  : stringValue_(argument), order_(order), type_(type), associativity_(associativity), value_(0.0) {}

EvalArg::~EvalArg() {
#ifdef DEBUG
  std::cout << "Destroying argument with stringValue: " << stringValue_ << '\n';
#endif
}

// Value operators
void Val::Operate() {
  value_ = StringToNumber<double>(stringValue_);
}

void PreVal::Operate() {
  std::shared_ptr<class Variable> obj = russel_->memory_->GetVariable(stringValue_);

  std::string type = obj->type();

  if (type == "int") {
    value_ = static_cast<double>(obj->GetInteger());
  } else if (type == "dbl") {
    value_ = obj->GetDouble();
  } else if (type == "str") {
    value_ = StringToNumber<double>(obj->GetString());
  }

#ifdef DEBUG
  std::cout << "PREVAL: arg " << stringValue_ << " = " << GetValue() << '\n';
#endif
}

void Ref::Operate() {
  value_ = russel_->memory_->GetVariable(stringValue_)->GetCastToDouble();

#ifdef DEBUG
  std::cout << "DEREF: arg " << stringValue_ << " = " << GetValue() << '\n';
#endif
}

void Array1D::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::cout << "Entered Array1D::Operate() function\n";

  std::advance(it, -1);

  //int id = static_cast<int>((*it)->GetValue()) - 1;

  double value = 0.0; // FIXME

  (*it)->SetValue(value);

  iBegin = 0;
  iCount = 1;

#ifdef DEBUG
  std::cout << "Array1D: " << stringValue_ << "[" << id << "] = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void Array2D::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::cout << "Entered Array2D::Operate() function\n";

  std::advance(it, -1);
  //int id = static_cast<int>((*it)->GetValue()) - 1;

  std::advance(it, -1);
  //int component = static_cast<int>((*it)->GetValue()) - 1;

  double value = 0.0; // FIXME

  (*it)->SetValue(value);

  iBegin = -1;
  iCount =  2;

#ifdef DEBUG
  std::cout << "Array2D: " << stringValue_ << "[" << id << "][" << component << "] = " << (*it)->GetValue() << '\n';
#endif

  return;
}

// Arithmetic operators
void LeftBracket::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  iBegin = 0;
  iCount = 1;

  std::cout << "Parenthesis operator -> current argument value " << (*it)->GetValue() << '\n';

  ExitProgram("EvalArg::LeftBracket::Operate()", "A left bracket was spotted in an operation.");
  return;
}

void RightBracket::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  iBegin = 0;
  iCount = 1;

  std::cout << "Parenthesis operator -> current argument value " << (*it)->GetValue() << '\n';

  ExitProgram("EvalArg::RightBracket::Operate()", "A right bracket was spotted in an operation.");
  return;
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

void Sub::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(valueLeft - valueRight);

#ifdef DEBUG
  std::cout << "SUB: " << valueLeft << " - " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void SubLead::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  (*it)->SetValue( -(*it)->GetValue() );

  iBegin = 0;
  iCount = 1;

#ifdef DEBUG
  std::cout << "SUBLEAD: " << (*it)->GetValue() << '\n';
#endif

  return;
}

void Mul::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(valueLeft * valueRight);

#ifdef DEBUG
  std::cout << "MUL: " << valueLeft << " * " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void Div::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(valueLeft / valueRight);

#ifdef DEBUG
  std::cout << "DIV: " << valueLeft << " / " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void Asg::Operate(int &iBegin, int &iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::cout << "Entered Asg::Operate() function\n";

  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  std::shared_ptr<class Variable> obj = russel_->memory_->GetVariable(stringValue_);

  std::string type = obj->type();

  if (type == "dbl" || type == "pcdbl") {
    obj->SetValue(valueRight);
  }

  if (type == "int" || type == "pcint") {
    obj->SetValue(static_cast<int>(valueRight));
  }

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(valueRight);

#ifdef DEBUG
  std::cout << "Assign: " << stringValue_ << " -> " << valueRight << '\n';
#endif
}

// Logical operators
void Or::Operate(int &iBegin, int& iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(static_cast<bool>(valueLeft) || static_cast<bool>(valueRight));

#ifdef DEBUG
  std::cout << "OR: " << valueLeft << " || " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void And::Operate(int &iBegin, int& iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double valueRight = (*it)->GetValue();

  std::advance(it, -1);
  double valueLeft = (*it)->GetValue();

  iBegin = -1;
  iCount = 2;

  (*it)->SetValue(static_cast<bool>(valueLeft) && static_cast<bool>(valueRight));

#ifdef DEBUG
  std::cout << "AND: " << valueLeft << " && " << valueRight << " = " << (*it)->GetValue() << '\n';
#endif

  return;
}

void Not::Operate(int &iBegin, int& iCount, std::_List_iterator<std::unique_ptr<EvalArg>> it) {
  std::advance(it, -1);
  double value= (*it)->GetValue();

  iBegin = 0;
  iCount = 1;

  (*it)->SetValue( !(static_cast<bool>(value)) );

#ifdef DEBUG
  std::cout << "NOT: " << value << (*it)->GetValue() << '\n';
#endif

  return;
}

void Special::Operate() {
#ifdef DEBUG
  std::cout << "SPECIAL: " << GetStringValue() << '\n';
#endif
}

} // RusselNS
