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

%%
{WORD}+_{WORD}+  { return yy::parser::token_type::OPCODE; }
INT              { return yy::parser::token_type::I32; }
%%
