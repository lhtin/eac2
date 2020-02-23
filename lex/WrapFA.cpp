#include <algorithm>
#include "all.hpp"

using namespace std;

WrapFA::WrapFA (FA* fa): fa(fa), current(fa->s0), at(0), finishedLength() {}
bool WrapFA::accept (char c) {
  auto it = find_if(
      fa->deltas.begin(), fa->deltas.end(),
      [&](FA::Delta* item) {
        return item->start == current && item->accept.count(c) > 0;
      });
  if (it != fa->deltas.end()) {
    current = (*it)->end;
    at += 1;
    if (isFinish()) {
      finishedLength.push_back(at);
    }
    return true;
  } else {
    return false;
  }
}
int WrapFA::getPrevFinishedDelta () {
  if (!finishedLength.empty()) {
    return at - finishedLength.back();
  } else {
    return 0;
  }
}
bool WrapFA::isFinish () {
  auto it = find_if(
      fa->SA.begin(), fa->SA.end(),
      [&](FA::State* item) {
        return item == current;
      });
  if (it != fa->SA.end()) {
    return true;
  } else {
    return false;
  }
}
void WrapFA::reset () {
  current = fa->s0;
  at = 0;
  finishedLength.clear();
}
