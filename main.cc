#include <fstream>
#include "driver/driver.hh"

int main(int argc, char** argv) {
    std::ifstream in(argv[1], std::ifstream::in);
    yy::Driver driver(in, std::cout);
    driver.parse();
}