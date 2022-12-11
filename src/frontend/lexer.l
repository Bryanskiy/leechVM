%option c++
%option noyywrap debug
%{
#include "parser.hh"
%}

WS [ \t\n\v]+
INTEGER [0]|[1-9][0-9]*
NAME [_]*[a-zA-Z][a-zA-Z0-9_]*

%%
{WS}
{INTEGER}   return yy::parser::token_type::INTEGER;
{NAME}      return yy::parser::token_type::IDENTIFIER;
%%
