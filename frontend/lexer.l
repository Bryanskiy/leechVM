%option c++ 
/* makes the scanner not call yywrap() upon an end-of-file */
%option noyywrap
%{
#include <iostream>
#include "../thirdparty/cpython/Include/opcode.h"
#include "parser.tab.hh"
%}

WS [ \t\n\v]+
DIGIT          [0-9]
POSITIVE_DIGIT [1-9]
INT            {POSITIVE_DIGIT}+{DIGIT}*

%%
"POP_TOP"       {
                    std::cout << "Hello world!" << std::endl; 
                    return yy::parser::token_type::OPCODE;
                }
%%
