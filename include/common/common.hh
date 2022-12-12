#ifndef __INCLUDE_COMMON_COMMON_HH__
#define __INCLUDE_COMMON_COMMON_HH__

#include <iostream>
#include <type_traits>

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
concept Container = !std::is_same_v<T, std::string> && requires(T a) {
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
  {a.clear()};
};

template <Number T> void serializeNum(std::ostream &ost, T val) {
  ost.write(reinterpret_cast<char *>(&val), sizeof(val));
}

struct ISerializable {
  virtual void serialize(std::ostream &ost) const = 0;
  virtual std::size_t serializedSize() const = 0;
  virtual ~ISerializable() = default;
};

} // namespace leech

#endif // __INCLUDE_COMMON_COMMON_HH__
