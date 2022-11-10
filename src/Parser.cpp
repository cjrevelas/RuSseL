#include <list>

#include "Parser.hpp"
#include "IOHelper.hpp"
#include "EvalArg.hpp"
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
  std::cout <<  flag << coeffsMinLength << '\n';
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
} // RusselNS
