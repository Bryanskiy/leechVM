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
  std::uint64_t retAddr_ = {};
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

  auto getRet() const { return retAddr_; }
  void setRet(auto val) { retAddr_ = val; }

  template <std::input_iterator T>
  void fillArgs(T beg, T end) requires
      std::is_same_v<typename std::iterator_traits<T>::value_type, pLeechObj> {
    for (auto &name : pmeta_->names) {
      if (beg == end)
        break;
      setVar(name, *beg++);
    }
  }

  void setVar(std::string_view name, pLeechObj obj) {
    setVar(std::string(name), obj);
  }

  auto stackSize() const { return dataStack_.size(); }

  void setVar(const std::string &name, pLeechObj obj) { vars_.at(name) = obj; }

  auto getVar(const std::string &name) const { return vars_.at(name); }

  auto getVar(std::string_view name) const { return getVar(std::string(name)); }

  void push(pLeechObj obj);

  auto getConst(ArgType idx) const { return pmeta_->cstPool.at(idx); }

  std::string_view getName(ArgType idx) const { return pmeta_->names.at(idx); }

  auto top() const { return dataStack_.top(); }

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
