#ifndef __INCLUDE_DRIVER_HH__
#define __INCLUDE_DRIVER_HH__

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "parser.hh"

namespace yy {

class Driver final {
public:
  ~Driver() = default;
  Driver(std::istream &in, std::ostream &out);

  parser::token_type yylex(parser::semantic_type *yylval);
  bool parse();
  void codegen();

  friend parser;

private:
  std::unique_ptr<yyFlexLexer> m_lexer = nullptr;
};

} // namespace yy

#endif /* __INCLUDE_DRIVER_HH__ */
