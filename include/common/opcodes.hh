#ifndef __INCLUDE_COMMON_OPCODES_HH__
#define __INCLUDE_COMMON_OPCODES_HH__

#include <concepts>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace leech {
enum class Opcodes {
  UNKNOWN,
#define LEECH_MAKE_OPCODE(opc) opc,

#include "opcodes.in"

#undef LEECH_MAKE_OPCODE
};

template <class T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

class OpcodeConv final {
private:
  static const auto &getStrToOpcodeMap() {
    static std::unordered_map<std::string_view, Opcodes> toOpcodeMap{
#define LEECH_MAKE_OPCODE(opc) {#opc, Opcodes::opc},

#include "opcodes.in"

#undef LEECH_MAKE_OPCODE
    };
    return toOpcodeMap;
  }

  static const auto &getOpcodeToStrMap() {
    static std::unordered_map<Opcodes, std::string_view> toStrMap{
#define LEECH_MAKE_OPCODE(opc) {Opcodes::opc, #opc},

#include "opcodes.in"

#undef LEECH_MAKE_OPCODE
    };
    return toStrMap;
  }

  template <typename Map, typename Key>
  static auto getOptFromMap(const Map &map, const Key &key)
      -> std::optional<typename Map::mapped_type> {
    if (auto found = map.find(key); found != map.end())
      return found->second;
    return std::nullopt;
  }

public:
  OpcodeConv() = delete;

  template <StringLike T>
  static std::optional<Opcodes> fromName(const T &name) {
    const auto &toOpcodeMap = getStrToOpcodeMap();
    return getOptFromMap(toOpcodeMap, name);
  }

  static std::optional<std::string_view> toName(Opcodes opcode) {
    const auto &toStrMap = getOpcodeToStrMap();
    return getOptFromMap(toStrMap, opcode);
  }
};

} // namespace leech

#endif /* __INCLUDE_COMMON_OPCODES_HH__ */