#pragma once

#include <memory>
#include <unordered_map>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

#include "../frontend/parser.tab.hh"
#include "../objFile/objFile.hh"

namespace yy {

class Driver {
public:
    Driver();
    Driver(const Driver&) = delete;
    Driver(Driver&&) = delete;
    
    Driver& operator=(const Driver&) = delete;
    Driver& operator=(Driver&&) = delete;

    Driver(std::istream& in, std::ostream& out);

    bool parse();
    yy::parser::token_type yylex(yy::parser::semantic_type* yylval);

    void insertCode(leech::Opcode opcode, leech::Operand operand = 0) { objFile.coCode.push_back({ opcode, operand }); }
    void insertName(leech::CoIndex idx, const std::string& name) { objFile.coNames[idx] = name; }
    void insertVarname(leech::CoIndex idx, const std::string& name) { objFile.coVarnames[idx] = name; }
    void insertConsts(leech::CoIndex idx, int32_t val) { objFile.coConsts[idx] = val; }

    void dumpObjFile(std::ostream& out) { objFile.dump(out); }
private:
    std::unique_ptr<yyFlexLexer> lexer = nullptr;
    std::unordered_map<std::string, uint32_t> opcodes;
    leech::ObjFile objFile;
};
} // namespace yy