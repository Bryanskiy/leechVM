#include "test_header.hh"

#include <fstream>
#include "frontend/frontend.hh"
#include "config.hh"

#define PATH(test) { TESTS_DIR test }

TEST(frontend, fibLexicalErr)
{
  std::ifstream file(PATH("fib.leech"));
  ASSERT_EQ(file.is_open(), true);
  yy::Driver driver(file, std::cout);
  bool res = driver.parse();
  ASSERT_EQ(res, true);
  file.close();
}

TEST(frontend, averageLexicalErr)
{
  std::ifstream file(PATH("average.leech"));
  ASSERT_EQ(file.is_open(), true);
  yy::Driver driver(file, std::cout);
  bool res = driver.parse();
  ASSERT_EQ(res, true);
  file.close();
}

#include "test_footer.hh"
