#include <filesystem>
#include <spdlog/spdlog.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "leechVM/leechVM.hh"

namespace fs = std::filesystem;

int main(int argc, char **argv) try {
  CLI::App app{"LeechVM"};
  fs::path input{};
  app.add_option("input", input, ".leech file")->required();

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }

  std::fstream in(input.c_str());
  if (!in.is_open()) {
    throw std::invalid_argument("can't find .leech file");
  }

  leech::LeechVM vm(in, std::cout);
  vm.run();
} catch (const std::exception &e) {
  spdlog::error(e.what());
  return 1;
}
