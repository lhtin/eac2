#ifndef EAC2_UTILS_HPP
#define EAC2_UTILS_HPP

#include <string>

using namespace std;

void printNow (string tag = "");
unsigned long long getNow ();
string escape (char c);
string escape (string str);
void assert_with_msg (bool isOK, string errMsg);

#endif //EAC2_UTILS_HPP
