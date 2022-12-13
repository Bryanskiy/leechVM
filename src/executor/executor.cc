#include "executor/executor.hh"

namespace leech {
StackFrame::StackFrame(const FuncMeta *pmeta) : pmeta_(pmeta) {
  if (pmeta_ == nullptr)
    throw std::invalid_argument("Creating stack frame w/ Null func meta");

  for (auto &name : pmeta_->names)
    vars_.emplace(name, nullptr);
}

void StackFrame::push(pLeechObj obj) {
  if (nullptr == obj)
    throw std::invalid_argument("Trying to push nullptr into stackframe");

  dataStack_.push(obj);
}

State::State(LeechFile *pfile) : pFile(pfile) {
  if (nullptr == pFile)
    throw std::invalid_argument("Trying to execute null leech file");
  auto &meta = pFile->meta;
  auto *mainFrame = &meta.funcs.at(std::string(kMainFuncName));
  pc = mainFrame->addr;
  funcStack.emplace(mainFrame);
}

void Executor::execute() {
  auto &fStack = state_.funcStack;
  while (fStack.size() != 0) {
    auto curInst = state_.getInst(state_.pc);

    state_.nextPC.reset();

    auto instF = execMap_.find(curInst.getOpcode());
    if (instF == execMap_.end()) {
      std::ostringstream ss;
      ss << "Unsupported opcode "
         << static_cast<int>(toUnderlying(curInst.getOpcode()));
      throw std::runtime_error(ss.str());
    }
    instF->second(curInst, state_);

    state_.pc = state_.nextPC.value_or(state_.pc + 1);
  }
}

const std::unordered_map<Opcodes, ExecFunc> Executor::execMap_{
    {Opcodes::LOAD_CONST,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       curFrame.push(curFrame.getConst(inst.getArg()));
     }},
    {Opcodes::STORE_FAST,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto name = curFrame.getName(inst.getArg());
       curFrame.setVar(name, curFrame.top());
     }},
    {Opcodes::LOAD_FAST,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto name = curFrame.getName(inst.getArg());
       curFrame.push(curFrame.getVar(name));
     }},
    {Opcodes::COMPARE_OP,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto op = static_cast<CmpOp>(inst.getArg());
       auto tos1 = curFrame.popGetTos();
       auto tos2 = curFrame.popGetTos();

       bool res = tos2->compare(tos1.get(), op);

       curFrame.emplace<IntObj>(static_cast<Integer>(res));
     }},
    {Opcodes::POP_JUMP_IF_FALSE,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto dest = inst.getArg();
       auto tos = curFrame.popGetTos();
       IntObj fal(0);

       bool res = tos->compare(&fal, CmpOp::EQ);
       if (res)
         state.nextPC = dest;
     }},
    {Opcodes::POP_JUMP_IF_TRUE,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto dest = inst.getArg();
       auto tos = curFrame.popGetTos();
       IntObj fal(0);

       bool res = tos->compare(&fal, CmpOp::EQ);
       if (!res)
         state.nextPC = dest;
     }},
    {Opcodes::BINARY_ADD,
     [](const Instruction &, State &state) {
       auto &curFrame = state.getCurFrame();
       auto tos1 = curFrame.popGetTos();
       auto tos2 = curFrame.popGetTos();

       curFrame.push(tos1->add(tos2.get()));
     }},
    {Opcodes::POP_TOP,
     [](const Instruction &, State &state) { state.getCurFrame().pop(); }},
    {
        Opcodes::PRINT,
        [](const Instruction &, State &state) {
          state.getCurFrame().popGetTos()->print();
          std::cout << std::endl;
        },
    },
    {Opcodes::CALL_FUNCTION,
     [](const Instruction &inst, State &state) {
       auto &curFrame = state.getCurFrame();
       auto idx = inst.getArg();
       auto fName = std::string(curFrame.getName(idx));

       auto *fMeta = &state.pFile->meta.funcs.at(fName);
       state.nextPC = fMeta->addr;

       /* Get args from data stack */
       std::vector<pLeechObj> args(curFrame.stackSize());
       std::generate(args.begin(), args.end(),
                     [&curFrame] { return curFrame.popGetTos(); });

       curFrame.setRet(state.pc + 1);
       state.funcStack.emplace(fMeta);

       state.getCurFrame().fillArgs(args.begin(), args.end());
     }},
    {Opcodes::RETURN_VALUE,
     [](const Instruction &, State &state) {
       auto &fstack = state.funcStack;
       auto tos = state.getCurFrame().popGetTos();

       fstack.pop();

       if (fstack.size() != 0) {
         state.nextPC = state.getCurFrame().getRet();
         state.getCurFrame().push(tos);
       }
     }},
    {Opcodes::BINARY_SUBSCR,
     [](const Instruction &, State &state) {
       auto &curFrame = state.getCurFrame();
       auto idx = curFrame.popGetTos();
       auto tuple = curFrame.popGetTos();

       curFrame.push(tuple->subscript(idx.get()));
     }},
    {Opcodes::BINARY_TRUE_DIVIDE, [](const Instruction &, State &state) {
       auto &curFrame = state.getCurFrame();
       auto two = curFrame.popGetTos();
       auto one = curFrame.popGetTos();

       curFrame.push(one->div(two.get()));
     }}};
} // namespace leech
