#include <cassert>
#include <cstdint>

#include "driver.hh"
#include "../thirdparty/cpython/Include/opcode.h"

namespace yy {

Driver::Driver(std::istream& in, std::ostream& out) : Driver() {
    lexer = std::make_unique<yyFlexLexer>(in, out);
}

bool Driver::parse() {
    parser parser(this);
    bool res = parser.parse();
    return !res;
}

yy::parser::token_type Driver::yylex(yy::parser::semantic_type* yylval) {
    parser::token_type token = static_cast<parser::token_type>(lexer->yylex());
    if(token == yy::parser::token_type::OPCODE) {
        std::string opcode(lexer->YYText());
        yylval->as<int>() = opcodes[opcode];
    } else if (token == yy::parser::token_type::I32) {
        yylval->as<int>() = std::atoi(lexer->YYText());
    } else if (token == yy::parser::token_type::STR) {
        std::string str(lexer->YYText());
        parser::semantic_type tmp;
        tmp.as<std::string>() = str;
        yylval->swap<std::string>(tmp);
    }
    return token;
}

Driver::Driver() {
    opcodes = {
        {"POP_TOP", POP_TOP},
        {"UNARY_POSITIVE", UNARY_POSITIVE},
        {"UNARY_NEGATIVE", UNARY_NEGATIVE},
        {"UNARY_NOT", UNARY_NOT},
        {"UNARY_INVERT", UNARY_INVERT},
        {"BINARY_MULTIPLY", BINARY_MULTIPLY},
        {"BINARY_ADD", BINARY_ADD},
        {"STORE_NAME", STORE_NAME},
        {"LOAD_NAME", LOAD_NAME},
        {"STORE_FAST", STORE_FAST},
        {"LOAD_FAST", LOAD_FAST},
        {"LOAD_CONST", LOAD_CONST},
        {"COMPARE_OP", COMPARE_OP},
        {"POP_JUMP_IF_TRUE", POP_JUMP_IF_TRUE},
        {"POP_JUMP_IF_FALSE", POP_JUMP_IF_FALSE},
        {"RETURN_VALUE", RETURN_VALUE},
        {"LOAD_GLOBAL", LOAD_GLOBAL},
        {"BINARY_SUBTRACT", BINARY_SUBTRACT},
        {"CALL_FUNCTION", CALL_FUNCTION},
    };
}

} // namespace yy