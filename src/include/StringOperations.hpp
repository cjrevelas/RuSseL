#ifndef STRING_OPERATIONS_H
#define STRING_OPERATIONS_H

#include <string>
#include <sstream>
#include <deque>
#include <vector>

namespace RusselNS {

std::vector<std::string> GetVectorTokens(const std::string &inputString);
std::vector<std::string> GetVectorTokensDelimiter(const std::string &inputString, const char ch);
std::deque<std::string> GetDequeTokens(const std::string &inputString);
std::string EraseCharFromString(std::string string, const char ch);
char StringBack(const std::string &inputString);
std::string DequeToString(const std::deque<std::string> &tokens);
std::string GetFirstNonBlankCharacter(const std::string &inputString);
std::string ReverseString(const std::string &inputString);
std::string RemoveMultipleSpaces(const std::string &inputString);
std::string ReplaceSubstring(const std::string &inputString1, const std::string &inputString2, const std::string &inputString3);
std::string UpperToLower(const std::string &inputString);
std::string UpperToLowerFirst(const std::string &inputString);
int NumCharInstancesInString(const std::string &inputString, const char ch);

template<typename T>
T StringToNumber(const std::string &text) {
  std::stringstream ss(text);

  // store as double to import with exponential notation
  double aux;
  ss >> aux;

  return static_cast<T>(aux);
}

template<typename T>
std::string NumberToString(const T number) {
  std::stringstream ss;
  ss << number;

  return ss.str();
}

template<typename T>
bool IsNumber(const std::string &str) {
  T nn;
  return (std::istringstream(str) >> nn >> std::ws).eof();
}

template<typename T>
bool IsInDeque(const T &element, const std::deque<T> &deq) {
  if (std::find(deq.begin(), deq.end(), element)) {
    return true;
  } else {
    return false;
  }
}

template<typename T>
bool IsInDeque(const T &element, const std::vector<T> &vec) {
  if (std::find(vec.begin(), vec.end(), element)) {
    return true;
  } else {
    return false;
  }
}

} // russelNS

#endif // STRING_OPERATIONS_H
