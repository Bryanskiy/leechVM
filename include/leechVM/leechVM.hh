#ifndef __INCLUDE_LEECHVM_LEECHVM_HH__
#define __INCLUDE_LEECHVM_LEECHVM_HH__

#include "executor/executor.hh"
#include "frontend/frontend.hh"

namespace leech {

class LeechVM final {
public:
  LeechVM(const LeechVM &) = delete;
  LeechVM(LeechVM &&) = delete;
  LeechVM &operator=(const LeechVM &) = delete;
  LeechVM &operator=(LeechVM &&) = delete;

  LeechVM(std::istream &in, std::ostream &out) : driver_{in, out} {}
  void run();

private:
  yy::Driver driver_;
};

} // namespace leech

#endif // __INCLUDE_LEECHVM_LEECHVM_HH__
