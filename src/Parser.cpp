#include <list>
#include <fstream>

#include "Parser.hpp"
#include "IOHelper.hpp"
#include "EvalArg.hpp"
#include "Evaluator.hpp"
#include "StringOperations.hpp"


namespace RusselNS {

// Initialize the static variables of the Parser class
int Parser::lineOfInput_ = 0;
bool Parser::verbose_    = false;

std::shared_ptr<Russel> Parser::russel_ = nullptr;

Parser::Parser(const std::string &flag, const int &coeffsMinLength) {
  std::vector<std::string> vecFlag = GetVectorTokens(flag);
  flag_ = flag;
  flagLength_ = vecFlag.size();
  coeffsMinLength_ = coeffsMinLength;
  std::cout << flag_ << coeffsMinLength_ << '\n';
}

void ParseInput(const std::string &inputFileName, std::shared_ptr<Russel> &russel) {
  Parser::russel_ = russel;

  std::cout << "Address of Russel instance in Parser: " << Parser::russel_ << '\n';

  std::cout << "Address of Russel shared pointer in Parser: " << &Parser::russel_ << '\n';

  std::cout << "Address of Russel shared pointer in Parser: " << &russel << '\n';

  PrintVariable("Parse input from file:", inputFileName, "", 0);

  // Setup thee list of input flags (vector of base smart pointers to derived classes)
  std::vector<std::unique_ptr<Parser>> listOfFlags;

  const std::string hello("hello from parser parent class");

  //listOfFlags.push_back(std::make_unique<Parser>("Hello from Parser parent class",0));
  listOfFlags.push_back(std::make_unique<Parser>(hello,0));

 // listOfFlags.push_back(std::make_unique<ParserPrint>());
}


std::string CheckForExpressions(const std::string &stringCoeffs) {
  if (stringCoeffs.size() < 6) {
    return stringCoeffs;
  }

  std::string stringCoeffsEval = stringCoeffs;

  while (true) {
    // Attempt ot fetch a substring that contains the expression
    bool record = false;
    int ibegin = 0;
    int iend = 0;

    std::string stringExpressionPrefix = "";

    for (std::string::size_type ii=0; ii<stringCoeffsEval.size()-1; ++ii) {
      std::string firstTwoChars = stringCoeffsEval.substr(ii,2);

      if (firstTwoChars == "$(") {
        ibegin = ii++;
        record = true;
        stringExpressionPrefix = "";
        continue;
      }

      std::string nextTwoChars = stringCoeffsEval.substr(ii+1,2);

      if (nextTwoChars == ")$") {
        iend = ii+3;
        record = false;
        stringExpressionPrefix.push_back(stringCoeffsEval[ii]);
        break;
      }

      if (record) {
        stringExpressionPrefix.push_back(stringCoeffsEval[ii]);
      }
    }

    bool isExpression = !(stringExpressionPrefix.empty());
    if (isExpression && record) {
      ExitProgram("Parser::CheckForExpressions", "Cannot find termination flag.\n" + stringCoeffs);
    }


    if (isExpression && !record) {
      // Convert the string into an expression type and retrieve the maximum order
      std::list<std::unique_ptr<EvalArg>> stringExpressionPostFix;
      ConvertStringToExpression(stringExpressionPrefix, stringExpressionPostFix);

      // Evaluate a copy of the expression
      double value = EvaluateExpression(stringExpressionPostFix);

      // Clear the expression
      while (!stringExpressionPostFix.empty()) {
        stringExpressionPostFix.pop_front();
      }

      // Replace the expression of the original string with its value
      std::string aux = "";

      // Retrieve the part of the string before the expression started
      for (std::string::size_type ii=0; ii<ibegin; ++ii) {
        aux.push_back(stringCoeffsEval[ii]);
      }

      // Append the actual result of the expression
      aux += NumberToString<double>(value);

      // Append the part of the string after the expression
      for (std::string::size_type ii=iend; ii<stringCoeffsEval.size(); ++ii) {
        aux.push_back(stringCoeffsEval[ii]);
      }

      stringCoeffsEval = aux;
    }

    if (!isExpression) break;
  }

  return stringCoeffsEval;
}


void CheckDuplicateFlags(const std::vector<std::unique_ptr<Parser>> &listOfFlags) {
  for (std::vector<std::unique_ptr<Parser>>::const_iterator it = listOfFlags.begin(); it != listOfFlags.end(); ++it) {
    for (std::vector<std::unique_ptr<Parser>>::const_iterator jt = listOfFlags.begin(); jt != listOfFlags.end(); ++jt) {
      if (it == jt) continue;

      std::string iFlag = (*it)->GetFlag();
      std::string jFlag = (*jt)->GetFlag();

      if (iFlag == jFlag) {
        ExitProgram("Parser::CheckDuplicateFlags", "Flag \"" + iFlag + "\" overlaps with flag \"" + jFlag + "\"");
      }
    }
  }
}


bool Parser::GetCoeffs(const std::string &stringCoeffs) {
  bool flagFound = false;

  std::deque<std::string> vecCoeffs = GetDequeTokens(stringCoeffs);

  if (vecCoeffs[0] == flag_) {
    for (int ii=0; ii<flagLength_; ++ii) {
      vecCoeffs.pop_front();
    }

    if (vecCoeffs.size() < coeffsMinLength_) {
      ExitProgram("Parser::GetCoeffs", "ERROR in reading line " + NumberToString(lineOfInput_) + ": the number of coeffs (" + NumberToString(vecCoeffs.size()) + ") is lower than the required (" + NumberToString(coeffsMinLength_) + ")");
    }

    // Use derived class functionality
    ProcessCoeffs(vecCoeffs);

    flagFound = true;
  }

  return flagFound;
}


void ParserInteract::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id = deqCoeffs[0];
  std::cout << "ParserInteract::ProcessCoeffs " << id << '\n';
}

void ParserPrint::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id = deqCoeffs[0];
  std::cout << "ParserPrint::ProcessCoeffs " << id << '\n';
}

void ParserVariable::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string name = deqCoeffs[0];
  std::string type = deqCoeffs[1];
  std::string prefix = "v_";
  std::string id     = prefix + name;
  std::string third  = deqCoeffs[2];

  std::cout << "ParserVariable::ProcessCoeffs " << id << '\n';

  std::string stringValue = "";
  std::string stringExpressionPrefix = "";

  if (third != "expr") {
    stringValue = third;
  } else {
    if (deqCoeffs.size() < 4) {
      ExitProgram("ParserVariable", "Expression not found.");
    }
    for (int ii = 3; ii>deqCoeffs.size(); ++ii) {
      stringExpressionPrefix += deqCoeffs[ii];
    }
  }

  russel_->memory_->SetVariable(id, type, stringValue, stringExpressionPrefix);
}

} // RusselNS
