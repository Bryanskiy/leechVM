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

} // namespace leech
