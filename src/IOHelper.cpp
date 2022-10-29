#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include "IOHelper.hpp"
#include "StringOperations.hpp"


namespace RusselNS {

std::map<char,int> convert::xyz2int = convert::CreateMapXYZ2Int();
std::map<int,char> convert::int2xyz = convert::CreateMapInt2XYZ();

std::map<std::string,bool> convertYesNo2Bool = convert::CreateMapYesNo2Bool();
std::map<bool,std::string> convertBool2YesNo = convert::CreateMapBool2YesNo();

std::vector<std::string> ParseUntilDash(const std::vector<std::string> &vecCoeffs, std::vector<std::string>::iterator &it) {
  std::vector<std::string> coeffList;

  while (it != vecCoeffs.end()) {
    std::string arg = (*it);

    if (IsNumber<double>(arg) && arg[0] == '-') {
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

    if (IsNumber<double>(arg) && arg[0] == '-') {
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
  std::cout << std::setw(3          ) << "#:";
  std::cout << std::left;
  std::cout << std::setw(widthLevel) << "";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  std::cout << std::right;
}


void PrintWarning(const std::string &message) {
  std::cout << std::left;
  std::cout << std::setw(3 ) << "#:";
  std::cout << std::left;
  std::cout << std::setw(9) << "";
  std::cout << std::left;
  std::cout << message;
  std::cout << '\n';
  std::cout << std::right;
}


void PrintWarning(const std::string &caller, const std::string &message) {
  std::cout << std::left;
  std::cout << std::setw(3 ) << "#:";
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
