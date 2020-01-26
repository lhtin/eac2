#include <iostream>
#include <ctime>
#include <string>
#include "utils.hpp"

using namespace std;

void printNow (string tag) {
  time_t timer = time(nullptr);
  tm* timeInfo = localtime(&timer);
  cout << "Now: " << asctime(timeInfo);
  if (!tag.empty()) {
    cout << " [" << tag << "]" << endl;
  }
}