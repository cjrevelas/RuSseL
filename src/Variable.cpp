#include "Variable.hpp"
#include "Evaluator.hpp"
#include "StringOperations.hpp"


namespace RusselNS {

Variable::Variable(std::string itype, std::string stringValue, std::string stringExpressionPrefix) {
  int_ = 0;
  dbl_ = 0.0;
  str_ = "";
  Reset(itype, stringValue, stringExpressionPrefix);
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + "), value(" + stringValue+ "), expression(" + stringExpressionPrefix + ")", 2);
#endif
}

Variable::Variable(int xx) {
  type_ = "int";
  int_  = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + "), value(" + NumberToString(int_) + ")", 2);
#endif
}

Variable::Variable(double xx) {
  type_ = "dbl";
  dbl_  = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + "), value(" + NumberToString(dbl_) + ")", 2);
#endif
}

Variable::Variable(std::string xx) {
  type_ = "str";
  str_  = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + "), value(" + str_ + ")", 2);
#endif
}

Variable::Variable(std::shared_ptr<const int> xx) {
  type_  = "pcint";
  pcint_ = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + ")", 2);
#endif
}

Variable::Variable(std::shared_ptr<const double> xx) {
  type_  = "pcdbl";
  pcdbl_ = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + ")", 2);
#endif
}

Variable::Variable(std::shared_ptr<const std::string> xx) {
  type_  = "pcstr";
  pcstr_ = xx;
  isExpression_ = false;
#ifdef DEV_REPORT_HEAP
  PrintMessage("Add Variable, type(" + type_ + ")", 2);
#endif
}

void Variable::Reset(std::string itype, std::string stringValue, std::string stringExpressionPrefix) {
  type_ = itype;
  stringExpressionPrefix_ = stringExpressionPrefix;
  isExpression_ = !(stringExpressionPrefix_.empty());

  if (isExpression_) {
    PreconditionExpression(stringExpressionPrefix_);

    // Clear the expression, if any
    ClearExpression();

    // Convert the string into an expression type and retrieve the maximum order
    ConvertStringToExpression(stringExpressionPrefix_, expressionPostfix_);
  } else {
    if (type_ == "int") {
      int_ = StringToNumber<int>(stringValue);
    } else if (type_ == "dbl") {
      dbl_ = StringToNumber<double>(stringValue);
    } else if (type_ == "str") {
      str_ = stringValue;
    } else {
      ExitProgram("Variable:", "Unknown type: " + itype);
    }
  }
}

Variable::~Variable() {
#ifdef DEV_REPORT_HEAP
  PrintMessage("Deleting Variable, type(" + type_ + "), expression(" + stringExpressionPrefix_ + ")", 2);
#endif
  ClearExpression();
}

void Variable::ClearExpression() {
  while(!expressionPostfix_.empty()) {
    expressionPostfix_.pop_front();
  }
}

int Variable::GetCastToInteger() {
  if (isExpression_) {
    int_ = static_cast<int>(EvaluateExpression(expressionPostfix_));
  } else if (type_ == "int") {
    // int_ = int_;
  } else if (type_ == "dbl") {
    int_ = static_cast<int>(dbl_);
  } else if (type_ == "str") {
    int_ = StringToNumber<int>(str_);
  } else if (type_ == "pcint") {
    int_ = *pcint_;
  } else if (type_ == "pcdbl") {
    int_ = static_cast<int>(*pcdbl_);
  } else if (type_ == "pcstr") {
    int_ = StringToNumber<int>(*pcstr_);
  }

  return int_;
}

double Variable::GetCastToDouble() {
  if (isExpression_) {
    dbl_ = static_cast<double>(EvaluateExpression(expressionPostfix_));
  } else if (type_ == "int") {
    dbl_ = static_cast<double>(int_);
  } else if (type_ == "dbl") {
    // dbl_ = dbl_;
  } else if (type_ == "str") {
    dbl_ = StringToNumber<double>(str_);
  } else if (type_ == "pcint") {
    dbl_ = static_cast<double>(*pcint_);
  } else if (type_ == "pcdbl") {
    dbl_ = *pcdbl_;
  } else if (type_ == "pcstr") {
    dbl_ = StringToNumber<double>(*pcstr_);
  }

  return dbl_;
}

std::string Variable::GetCastToString(int precision) {
  std::stringstream buffer;
  buffer << std::setprecision(precision);

  if (isExpression_) {
    buffer << EvaluateExpression(expressionPostfix_);
  } else if (type_ == "int") {
    buffer << int_;
  } else if (type_ == "dbl") {
    buffer << dbl_;
  } else if (type_ == "str") {
    buffer << str_;
  } else if (type_ == "pcint") {
    buffer << *pcint_;
  } else if (type_ == "pcdble") {
    buffer << *pcdbl_;
  } else if (type_ == "pcstr") {
    buffer << *pcstr_;
  }

  return buffer.str();
}


} // RusselNS
