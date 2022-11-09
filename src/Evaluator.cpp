#include <memory>
#include <Evaluator.hpp>
#include <sstream>

namespace RusselNS {

void ConvertStringToExpression (std::string stringExpressionPrefix, std::list<std::unique_ptr<EvalArg>> &stringExpressionPostfix) {
  std::list<std::unique_ptr<EvalArg>> expressionPrefix;

  // Precondition the string expression by adding spaces between the operators
  PreconditionExpression(stringExpressionPrefix);

  // If true, the argument is positioned after a left bracker, or it is the first
  // argument of the expression
  bool leadingOperator = true;

  // Parse the arguments of the string prefix expression
  std::string argument;
  std::stringstream ss(stringExpressionPrefix);


}


} // RusselNS
