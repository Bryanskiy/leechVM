#include <cassert>
#include <cstdint>

#include "driver.hh"
#include "../thirdparty/cpython/Include/opcode.h"

static uint32_t getOpcode(const std::string& str);

namespace yy {

Driver::Driver(std::istream& in, std::ostream& out) {
    lexer = std::make_unique<yyFlexLexer>(in, out);
}

bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    assert(res == false);
    return !res;
}

yy::parser::token_type Driver::yylex(yy::parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(lexer->yylex());
    if(token == yy::parser::token_type::OPCODE) {
        std::string opcode(lexer->YYText());
        yylval->as<int>() = getOpcode(opcode);
    }
    return token;
}

} // namespace yy

static uint32_t getOpcode(const std::string& str) {
    if (str == "POP_TOP") {
        return POP_TOP;
    }
}