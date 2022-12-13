#include "leechVM/leechVM.hh"

namespace leech {

void LeechVM::run() {
  driver_.parse();
  auto &&leechFile = driver_.getLeechFile();
  leechFile->dump2LeechFormat(std::cout);
}

} // namespace leech
