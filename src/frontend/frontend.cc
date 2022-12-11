#include "frontend/frontend.hh"

namespace yy {

Driver::Driver(std::istream &in, std::ostream &out) {
  m_lexer = std::make_unique<yyFlexLexer>(in, out);
}

bool Driver::parse() {
  parser parser(this);
  bool res = parser.parse();
  return !res;
}

parser::token_type Driver::yylex(parser::semantic_type *yylval) {
  parser::token_type token = static_cast<parser::token_type>(m_lexer->yylex());
  if (token == yy::parser::token_type::IDENTIFIER) {
    std::string name(m_lexer->YYText());
    parser::semantic_type tmp;
    tmp.as<std::string>() = name;
    yylval->swap<std::string>(tmp);
  } else if (token == yy::parser::token_type::INTEGER) {
    yylval->as<int>() = std::atoi(m_lexer->YYText());
  }

  return token;
}

} // namespace yy
