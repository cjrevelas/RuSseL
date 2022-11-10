#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <deque>
#include <memory>

#include "Russel.hpp"

namespace RusselNS {

class Parser {
 protected:
  std::string flag_;
  std::string coeffs_;
  int flagLength_;
  int coeffsMinLength_;

 public:
  Parser(const std::string &flag, const int &coeffsMinLength);

  virtual ~Parser() {}

  std::string GetFlag() const { return flag_;}

  bool GetCoeffs(const std::string &stringCoeffs) const;

  virtual void ProcessCoeffs(const std::deque<std::string> &coeffs);

  static int lineOfInput_;
  static bool verbose_;
  static std::shared_ptr<Russel> russel_;
};

void ParseInput(const std::string &filename, std::shared_ptr<Russel> &russel); // we pass a smart pointer by reference here

void CheckDuplicateFlags(const std::vector<std::unique_ptr<Parser>> &listOfFlags); // we pass a vector of smart pointers by reference here

std::string CheckForExpressions(const std::string &stringCoeffs);

} // RusselNS

#endif // PARSER_HPP
