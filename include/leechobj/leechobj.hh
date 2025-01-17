#ifndef __INCLUDE_LEECHOBJ_LEECHOBJ_HH__
#define __INCLUDE_LEECHOBJ_LEECHOBJ_HH__

#include <algorithm>
#include <concepts>
#include <istream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "common/common.hh"
#include "common/opcodes.hh"

namespace leech {

class LeechObj;
using pLeechObj = std::shared_ptr<LeechObj>;

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

  virtual pLeechObj clone() const = 0;
  [[noreturn]] virtual bool compare(LeechObj *obj, CmpOp op) const;
  [[noreturn]] virtual pLeechObj add(LeechObj *obj) const;
  [[noreturn]] virtual pLeechObj div(LeechObj *obj) const;
  [[noreturn]] virtual pLeechObj subscript(LeechObj *obj) const;

  auto getType() const { return type_; }

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

inline pLeechObj deserializeObj(std::istream &ist);

class NoneObj final : public LeechObj {
public:
  NoneObj() : LeechObj(0, ValueType::None) {}

  void print() const override { std::cout << "None" << std::endl; }

  pLeechObj clone() const override { return std::make_unique<NoneObj>(); }

private:
  void serializeVal(std::ostream &) const override {}
};

template <NumberLeech T> class NumberObj final : public LeechObj {
  T value_{};

public:
  explicit NumberObj(T value)
      : LeechObj(sizeof(T), typeToValueType<T>()), value_(value) {}

  void print() const override { std::cout << value_; }

  static pLeechObj deserialize(std::istream &ist) {
    deserializeNum<uint64_t>(ist);
    auto val = deserializeNum<T>(ist);
    return std::make_unique<NumberObj>(val);
  }

  pLeechObj clone() const override {
    return std::make_unique<NumberObj>(value_);
  }

  pLeechObj div(LeechObj *obj) const override {
    if (obj->getType() != getType())
      throw std::invalid_argument("Can't divide");

    auto *pObj = dynamic_cast<NumberObj<T> *>(obj);
    if (pObj == nullptr)
      throw std::runtime_error("Dynamic cast failed");
    return std::make_shared<NumberObj<Float>>(static_cast<Float>(value_) /
                                              static_cast<Float>(pObj->value_));
  }

  bool compare(LeechObj *obj, CmpOp op) const override {
    if (obj->getType() != getType() || getType() != ValueType::Integer)
      throw std::invalid_argument("Can't compare");

    auto *pObj = dynamic_cast<NumberObj<T> *>(obj);

    if (pObj == nullptr)
      throw std::runtime_error("Dynamic cast failed");

    switch (op) {
    case CmpOp::LE:
      return value_ < pObj->value_;
    case CmpOp::LEQ:
      return value_ <= pObj->value_;
    case CmpOp::EQ:
      return value_ == pObj->value_;
    case CmpOp::NEQ:
      return value_ != pObj->value_;
    case CmpOp::GR:
      return value_ > pObj->value_;
    case CmpOp::GREQ:
      return value_ >= pObj->value_;

    default:
      throw std::runtime_error("Uknown cmp op type");
    }
  }

  pLeechObj add(LeechObj *obj) const override {
    auto pobj = dynamic_cast<NumberObj *>(obj);
    if (nullptr == pobj)
      throw std::runtime_error("Dynamic cast failed");
    return std::make_unique<NumberObj>(pobj->value_ + value_);
  }

  auto getVal() const { return value_; }

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

  void print() const override { std::cout << '"' << string_ << '"'; }

  pLeechObj clone() const override {
    return std::make_unique<StringObj>(string_);
  }

  static pLeechObj deserialize(std::istream &ist) {
    auto strlen = deserializeNum<uint64_t>(ist);
    auto str = deserializeString(ist, strlen);
    return std::make_unique<StringObj>(str);
  }

private:
  void serializeVal(std::ostream &ost) const override {
    for (auto sym : string_)
      serializeNum(ost, sym);
  }
};

using Tuple = std::vector<pLeechObj>;

class TupleObj final : public LeechObj {
  Tuple tuple_;

public:
  template <std::input_iterator It>
  TupleObj(It begin, It end)
      : LeechObj(static_cast<std::size_t>(std::distance(begin, end)),
                 ValueType::Tuple),
        tuple_(getSize()) {
    std::move(begin, end, tuple_.begin());
  }

  template <Container Cont>
  explicit TupleObj(Cont &&cont) : TupleObj(cont.begin(), cont.end()) {}

  void print() const override {
    std::cout << '(';
    for (auto &&elem : tuple_) {
      elem->print();
      std::cout << ',';
    }
    std::cout << ')';
  }

  pLeechObj subscript(LeechObj *pobj) const override {
    if (pobj->getType() != ValueType::Integer)
      throw std::invalid_argument("Incorrect subscription index");

    auto *numObj = dynamic_cast<IntObj *>(pobj);

    return tuple_.at(static_cast<std::size_t>(numObj->getVal()));
  }

  pLeechObj clone() const override {
    Tuple res;
    res.reserve(tuple_.size());

    for (auto &&elem : tuple_)
      res.push_back(elem->clone());

    return std::make_unique<TupleObj>(std::move(res));
  }

  static pLeechObj deserialize(std::istream &ist) {
    auto len = deserializeNum<uint64_t>(ist);
    Tuple tuple{};
    for (uint64_t i = 0; i < len; ++i)
      tuple.push_back(deserializeObj(ist));

    return std::make_unique<TupleObj>(std::move(tuple));
  }

private:
  void serializeVal(std::ostream &ost) const override {
    for (auto &&ptr : tuple_)
      ptr->serialize(ost);
  }
};

inline pLeechObj deserializeObj(std::istream &ist) {
  auto cstTyVal = deserializeNum<std::underlying_type_t<ValueType>>(ist);
  auto cstTy = static_cast<ValueType>(cstTyVal);

  switch (cstTy) {
  case ValueType::Integer:
    return IntObj::deserialize(ist);
  case ValueType::Float:
    return FloatObj::deserialize(ist);
  case ValueType::String:
    return StringObj::deserialize(ist);
  case ValueType::Tuple:
    return TupleObj::deserialize(ist);
  default:
    return nullptr;
  }
}

} // namespace leech

#endif // __INCLUDE_LEECHOBJ_LEECHOBJ_HH__
