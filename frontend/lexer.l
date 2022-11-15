%option c++ 
/* makes the scanner not call yywrap() upon an end-of-file */
%option noyywrap
%{
#include <iostream>
#include "parser.tab.hh"
%}

WS             [ \t\n\v]+
INT            [0]|[1-9][0-9]*
NAME           [a-z][a-z0-9]*
OPCODE         [A-Z]+[_A-Z]+

%%
{WS}                   {}
"Constants"            { return yy::parser::token_type::CONSTANTS; }
"Varnames"             { return yy::parser::token_type::VARNAMES; }
"Names"                { return yy::parser::token_type::NAMES; }
"Bytecode"             { return yy::parser::token_type::BYTECODE; }
":"                    { return yy::parser::token_type::COLON; }
{OPCODE}               { return yy::parser::token_type::OPCODE; }
{INT}                  { return yy::parser::token_type::I32; }
{NAME}                 { return yy::parser::token_type::STR; }
%%
