#include "string_helper.hpp"

std::vector<std::string> tokenize(const std::string &input_string){
  std::string buf;
  std::stringstream ss(input_string);
  std::vector<std::string> tokens;

  while(ss >> buf) { tokens.push_back(buf); }

  return tokens;
}
