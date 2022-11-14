%option c++ 
/* makes the scanner not call yywrap() upon an end-of-file */
%option noyywrap
%{
#include <iostream>
#include "parser.tab.hh"
%}

WS [ \t\n\v]+
DIGIT          [0-9]
POSITIVE_DIGIT [1-9]
INT            {POSITIVE_DIGIT}+{DIGIT}*

WORD           [a-zA-Z]
CAPITAL        [A-Z]

%%
"Constants"            { return yy::parser::token_type::CONSTANTS; }
"Variables"            { return yy::parser::token_type::VARIABLES; }
"Names"                { return yy::parser::token_type::NAMES; }
"Bytecode"             { return yy::parser::token_type::BYTECODE; }
":"                    { return yy::parser::token_type::COLON; }
{CAPITAL}_{CAPITAL}+   { return yy::parser::token_type::OPCODE; }
INT                    { return yy::parser::token_type::I32; }
WORD                   { return yy::parser::token_type::STR; }
WS                     {}
%%
