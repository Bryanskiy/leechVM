#ifndef __INCLUDE_LEECHOBJ_LEECHOBJ_HH__
#define __INCLUDE_LEECHOBJ_LEECHOBJ_HH__

#include <algorithm>
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "common/common.hh"
#include "common/opcodes.hh"

namespace leech {

class LeechObj : public ISerializable {
  std::size_t size_{};
  ValueType type_{};

public:
  LeechObj(std::size_t size, ValueType type) : size_(size), type_(type) {}

  void serialize(std::ostream &ost) const override final {
    serializeTypeNSize(ost);
    serializeVal(ost);
  }

  virtual void print() const = 0;

protected:
  void serializeTypeNSize(std::ostream &ost) const {
    serializeNum(ost, toUnderlying(type_));
    if (type_ != ValueType::None)
      serializeNum(ost, size_);
  }

  auto getSize() const { return size_; }

private:
  virtual void serializeVal(std::ostream &) const = 0;
};

class NoneObj final : public LeechObj {
public:
  NoneObj() : LeechObj(0, ValueType::None) {}

  void print() const override { std::cout << "None" << std::endl; }

private:
  void serializeVal(std::ostream &) const override {}
};

template <NumberLeech T> class NumberObj final : public LeechObj {
  T value_{};

public:
  explicit NumberObj(T value)
      : LeechObj(sizeof(T), typeToValueType<T>()), value_(value) {}

  void print() const override { std::cout << value_ << std::endl; }

private:
  void serializeVal(std::ostream &ost) const override {
    serializeNum(ost, value_);
  }
};

using IntObj = NumberObj<Integer>;
using FloatObj = NumberObj<Float>;

class StringObj final : public LeechObj {
  std::string string_;

public:
  explicit StringObj(std::string_view string)
      : LeechObj(string.size(), ValueType::String), string_(string) {}

  void print() const override {
    std::cout << '"' << string_ << '"' << std::endl;
  }

private:
  void serializeVal(std::ostream &ost) const override {
    for (auto sym : string_)
      serializeNum(ost, sym);
  }
};

using pLeechObj = std::unique_ptr<LeechObj>;
using Tuple = std::vector<pLeechObj>;
template <typename T>
concept ConvToLeechPtr = std::convertible_to<typename T::pointer, LeechObj *>;

class TupleObj final : public LeechObj {
  Tuple tuple_;

public:
  template <std::input_iterator It>
  TupleObj(It begin, It end) requires
      ConvToLeechPtr<typename std::iterator_traits<It>::value_type>
      : LeechObj(static_cast<std::size_t>(std::distance(begin, end)),
                 ValueType::Tuple),
        tuple_(getSize()) {
    std::move(begin, end, tuple_.begin());
  }

  template <Container Cont>
  explicit TupleObj(Cont &&cont) requires
      ConvToLeechPtr<typename Cont::value_type>
      : TupleObj(cont.begin(), cont.end()) {}

  void print() const override {
    std::cout << '(';
    for (auto &&elem : tuple_) {
      elem->print();
      std::cout << ',';
    }
    std::cout << ')' << std::endl;
  }

private:
  void serializeVal(std::ostream &ost) const override {
    for (auto &&ptr : tuple_)
      ptr->serialize(ost);
  }
};

} // namespace leech

#endif // __INCLUDE_LEECHOBJ_LEECHOBJ_HH__
