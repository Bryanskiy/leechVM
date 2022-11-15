%language "c++"
%skeleton "lalr1.cc"

%define api.value.type variant
%define parse.error custom
%param {Driver* driver}

%code requires {
    namespace yy {class Driver;}
}

%code {
    #include <cstdint>
    #include <string>
    #include "../driver/driver.hh"
    namespace yy {parser::token_type yylex(parser::semantic_type* yylval, Driver* driver);}
}

%token <int32_t>      I32
%token <uint32_t>     OPCODE
%token <std::string>  STR

%token                CONSTANTS
                      NAMES
                      VARNAMES
                      BYTECODE
                      COLON
%%
program:    section program           {}
          | /* empty */ 	          {}

section:    bytecodes                 {}
          | names                     {}
          | constants                 {}
          | varnames                  {}

bytecodes:  BYTECODE COLON opcodes    {}

opcodes:   OPCODE I32 opcodes         { driver->insertCode($1, $2); }
         | OPCODE opcodes             { driver->insertCode($1); }
         | /* empty */ 	              {}

names:     NAMES COLON strings        {}
strings:   I32 COLON STR strings      { driver->insertName($1, $3); }
         | /* empty */ 	              {}

varnames:  VARNAMES  COLON strings    {}

constants: CONSTANTS COLON numbers    {}
numbers:   I32 COLON I32 numbers      { driver->insertConsts($1, $3); }
         | /* empty */ 	              {}


%%

namespace yy {
    parser::token_type yylex (parser::semantic_type* yylval, Driver* driver) {
		return driver->yylex(yylval);
	}

    void parser::error (const std::string& msg) {
        // std::cout << msg << std::endl;
	}

	void parser::report_syntax_error(parser::context const& ctx) const {
	}
}