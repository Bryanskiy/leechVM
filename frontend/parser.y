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

%%
program:  line program      {}
        | /* empty */ 	    {}

line:     OPCODE            {}
        | OPCODE I32        {}
        | OPCODE I32 I32    {}
%%

namespace yy {
    parser::token_type yylex (parser::semantic_type* yylval, Driver* driver) {
		return driver->yylex(yylval);
	}

    void parser::error (const std::string& msg) {
        std::cout << msg << std::endl;
	}

	void parser::report_syntax_error(parser::context const& ctx) const {
	}
}