#include "objFile.hh"

namespace leech {

void ObjFile::dump(std::ostream& out) {
    if (!coNames.empty()) {
        out << "Names:" << std::endl;
        for(const auto &[key, value]: coNames) {
            out << "    " << key << ": " << value << std::endl;
        }
    }
    if (!coVarnames.empty()) {
        out << "Varnames:" << std::endl;
        for(const auto &[key, value]: coVarnames) {
            out << "    " << key << ": " << value << std::endl;
        }
    }
    if (!coConsts.empty()) {
        out << "Constants:" << std::endl;
        for(const auto &[key, value]: coConsts) {
            out << "    " << key << ": " << value << std::endl;
        }
    }
    std::cout << "Bytecode:" << std::endl;
    for (auto&& instr: coCode) {
         out << "    " << instr.opcode << " " << instr.operand << std::endl;
    }
}

} // namespace leech