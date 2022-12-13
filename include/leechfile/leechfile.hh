#ifndef __INCLUDE_LEECHFILE_LEECHFILE_HH__
#define __INCLUDE_LEECHFILE_LEECHFILE_HH__

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "common/common.hh"
#include "leechobj/leechobj.hh"

namespace leech {

struct FuncMeta final : public ISerializable {
  FuncAddr addr;
  std::vector<std::shared_ptr<LeechObj>> cstPool{};
  std::vector<std::string> names;

  void serialize(std::ostream &ost) const override;
};

struct Meta final : public ISerializable {
  std::unordered_map<std::string, FuncMeta> funcs{};

  void serialize(std::ostream &ost) const override;
};

struct LeechFile final : public ISerializable {
  Meta meta{};
  std::vector<Instruction> code{};

  void serialize(std::ostream &ost) const override;
  void dump2LeechFormat(std::ostream &ost);
};

} // namespace leech

#endif // __INCLUDE_LEECHFILE_LEECHFILE_HH__
