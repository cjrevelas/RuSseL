#include <memory>
#include <Evaluator.hpp>
#include <sstream>

#include "StringOperations.hpp"
#include "IOHelper.hpp"

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

double EvaluateExpression(std::list<std::unique_ptr<EvalArg>> &expressionPostfix) {
  // Perform fresh evaluations of Ref types
  for (std::list<std::unique_ptr<EvalArg>>::iterator it=expressionPostfix.begin(); it!=expressionPostfix.end(); ++it) {
    if ((*it)->GetType() == kValue) {
      (*it)->Operate();
    }
  }

  // Evaluate the expression
  for (std::list<std::unique_ptr<EvalArg>>::iterator it=expressionPostfix.begin(); it!=expressionPostfix.end(); ++it) {
    int iBegin = 0; // Marks the position of the first argument to be deleted
    int iCount = 0; // Marks the number of arguments to be deleted

    (*it)->Operate(iBegin, iCount, it);

    // Delete the evaluated arguments from the expression
    std::advance(it, iBegin);
    for (int kk = 0; kk<iCount; ++kk) {
      expressionPostfix.erase(it++);
    }

    if (iCount) --it;
  }

  double value = expressionPostfix.front()->GetValue();

  if (expressionPostfix.size() > 1) {
    ExitProgram("EvaluateExpression", "Problem with evaluating the expression.");
  }

  return value;
}


// The following function introduces additional whitespaces between arguments in order
// to facilitate the parsing of the expression
void PreconditionExpression(std::string &stringExpressionPrefix) {
  std::string auxExpression = "";

  for (int ii=0; ii<stringExpressionPrefix.size(); ++ii) {
    char character = stringExpressionPrefix[ii];

    bool addSpaceBefore = false;
    bool addSpaceAfter  = false;

    // Deal with single operators
    if (character == '(' ||
        character == ')' ||
        character == '+' ||
        character == '-' ||
        character == '*' ||
        character == '/' ||
        character == '^' ||
        character == '!' ||
        character == ',') {
      addSpaceBefore = true;
      addSpaceAfter  = true;
    // Deal with operators that form pairs
    } else if (character == '=' ||
               character == '>' ||
               character == '<') {
      char previousCharacter = stringExpressionPrefix[ii-1];
      if (previousCharacter != ' ' &&
          previousCharacter != '=' &&
          previousCharacter != '>' &&
          previousCharacter != '<') {
        addSpaceBefore = true;
      }
      char nextCharacter = stringExpressionPrefix[ii+1];
      if (nextCharacter != ' ' &&
          nextCharacter != '=' &&
          nextCharacter != '>' &&
          nextCharacter != '<') {
        addSpaceAfter = true;
      }
    } else if (character == 'e' ||
               character == 'E') {
      char previousCharacter = stringExpressionPrefix[ii-1];
      if (previousCharacter == '.' || std::isdigit(previousCharacter)) {
        addSpaceBefore = true;
      }
      char nextCharacter = stringExpressionPrefix[ii+1];
      if (nextCharacter == '.' || std::isdigit(nextCharacter)) {
        addSpaceAfter = true;
      }
    }

    if (addSpaceBefore) auxExpression += " ";

    auxExpression += character;

    if (addSpaceAfter) auxExpression += " ";
  }

  stringExpressionPrefix = auxExpression;
}


} // RusselNS
