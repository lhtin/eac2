#include <filesystem>
#include "test.h"
#include "utils.hpp"

using namespace std;

int main (int argc, char* args[]) {
  filesystem::path currentPath = filesystem::current_path();
  currentPath.append(argc >= 2 ? string(args[1]) : "");
  string p = currentPath.lexically_normal().string();

  checkTailPath(p, "/eac2/test/");

  test_re();
  test_parser(currentPath);
  return 0;
}