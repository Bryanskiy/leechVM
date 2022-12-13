#ifndef __INCLUDE_EXECUTOR_EXECUTOR_HH__
#define __INCLUDE_EXECUTOR_EXECUTOR_HH__

#include <functional>
#include <optional>
#include <stack>
#include <string_view>

#include "leechfile/leechfile.hh"

namespace leech {

constexpr std::string_view kMainFuncName = "main";

class StackFrame final {
  const FuncMeta *pmeta_ = nullptr;
  std::stack<pLeechObj> dataStack_{};
  std::unordered_map<std::string, pLeechObj> vars_{};

public:
  StackFrame(const FuncMeta *pmeta);

  StackFrame(const StackFrame &) = delete;
  StackFrame &operator=(const StackFrame &) = delete;
  StackFrame(StackFrame &&) = default;
  StackFrame &operator=(StackFrame &&) = default;

  template <class T, typename... Args>
  void emplace(Args &&...args) requires std::derived_from<T, LeechObj> {
    dataStack_.emplace(new T(std::forward<Args>(args)...));
  }

  void setVar(std::string_view name, pLeechObj obj) {
    setVar(std::string(name), std::move(obj));
  }

  void setVar(const std::string &name, pLeechObj obj) {
    vars_.at(name) = std::move(obj);
  }

  const auto *getVar(const std::string &name) const {
    return vars_.at(name).get();
  }

  const auto *getVar(std::string_view name) const {
    return getVar(std::string(name));
  }

  void push(pLeechObj obj);

  auto getConst(ArgType idx) const { return pmeta_->cstPool.at(idx).get(); }

  std::string_view getName(ArgType idx) const { return pmeta_->names.at(idx); }

  auto top() const { return dataStack_.top().get(); }

  auto popGetTos() {
    auto tos = top()->clone();
    pop();
    return tos;
  }

  void pop() { dataStack_.pop(); }
};

using FuncStack = std::stack<StackFrame>;

struct State final {
  FuncStack funcStack{};
  LeechFile *pFile{};
  std::uint64_t pc{};
  std::optional<std::uint64_t> nextPC{};

  State() = default;
  State(LeechFile *pfile);

  State(const State &) = default;
  State &operator=(const State &) = default;

  const auto &getInst(std::uint64_t idx) const { return pFile->code.at(idx); }

  auto &getCurFrame() { return funcStack.top(); }
};

using ExecFunc = std::function<void(const Instruction &, State &)>;

class Executor final {
  State state_{};
  static const std::unordered_map<Opcodes, ExecFunc> execMap_;

public:
  Executor(LeechFile *leechFile) : state_(leechFile) {}

  void execute();
};

} // namespace leech

#endif // __INCLUDE_EXECUTOR_EXECUTOR_HH__
