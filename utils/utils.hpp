#ifndef EAC2_UTILS_HPP
#define EAC2_UTILS_HPP

#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

void printNow (string tag = "");
unsigned long long getNow ();
string escape (char c);
string escape (string str);
void assert_with_msg (bool isOK, string errMsg);
// 判断文件内容是否跟content一致
bool isSame (ifstream& file, string content);
string joinPath(const filesystem::path& path1, string path2);
void checkTailPath (string currentPath, string expectedTailPath);

#endif //EAC2_UTILS_HPP
