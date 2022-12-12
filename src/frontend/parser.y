%language "c++"
%skeleton "lalr1.cc"

%define parse.trace
%define parse.lac full
%locations
%define api.value.type variant
%define parse.error custom

%param {Driver* driver}

%code requires {
    #include <string>
    #include <memory>
    namespace yy { class Driver; }
}

%code {
    #include "frontend/frontend.hh"
namespace yy
{ parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver); }
}

%token <std::string> IDENTIFIER
%token <int> INTEGER
%token FUNC_DECL              ".func"
       CPOLL_DECL             ".cpool"
       NAMES_DECL             ".names"
       CODE_DECL              ".code"
       LABEL                  ".label"
       COLON                  ":"
       LRB                    "("
       RRB                    ")"
       COMMA                  ","
%%

program:            funcList                                  {};

funcList:           funcList func                             {};
                  | func                                      {};

func:               FUNC_DECL IDENTIFIER
                        cpollBlock namesBlock codeBlock       {};
cpollBlock:         CPOLL_DECL constants                      {};
                  | /* empty */                               {};

constants:          constants leechObjEntry                   {};
                  | leechObjEntry                             {};

leechObjEntry:      INTEGER COLON leechObj                    {};

leechObj:           primitiveTy                               {};
                  | array                                     {};

primitiveTy:        IDENTIFIER                                {};
                  | INTEGER                                   {};

array:              LRB arrayArgs RRB                         {};
arrayArgs:          arrayArgs COMMA primitiveTy               {};
                  | primitiveTy                               {};

namesBlock:         NAMES_DECL names                          {};
                  | /* empty */                               {};

names:              names nameEntry                           {};
                  | nameEntry                                 {};

nameEntry:          INTEGER COLON IDENTIFIER                  {};

codeBlock:          CODE_DECL code                            {};
                  | /* empty */                               {};

code:               code codeEntry                            {};
                  | codeEntry                                 {};

codeEntry:          LABEL IDENTIFIER                          {};
                  | instruction

instruction:        IDENTIFIER                                {};
                  | IDENTIFIER INTEGER                        {};
                  | IDENTIFIER COLON IDENTIFIER               {};
%%

namespace yy {
  void parser::error (const parser::location_type& location, const std::string& string)
  {
    std::cerr << string << " in (line.column): "<< location << std::endl;
  }

  parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver)
  {
    return driver->yylex(yylval, yylloc);
  }

  void parser::report_syntax_error(parser::context const& ctx) const
  {
    driver->reportSyntaxError(ctx);
  }
}
