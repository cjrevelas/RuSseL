#include <list>
#include <fstream>

#include "Parser.hpp"
#include "IOHelper.hpp"
#include "EvalArg.hpp"
#include "Evaluator.hpp"
#include "StringOperations.hpp"
#include "EosHelfand.hpp"
#include "EosSanchezLacombe.hpp"
#include "ContourUniform.hpp"
#include "ContourSymmetric.hpp"
#include "ContourAsymmetric.hpp"
#include "ContourHybrid.hpp"

namespace RusselNS {

// Initialize the static variables of the Parser class
int Parser::lineOfInput_ = 0;
bool Parser::verbose_    = false;

std::shared_ptr<Russel> Parser::russel_ = nullptr;

Parser::Parser(const std::string &flag, const int &coeffsMinLength) {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Create new Parser instance", 1);
#endif
  std::vector<std::string> vecFlag = GetVectorTokens(flag);
  flag_            = flag;
  flagLength_      = vecFlag.size();
  coeffsMinLength_ = coeffsMinLength;
}

Parser::~Parser() {
#ifdef EXPORT_MEMORY_STATUS
  PrintMessage("Delete Parser instance", 1);
#endif
}

void ParseInput(const std::string &inputFileName, std::shared_ptr<Russel> &russel) {
  Parser::russel_ = russel;

#ifdef EXPORT_MEMORY_STATUS
  PrintVariable("Number of russel shared pointers [Parser]: ", russel.use_count(), "", 0);
#endif

  PrintMessage("Parsing input from file: " + inputFileName, 1);

  // Setup the list of input flags (vector of base smart pointers to derived classes)
  std::vector<std::unique_ptr<Parser>> listOfFlags;
  std::vector<std::unique_ptr<Parser>>::iterator listOfFlagsIt;

  listOfFlags.push_back(std::make_unique<ParserInteract>("interact", 1));
  listOfFlags.push_back(std::make_unique<ParserPrint>("print", 1));
  listOfFlags.push_back(std::make_unique<ParserVariable>("variable", 2));
  listOfFlags.push_back(std::make_unique<ParserMesh>("mesh", 1));
  listOfFlags.push_back(std::make_unique<ParserEos>("eos", 2));
  listOfFlags.push_back(std::make_unique<ParserContour>("contour", 2));
  listOfFlags.push_back(std::make_unique<ParserChain>("chain", 4));

  CheckDuplicateFlags(listOfFlags);

  std::ifstream inputFile(inputFileName.c_str(), std::ifstream::in);

  std::vector<std::string> linesOfFile;

  std::string stringCoeffs = "";

  while (inputFile.good()) {
    std::string rawLine;
    std::getline(inputFile, rawLine);

    Parser::lineOfInput_ += 1;

    std::string commentStrippedLine = "";

    for (char &ch : rawLine) {
      if (ch == '#') break;
      commentStrippedLine += ch;
    }

    if (commentStrippedLine.empty()) continue;

    stringCoeffs += " " + commentStrippedLine;

    // In case the last char of the line is &, merge it with the next one
    bool continueToNextLine = false;

    for (char &ch : ReverseString(commentStrippedLine)) {
      if (ch == '&') continueToNextLine = true;
    }

    if (continueToNextLine) continue;

    // Sanitize the string by removing extra spaces and '&', if any
    stringCoeffs = ReplaceSubstring(stringCoeffs, "&", "");
    stringCoeffs = RemoveMultipleSpaces(stringCoeffs);
    stringCoeffs = CheckForExpressions(stringCoeffs);

    // Try to read the string of coeffs
    bool parseFailure = true;
    for (listOfFlagsIt = listOfFlags.begin(); listOfFlagsIt != listOfFlags.end(); ++listOfFlagsIt) {
      if ((*listOfFlagsIt)->GetCoeffs(stringCoeffs)) {
        parseFailure = false;
        break;
      }
    }

    if (parseFailure) {
      PrintWarning("ParseInput", "Unknown command: " + stringCoeffs);
    }

    stringCoeffs = "";
  }

  inputFile.close();

  // Update the main::russel pointer
  russel = Parser::russel_;
  Parser::russel_.reset();
}


std::string CheckForExpressions(const std::string &stringCoeffs) {
  if (stringCoeffs.size() < 6) {
    return stringCoeffs;
  }

  std::string stringCoeffsEval = stringCoeffs;

  while (true) {
    // Attempt ot fetch a substring that contains the expression
    bool record = false;
    int ibegin  = 0;
    int iend    = 0;

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
      for (int /*std::string::size_type*/ ii=0; ii<ibegin; ++ii) {
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

    if (static_cast<int>(vecCoeffs.size()) < coeffsMinLength_) {
      ExitProgram("Parser::GetCoeffs", "ERROR in reading line " + NumberToString(lineOfInput_) + ": the number of coeffs (" + NumberToString(vecCoeffs.size()) + ") is lower than the required (" + NumberToString(coeffsMinLength_) + ")");
    }

    // Use derived class functionality
    ProcessCoeffs(vecCoeffs);

    flagFound = true;
  }

  return flagFound;
}

void ParserChain::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id   = deqCoeffs[0];
  std::string kind = deqCoeffs[1];

  PrintMessage("Add chain of kind " + kind + " with id " + id, 1);

  std::shared_ptr<class Chain> chain = std::make_shared<class Chain>(id, russel_);

  russel_->memory_->SetChain(id,chain);
  russel_->memory_->GetChain(id)->Parse(deqCoeffs);
}

void ParserContour::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id    = deqCoeffs[0];
  std::string style = deqCoeffs[1];

  PrintMessage("Add contour " + style + " with id " + id, 1);

  std::shared_ptr<class Contour> contour;

  if (style == "uniform") {
    contour = std::make_shared<class ContourUniform>(id, russel_);
  } else if (style == "symmetric") {
    contour = std::make_shared<class ContourSymmetric>(id, russel_);
  } else if (style == "asymmetric") {
    contour = std::make_shared<class ContourAsymmetric>(id, russel_);
  } else if (style == "hybrid") {
    contour = std::make_shared<class ContourHybrid>(id, russel_);
  } else {
    ExitProgram("ParserContour", "Unknown Contour style \"" + style + "\"");
  }

  russel_->memory_->SetContour(id, contour);
  russel_->memory_->GetContour(id)->Parse(deqCoeffs);
  russel_->memory_->GetContour(id)->Report();
}

void ParserEos::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id    = deqCoeffs[0];
  std::string style = deqCoeffs[1];

  PrintMessage("Add eos " + style + " with id " + id, 1);

  std::shared_ptr<class Eos> eos;

  if (style == "sanchez-lacombe") {
    eos = std::make_shared<class EosSanchezLacombe>(id, russel_);
  } else if (style == "helfand") {
    eos = std::make_shared<class EosHelfand>(id, russel_);
  } else {
    ExitProgram("ParserEos", "Unknown EoS style \"" + style + "\"");
  }

  russel_->memory_->SetEos(id, eos);
  russel_->memory_->GetEos(id)->Parse(deqCoeffs);
  russel_->memory_->GetEos(id)->Report();
}

void ParserInteract::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id = deqCoeffs[0];
}

void ParserPrint::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id = deqCoeffs[0];
}

void ParserMesh::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string id = deqCoeffs[0];

  PrintMessage("Add mesh with id " + id, 1);

  int graftPointId = 4852;

  russel_->memory_->mesh_ = std::make_shared<class Mesh>("in.mesh");

  russel_->memory_->mesh_->ElementsContainingNode(graftPointId);
  russel_->memory_->mesh_->ComputeMeshVolume();
  russel_->memory_->mesh_->ComputeBulkNodePairs();
}

void ParserVariable::ProcessCoeffs(std::deque<std::string> &deqCoeffs) {
  std::string name   = deqCoeffs[0];
  std::string type   = deqCoeffs[1];
  std::string prefix = "v_";
  std::string id     = prefix + name;
  std::string third  = deqCoeffs[2];

  std::string stringValue = "";
  std::string stringExpressionPrefix = "";

  if (third != "expr") {
    stringValue = third;
  } else {
    if (deqCoeffs.size() < 4) {
      ExitProgram("ParserVariable", "Expression not found.");
    }
    for (int ii = 3; ii>static_cast<int>(deqCoeffs.size()); ++ii) {
      stringExpressionPrefix += deqCoeffs[ii];
    }
  }

  russel_->memory_->SetVariable(id, type, stringValue, stringExpressionPrefix);
}

} // RusselNS