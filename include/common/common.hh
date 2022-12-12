#ifndef __INCLUDE_COMMON_COMMON_HH__
#define __INCLUDE_COMMON_COMMON_HH__

#include <iostream>
#include <type_traits>

#include "opcodes.hh"

namespace leech {

enum class ValueType : std::uint8_t { Unknown, Integer, Float, String, Tuple };

template <typename T> constexpr auto toUnderlying(T enumVal) {
  return static_cast<std::underlying_type_t<T>>(enumVal);
}

using Integer = std::int64_t;
using Float = double;

template <typename T>
concept NumberLeech = std::is_same_v<T, Integer> || std::is_same_v<T, Float>;

template <typename T>
concept Number = NumberLeech<T> || std::is_integral_v<T>;

template <NumberLeech T> inline consteval ValueType typeToValueType() {
  if constexpr (std::is_same_v<T, Integer>)
    return ValueType::Integer;
  return ValueType::Float;
}
template <typename T>
concept Container = !
std::is_same_v<T, std::string>
    &&requires(T a) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
        typename T::size_type;
        { a.begin() } -> std::convertible_to<typename T::iterator>;
        { a.end() } -> std::convertible_to<typename T::iterator>;
        { a.cbegin() } -> std::convertible_to<typename T::const_iterator>;
        { a.cend() } -> std::convertible_to<typename T::const_iterator>;
        { a.clear() };
      };

template <Number T> void serializeNum(std::ostream &ost, T val) {
  ost.write(reinterpret_cast<char *>(&val), sizeof(val));
}

inline void serializeString(std::ostream &ost, std::string_view sv) {
  auto svLen = sv.size();
  serializeNum(ost, svLen);
  if (svLen > 0)
    ost.write(sv.data(), static_cast<std::streamsize>(
                             svLen * sizeof(std::string_view::value_type)));
}

struct ISerializable {
  virtual void serialize(std::ostream &ost) const = 0;
  virtual ~ISerializable() = default;
};

using ArgType = std::uint8_t;

constexpr std::size_t kArgSize = sizeof(ArgType);
constexpr std::size_t kInstSize =
    sizeof(std::underlying_type_t<Opcodes>) + kArgSize;

class Instruction final : public ISerializable {
  Opcodes opcode_{};
  ArgType arg_{};

public:
  Instruction(Opcodes opcode, ArgType arg = 0) : opcode_(opcode), arg_(arg) {
    if (opcode_ == Opcodes::UNKNOWN)
      throw std::invalid_argument(
          "Trying to create Instruction with UNKNOWN opcode");
  }

  Instruction(std::underlying_type_t<Opcodes> opcode, ArgType arg = 0)
      : Instruction(static_cast<Opcodes>(opcode), arg) {}

  void serialize(std::ostream &ost) const override {
    serializeNum(ost, toUnderlying(opcode_));
    serializeNum(ost, arg_);
  }

  auto getOpcode() const { return opcode_; }
  auto getArg() const { return arg_; }
};

} // namespace leech

#endif // __INCLUDE_COMMON_COMMON_HH__
