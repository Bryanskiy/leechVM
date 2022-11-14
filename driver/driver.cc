#include "driver.hh"

namespace yy {

Driver::Driver(std::istream& in, std::ostream& out) {
    lexer = std::make_unique<yyFlexLexer>(in, out);
}

bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

yy::parser::token_type Driver::yylex(yy::parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(lexer->yylex());
    return token;
}

} // namespace yy