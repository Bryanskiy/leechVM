#include "frontend/frontend.hh"

namespace yy {

Driver::Driver(std::istream &in, std::ostream &out)
    : lexer_{std::make_unique<Lexer>(in, out)} {}

bool Driver::parse() {
  parser parser(this);
  bool res = parser.parse();
  return !res;
}

parser::token_type Driver::yylex(parser::semantic_type *yylval,
                                 parser::location_type *yylloc) {
  parser::token_type token = static_cast<parser::token_type>(lexer_->yylex());
  if (token == yy::parser::token_type::IDENTIFIER) {
    std::string name(lexer_->YYText());
    parser::semantic_type tmp;
    tmp.as<std::string>() = name;
    yylval->swap<std::string>(tmp);
  } else if (token == yy::parser::token_type::INTEGER) {
    yylval->as<int>() = std::atoi(lexer_->YYText());
  }

  *yylloc = lexer_->getCurLocation();
  return token;
}

void Driver::reportSyntaxError(const parser::context &ctx) {
  auto loc = ctx.location();

  std::cerr << "syntax error in ";
  std::cerr << "line: " << loc.begin.line;
  std::cerr << ", column: " << loc.begin.column << std::endl;

  reportExpctdTok(ctx);
  reportUnexpctdTok(ctx);
}

void Driver::reportExpctdTok(const yy::parser::context &ctx) {
  // Report the tokens expected at this point.

  std::array<parser::symbol_kind_type, numTokens> expectdTokns;
  size_t numOfExpectdTokns = static_cast<size_t>(
      ctx.expected_tokens(expectdTokns.data(), expectdTokns.size()));

  std::cerr << "expected:";

  for (size_t i = 0; i < numOfExpectdTokns; ++i) {
    if (i != 0)
      std::cerr << " or ";

    std::cerr << " <" << parser::symbol_name(expectdTokns[i]) << "> ";
  }

  std::cerr << std::endl;
}

void Driver::reportUnexpctdTok(const yy::parser::context &ctx) {
  // Report the unexpected token.
  auto lookahead = ctx.token();

  std::cerr << "before: "
            << "<" << parser::symbol_name(lookahead) << ">" << std::endl;
}

} // namespace yy
