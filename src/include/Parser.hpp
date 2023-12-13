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

  bool GetCoeffs(const std::string &stringCoeffs);

  virtual void ProcessCoeffs(std::deque<std::string> &) {}

  static int lineOfInput_;
  static bool verbose_;
  static std::shared_ptr<Russel> russel_;
};

void ParseInput(const std::string &filename, std::shared_ptr<class Russel> &russel);

void CheckDuplicateFlags(const std::vector<std::unique_ptr<Parser>> &listOfFlags);

std::string CheckForExpressions(const std::string &stringCoeffs);

class ParserInteract : public Parser { public: ParserInteract (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserInteract() override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserPrint    : public Parser { public: ParserPrint    (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserPrint()    override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserVariable : public Parser { public: ParserVariable (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserVariable() override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserMesh     : public Parser { public: ParserMesh     (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserMesh()     override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserEos      : public Parser { public: ParserEos      (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserEos()      override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserContour  : public Parser { public: ParserContour  (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserContour()  override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };
class ParserChain    : public Parser { public: ParserChain    (const std::string &flag, const int &coeffsMinLength) : Parser(flag, coeffsMinLength) {} virtual ~ParserChain()    override {} private: virtual void ProcessCoeffs(std::deque<std::string> &deqCoeffs) override; };

} // RusselNS

#endif // PARSER_HPP
