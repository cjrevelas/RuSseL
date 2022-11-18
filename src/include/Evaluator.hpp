#ifndef EVALUATOR_HPP
#define EVALUATOR_HPP

#include <string>
#include <memory>
#include <list>

#include <EvalArg.hpp>

namespace RusselNS {

void PreconditionExpression(std::string &);
void ConvertStringToExpression(std::string, std::list<std::unique_ptr<EvalArg>> &);
double EvaluateExpression(std::list<std::unique_ptr<EvalArg>> &);

enum {
  kValue,
  kFunction,
  kOperator,
  kLeftBracket,
  kRightBracket,
};

}  // RusselNS

#endif  // EVALUATOR_HPP
