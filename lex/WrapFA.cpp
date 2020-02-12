#include <algorithm>
#include "all.hpp"

using namespace std;

WrapFA::WrapFA (FA* fa): fa(fa), current(fa->s0), current2(fa->s0->n), deltas(fa->getDeltas()) {}
bool WrapFA::accept2 (char c) {
  auto it1 = deltas.find(current2);
  if (it1 != deltas.end()) {
    auto it2 = it1->second.find(c);
    if (it2 != it1->second.end()) {
      current2 = it2->second;
      return true;
    }
  }
  return false;
}
bool WrapFA::isFinish2 () {
  auto it = find_if(
      fa->SA.begin(), fa->SA.end(),
      [&](FA::State* item) {
        return item->n == current2;
      });
  if (it != fa->SA.end()) {
    return true;
  } else {
    return false;
  }
}
void WrapFA::reset2 () {
  current2 = fa->s0->n;
}
bool WrapFA::accept (char c) {
  auto it = find_if(
      fa->deltas.begin(), fa->deltas.end(),
      [&](FA::Delta* item) {
        return item->start == current && item->accept.count(c) > 0;
      });
  if (it != fa->deltas.end()) {
    current = (*it)->end;
    return true;
  } else {
    return false;
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
}
