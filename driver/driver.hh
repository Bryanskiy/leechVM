#pragma once

#include <memory>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "../frontend/parser.tab.hh"

namespace yy {

class Driver {
public:
    Driver() = default;
    Driver(const Driver&) = delete;
    Driver(Driver&&) = delete;
    
    Driver& operator=(const Driver&) = delete;
    Driver& operator=(Driver&&) = delete;

    Driver(std::istream& in, std::ostream& out);

    bool parse();
    yy::parser::token_type yylex(yy::parser::semantic_type* yylval);
private:
    std::unique_ptr<yyFlexLexer> lexer = nullptr;
};
} // namespace yy