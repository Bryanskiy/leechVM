#ifndef __INCLUDE_FRONTEND_FRONTEND_HH__
#define __INCLUDE_FRONTEND_FRONTEND_HH__

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <array>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "lLexer.hh"
#include "parser.hh"

namespace yy {

class Driver final {
public:
  ~Driver() = default;
  Driver(std::istream &in, std::ostream &out);

  parser::token_type yylex(parser::semantic_type *yylval,
                           parser::location_type *yylloc);
  bool parse();

  friend parser;

private:
  void reportSyntaxError(const parser::context &ctx);
  void reportExpctdTok(const yy::parser::context &ctx);
  void reportUnexpctdTok(const yy::parser::context &ctx);

  static inline constexpr size_t numTokens = 10;

private:
  std::unique_ptr<Lexer> lexer_ = nullptr;
};

} // namespace yy

#endif // __INCLUDE_FRONTEND_FRONTEND_HH__
