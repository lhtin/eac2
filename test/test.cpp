#include <filesystem>
#include "test.h"
#include "utils.hpp"

using namespace std;

int main (int argc, char* args[]) {
  filesystem::path currentPath = filesystem::current_path();
  currentPath.append(argc >= 2 ? string(args[1]) : "");
  string p = currentPath.lexically_normal().string();

  const string expectedTailPath1 = "EaC2/test";
  const string expectedTailPath2 = "EaC2/test/";
  string::size_type index1 = p.rfind(expectedTailPath1);
  string::size_type index2 = p.rfind(expectedTailPath2);
  assert_with_msg(
      (index1 != string::npos && p.size() - index1 == expectedTailPath1.size()) ||
      (index2 != string::npos && p.size() - index2 == expectedTailPath2.size()),
      "需要在项目的test目录下运行测试程序，当前所在环境为：" + p);
  test_re();
  test_parser(currentPath);
  return 0;
}