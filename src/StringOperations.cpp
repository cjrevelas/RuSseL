#include <StringOperations.hpp>
#include <algorithm>

namespace RusselNS {

std::vector<std::string> GetVectorTokens(const std::string &inputString){
  std::string buf;
  std::stringstream ss(inputString);
  std::vector<std::string> tokens;

  while (ss >> buf) {
    tokens.push_back(buf);
  }

  return tokens;
}

std::vector<std::string> GetVectorTokensDelimiter(const std::string &inputString, const char ch) {
  std::string buf;
  std::stringstream ss(inputString);

  std::vector<std::string> tokens;

  while (std::getline(ss, buf, ch)) {
    tokens.push_back(buf);
  }

  return tokens;
}

std::deque<std::string> GetDequeTokens(const std::string &inputString) {
  std::string buf;
  std::stringstream ss(inputString);
  std::deque<std::string> tokens;

  while (ss >> buf) {
    tokens.push_back(buf);
  }

  return tokens;
}

std::string EraseCharFromString(std::string string, const char ch) {
  string.erase(std::remove(string.begin(), string.end(), ch), string.end());

  return string;
}

int NumCharInstancesInString(const std::string &inputString, const char ch) {
  return std::count(inputString.begin(), inputString.end(), ch);
}

char StringBack(const std::string &inputString) {
  return (*(inputString.rbegin()));
}

std::string DequeToString(const std::deque<std::string> &tokens) {
  std::string outputString = "";

  for (int ii=0; ii<tokens.size(); ++ii) {
    if (ii > 0) {
      outputString += " ";
    }

    outputString += tokens[ii];
  }

  return outputString;
}

std::string GetFirstNonBlankCharacter(const std::string &inputString) {
  if (inputString.empty()) {
    return inputString;
  } else {
    std::vector<std::string> tokens = GetVectorTokens(inputString);
    return tokens[0].substr(0,1);
  }
}

std::string ReverseString(const std::string &inputString) {
  return std::string(inputString.rbegin(), inputString.rend());
}

std::string ReplaceSubstring(const std::string &inputString1, const std::string &inputString2, const std::string &inputString3) {
  std::string auxString = inputString1;
  std::string::size_type nn = 0;

  while ((nn = auxString.find(inputString2,nn)) != std::string::npos) {
    auxString.replace(nn, inputString2.size(), inputString3);
    nn += inputString3.size();
  }

  return auxString;
}

std::string RemoveMultipleSpaces(const std::string &inputString) {
  std::vector<std::string> tokens = GetVectorTokens(inputString);
  std::string outputString = "";

  for (int ii=0; ii<tokens.size(); ++ii) {
    if (ii>0) {
      outputString += " ";
    }

    outputString += tokens[ii];
  }

  return outputString;
}

std::string UpperToLower(const std::string &inputString) {
  std::string lower = "";

  for (int ii=0; ii<inputString.size(); ++ii) {
    char ch = inputString[ii];
    if (ch>=65 && ch<=92) {
      ch += 32;
    }

    lower += ch;
  }

  return lower;
}

std::string UpperToLowerFirst(const std::string &inputString) {
  std::string outputString = inputString;

  char ch = outputString[0];

  if (ch>=65 && ch<=92) {
    outputString[0] = ch+32;
  }

  return outputString;
}

} // russelNS
