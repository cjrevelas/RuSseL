#ifndef IO_HELPER
#define IO_HELPER

#include <iostream>
#include <vector>
#include <deque>
#include <iomanip>
#include <map>

namespace RusselNS {

std::vector<std::string> ParseUntilDash(const std::vector<std::string> &vecCoeffs, std::vector<std::string>::iterator &it);
std::deque<std::string> ParseUntiDash(const std::deque<std::string> &deqCoeffs, std::deque<std::string>::iterator &it);

void PrintMessage(const std::string &message, const int indentLevel);
void PrintWarning(const std::string &message);
void PrintWarning(const std::string &caller, const std::string &message);
void ExitProgram(const std::string &message);
void ExitProgram(const std::string &caller, const std::string &message);
bool FileExists(const std::string &filename);

template <typename V>
void PrintVariable(const std::string &description, const V &variable, const std::string &units, const int indentLevel) {
  int widthLevel       = indentLevel * 3;
  int widthDescription = 43 - widthLevel;
  int widthVariable    = 15;

  std::cout << std::left;
  std::cout << std::setw(3)           << "#:";
  std::cout << std::left;
  std::cout << std::setw(widthLevel) << "";
  std::cout << std::left;
  std::cout << std::setw(widthDescription) << description;
  std::cout << std::right;
  std::cout << std::setw(widthVariable) << variable;
  std::cout << std::left;
  std::cout << ' ';
  std::cout << units;
  std::cout << '\n';
  std::cout << std::right;
}

struct convert {
  static std::map<char,int> CreateMapXYZ2Int() {
    std::map<char,int> map;
    map['x'] = 0;
    map['y'] = 1;
    map['z'] = 2;

    return map;
  }

  static std::map<int,char> CreateMapInt2XYZ() {
    std::map<int,char> map;
    map[0] = 'x';
    map[1] = 'y';
    map[2] = 'z';

    return map;
  }

  static std::map<std::string,bool> CreateMapYesNo2Bool() {
    std::map<std::string,bool> map;
    map["yes"] = true;
    map["no"]  = false;

    return map;
  }

  static std::map<bool,std::string> CreateMapBool2YesNo() {
    std::map<bool,std::string> map;
    map[true]  = "yes";
    map[false] = "no";

    return map;
  }

  static std::map<int,char> int2xyz;
  static std::map<char,int> xyz2int;
  static std::map<std::string,bool> YesNo2Bool();
  static std::map<bool,std::string> bool2YesNo();
};

} // RusselNS

#endif
