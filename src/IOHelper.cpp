#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include "IOHelper.hpp"
#include "StringOperations.hpp"


namespace RusselNS {

std::map<char,int> Convert::XYZToInt = Convert::CreateMapXYZToInt();
std::map<int,char> Convert::IntToXYZ = Convert::CreateMapIntToXYZ();

std::map<std::string,bool> convertYesNo2Bool = Convert::CreateMapYesNoToBool();
std::map<bool,std::string> convertBool2YesNo = Convert::CreateMapBoolToYesNo();

void ParseArray(const std::string &flag, std::string &name, std::vector<std::string> &arguments) {
  std::string auxString = EraseCharFromString(flag, ']');
  std::vector<std::string> tokens = GetVectorTokensDelimiter(auxString, '[');

  name = tokens[0];

  arguments.clear();

  for (int ii=1; ii<static_cast<int>(tokens.size()); ++ii) {
    arguments.push_back(tokens[ii]);
  }
}

std::vector<std::string> ParseUntilDash(const std::vector<std::string> &vecCoeffs, std::vector<std::string>::iterator &it) {
  std::vector<std::string> coeffList;

  while (it != vecCoeffs.end()) {
    std::string arg = (*it);

    if (RusselNS::IsNumber<double>(arg) && arg[0] == '-') {
      break;
    } else {
      coeffList.push_back(arg);
    }

    ++it;
  }

  // --it;

  return coeffList;
}

std::deque<std::string> ParseUntilDash(const std::deque<std::string> &deqCoeffs, std::deque<std::string>::iterator &it) {
  std::deque<std::string> coeffList;

  while (it != deqCoeffs.end()) {
    std::string arg = (*it);

    if (RusselNS::IsNumber<double>(arg) && arg[0] == '-') {
      break;
    } else {
      coeffList.push_back(arg);
    }

    ++it;
  }

  // --it;

  return coeffList;
}

void PrintMessage(const std::string &message, const int indentLevel) {
  int widthLevel = indentLevel * 3;

  std::cout << std::left;
  std::cout << std::setw(0) << "";
  std::cout << std::left;
  std::cout << std::setw(widthLevel) << "";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  std::cout << std::right;
}

void PrintWarning(const std::string &message) {
  std::cout << std::left;
  std::cout << std::setw(3 ) << "- ";
  std::cout << std::left;
  std::cout << std::setw(9) << "";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  std::cout << std::right;
}

void PrintWarning(const std::string &caller, const std::string &message) {
  std::cout << std::left;
  std::cout << std::setw(3 ) << "- ";
  std::cout << std::left;
  std::cout << std::setw(11 + caller.size()) << "Warning(" + caller + "):";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  std::cout << std::right;
}


void ExitProgram(const std::string &message) {
  std::cout << '\n';
  std::cout << message << '\n';
  exit(EXIT_SUCCESS);
}

void ExitProgram(const std::string &caller, const std::string &message) {
  std::cout << '\n';
  std::cout << std::left;
  std::cout << std::setw(12 + caller.size()) << "Quitting(" + caller + "):";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  exit(EXIT_SUCCESS);
}

bool FileExists(const std::string &fileName) {
  std::ifstream file;

  file.open(fileName);

  if (file) {
    return true;
  } else {
    return false;
  }
}

} // RusselNS
