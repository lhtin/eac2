#include <iostream>
#include <string>
#include <chrono>
#include <stdexcept>
#include <filesystem>

#include "utils.hpp"

using namespace std;

const char EMPTY = '\0';

unsigned long long getNow () {
  return chrono::system_clock::now().time_since_epoch().count();
}

void printNow (string tag) {
  auto now = getNow();

  cout << "Now: " << now << endl;
  if (!tag.empty()) {
    cout << " [" << tag << "]" << endl;
  }
}

string escape (char c) {
  string res;
  if (c == EMPTY) {
    res = "ε";
  } else if (c == '\t') {
    res = R"(\t)";
  } else if (c == ' ') {
    res = R"(\s)";
  } else if (c == '\n') {
    res = R"(\n)";
  } else {
    res = string(1, c);
  }
  return res;
}

string escape (string str) {
  string res;
  int len = str.length();
  for (int i = 0; i < len; i += 1) {
    res += escape(str[i]);
  }
  return res;
}

void assert_with_msg (bool isOK, string errMsg) {
  if (!isOK) {
    throw runtime_error(errMsg);
  }
}

bool isSame (ifstream& file, string content) {
  char eof = char_traits<char>::eof();
  int len = content.length();
  int at = 0;

  while (true) {
    char c1 = file.get();
    if (c1 == eof) {
      return at == len;
    }
    if (at >= len) {
      return false;
    }
    char c2 = content[at++];
    if (c1 != c2) {
      return false;
    }
  }
}

string joinPath(const filesystem::path& path1, string path2) {
  return filesystem::path(path1).append(path2).lexically_normal().string();
}