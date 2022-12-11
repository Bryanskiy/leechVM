#include <spdlog/spdlog.h>

#include "frontend/frontend.hh"

int main() try {
  yy::Driver driver(std::cin, std::cout);
} catch (const std::exception &e) {
  spdlog::error(e.what());
  return 1;
}
