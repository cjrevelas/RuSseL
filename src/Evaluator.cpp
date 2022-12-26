#include <memory>
#include <Evaluator.hpp>
#include <sstream>

#include "StringOperations.hpp"
#include "IOHelper.hpp"

namespace RusselNS {

void ConvertStringToExpression(std::string stringExpressionPrefix, std::list<std::unique_ptr<EvalArg>> &expressionPostfix) {
  std::list<std::unique_ptr<EvalArg>> expressionPrefix;

  // Precondition the string expression by adding spaces between the operators
  PreconditionExpression(stringExpressionPrefix);

  // If true, the argument is positioned after a left bracket, or it is the first
  // argument of the expression
  bool leadingOperator = true;

  // Parse the arguments of the string prefix expression
  std::string argument;
  std::stringstream ss(stringExpressionPrefix);

  while (ss >> argument) {
    // IsNumber reads '+' and '-' as numbers
    if (IsNumber<double>(argument) && argument != "+" && argument != "-") {
      expressionPrefix.push_back(std::make_unique<Val>(argument, -1, kValue, ""));
      leadingOperator = false;
      continue;
    } else if (argument.rfind("s_", 0) == 0 ||
               argument.rfind("v_", 0) == 0 ||
               argument.rfind("i_", 0) == 0 ||
               argument.rfind("c_", 0) == 0 ||
               argument.rfind("p_", 0) == 0) {

      std::string name;
      std::vector<std::string> argumentVector;
      ParseArray(argument, name, argumentVector);

      switch (argumentVector.size()) {
        case 0:
          expressionPrefix.push_back(std::make_unique<Ref>(argument, -1, kValue, ""));
          break;
        case 1:
          expressionPrefix.push_back(std::make_unique<Array1D>(name, 8, kFunction, "L"));
          expressionPrefix.push_back(std::make_unique<Val>(argumentVector[0], -1, kValue, ""));
          if (StringToNumber<int>(argumentVector[0]) - 1 < 0) ExitProgram("Eval", "Array component is out of bounds.");
          break;
        case 2:
          expressionPrefix.push_back(std::make_unique<Array2D>(name, 8, kFunction, "L"));
          expressionPrefix.push_back(std::make_unique<Val>(argumentVector[1], -1, kValue, ""));
          expressionPrefix.push_back(std::make_unique<Val>(argumentVector[0], -1, kValue, ""));
          if (StringToNumber<int>(argumentVector[0]) - 1 < 0) ExitProgram("Eval","Array component is out of bounds.");
          if (StringToNumber<int>(argumentVector[1]) - 1 < 0) ExitProgram("Eval", "Array component is out of bounds.");
          break;
        default:
          ExitProgram("Eval","There is no support for arrays other than 1D and 2D.");
          break;
      }

      leadingOperator = false;
      continue;

    } else if (argument.rfind("$s_", 0) == 0 ||
               argument.rfind("$v_", 0) == 0 ||
               argument.rfind("$i_", 0) == 0 ||
               argument.rfind("$c_", 0) == 0 ||
               argument.rfind("$p_", 0) == 0) {
      argument.erase(0,1);
      expressionPrefix.push_back(std::make_unique<PreVal>(argument, -1, kValue, ""));

      leadingOperator = false;
      continue;

    } else if (argument.rfind("S_", 0) == 0 ||
               argument.rfind("V_", 0) == 0 ||
               argument.rfind("I_", 0) == 0 ||
               argument.rfind("C_", 0) == 0 ||
               argument.rfind("P", 0) == 0) {

      // Check whether it is an array; if yes, read the arguments.
      std::string name;
      std::vector<std::string> arguments;
      ParseArray(argument, name, arguments);

      switch (arguments.size()) {
        case 0:
          expressionPrefix.push_back(std::make_unique<Special>(argument, -1, kFunction, ""));
          break;
        case 1:
          expressionPrefix.push_back(std::make_unique<Special>(name, 8, kFunction,  "L"));
          expressionPrefix.push_back(std::make_unique<Val>(arguments[0], -1, kValue, ""));

          if (StringToNumber<int>(arguments[0]) - 1 < 0) ExitProgram("Eval", "Array component is out of bounds.");
          break;
        case 2:
          expressionPrefix.push_back(std::make_unique<Special>(name, 8, kFunction, "L"));
          expressionPrefix.push_back(std::make_unique<Val>(arguments[1], -1, kValue, ""));
          expressionPrefix.push_back(std::make_unique<Val>(arguments[0], -1, kValue, ""));

          if (StringToNumber<int>(arguments[0]) - 1 < 0) ExitProgram("Eval", "Array component is out of bounds.");
          if (StringToNumber<int>(arguments[1]) - 1 < 0) ExitProgram("Eval", "Array component is out of bounds.");
          break;
        default:
          ExitProgram("Eval", "There  is no support for arrays other than 1D and 2D.");
          break;
      }
      leadingOperator = false;
      continue;
    } else if (argument == "(") {
      expressionPrefix.push_back(std::make_unique<LeftBracket>(argument, -1, kLeftBracket, ""));
      std::cout << "Oh look, a parethesis operator\n";
    } else if (argument == ")") {
      expressionPrefix.push_back(std::make_unique<RightBracket>(argument, -1, kRightBracket, ""));
      leadingOperator = false;
      continue;
    } else if (argument == "=") {
      expressionPrefix.push_back(std::make_unique<Asg>(argument, 1, kOperator, "L"));
    } else if (argument == "or") {
      expressionPrefix.push_back(std::make_unique<Or>(argument, 2, kOperator, "L"));
    } else {
      expressionPrefix.push_back(std::make_unique<Special>(argument, -1, kFunction, ""));
    }

    leadingOperator = true;
  }
}


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

  for (int ii=0; ii < static_cast<int>(stringExpressionPrefix.size()); ++ii) {
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
