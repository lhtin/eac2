#include <iostream>
#include <ctime>
#include <string>
#include <chrono>
#include "utils.hpp"

using namespace std;

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