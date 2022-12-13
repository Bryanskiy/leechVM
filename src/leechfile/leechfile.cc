#include "leechfile/leechfile.hh"

namespace leech {

/**
 * FuncMeta definitions
 */

void FuncMeta::serialize(std::ostream &ost) const {
  /* Write function address */
  serializeNum<std::uint64_t>(ost, addr);

  /* Write constant pool */
  auto cstNum = cstPool.size();
  serializeNum<std::uint64_t>(ost, cstNum);
  for (const auto &cst : cstPool)
    cst->serialize(ost);

  /* Write names */
  auto nameNum = names.size();
  serializeNum<std::uint64_t>(ost, nameNum);
  for (const auto &name : names)
    serializeString(ost, name);
}

/**
 * Meta definitions
 */

void Meta::serialize(std::ostream &ost) const {
  /* Write function number */
  auto funcNum = funcs.size();
  serializeNum<std::uint64_t>(ost, funcNum);

  /* Write functions meta */
  for (auto &&[name, fm] : funcs) {
    serializeString(ost, name);
    fm.serialize(ost);
  }
}

/**
 * LeechFile definitions
 */

void LeechFile::serialize(std::ostream &ost) const {
  /* Write magic */
  auto magic = reinterpret_cast<const std::uint64_t *>("theLEECH");
  serializeNum<uint64_t>(ost, *magic);

  /* Write meta */
  meta.serialize(ost);

  /* Write code */
  serializeNum<uint64_t>(ost, code.size());
  for (const auto &inst : code)
    inst.serialize(ost);
}

void LeechFile::dump2LeechFormat(std::ostream &ost) {
  constexpr char blockOffset[] = "    ";
  constexpr char dataOffset[] = "        ";
  for (const auto &[name, fmeta] : meta.funcs) {
    ost << ".func " << name << "(" << fmeta.addr << ")" << std::endl;
    ost << blockOffset << ".cpool" << std::endl;
    for (std::size_t i = 0; i < fmeta.cstPool.size(); ++i) {
      ost << dataOffset << i << ": ";
      fmeta.cstPool[i]->print();
      ost << std::endl;
    }
    ost << blockOffset << ".names" << std::endl;
    for (std::size_t i = 0; i < fmeta.names.size(); ++i) {
      ost << dataOffset << i << ": " << fmeta.names[i] << std::endl;
    }
  }
  ost << ".code" << std::endl;
  for (std::size_t i = 0; i < code.size(); ++i) {
    ost << blockOffset << i << " "
        << OpcodeConv::toName(code[i].getOpcode()).value() << " "
        << static_cast<int>(code[i].getArg()) << std::endl;
  }
}

} // namespace leech
