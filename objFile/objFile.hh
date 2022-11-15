#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

namespace leech {

using Opcode = uint16_t;
using Operand = uint8_t;
using CoIndex = uint16_t;

struct Instruction {
    Opcode opcode;
    Operand operand;
};

struct ObjFile final {
public:
    std::unordered_map<CoIndex, std::string> coNames;
    std::unordered_map<CoIndex, std::string> coVarnames;
    std::unordered_map<CoIndex, int32_t> coConsts;
    std::vector<Instruction> coCode;

    void dump(std::ostream& out);
};

} // namespace leech