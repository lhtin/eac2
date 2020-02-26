#include "all.hpp"
#include "utils.hpp"
#include <iostream>
#include <vector>
#include <utility>

// Select (|) Connect Closure (*) Group (()) Leaf ε
// \\ \t \n \( \) \[ \] \* \+ \-
vector<pair<string, string>> TEST_SET = {
    {"", R"(
(Leaf "ε")
)"},

    {"a", R"(
(Leaf "a")
)"},

    {"a*", R"(
(Closure
  (Leaf "a"))
)"},

    {"a+", R"(
(Connect
  (Leaf "a")
  (Closure
    (Leaf "a")))
)"},

    {"[a-c]", R"(
(Leaf "a|b|c")
)"},

    {"(ab)|c", R"(
(Select
  (Group
    (Connect
      (Leaf "a")
      (Leaf "b")))
  (Leaf "c"))
)"},

    {"(a|)b", R"(
(Connect
  (Group
    (Select
      (Leaf "a")
      (Leaf "ε")))
  (Leaf "b"))
)"},

    {R"([\(\[][\+\*][\)\-\]])", R"(
(Connect
  (Leaf "(|[")
  (Connect
    (Leaf "*|+")
    (Leaf ")|-|]")))
)"},

    {R"(
 	)", R"(
(Connect
  (Leaf "\n")
  (Connect
    (Leaf "\s")
    (Leaf "\t")))
)"},

    {R"(a|(b*|c+))", R"(
(Select
  (Leaf "a")
  (Group
    (Select
      (Closure
        (Leaf "b"))
      (Connect
        (Leaf "c")
        (Closure
          (Leaf "c"))))))
)"}
};

void test_re () {
  cout << endl;
  int total = 0;
  int success = 0;
  for (const auto& item : TEST_SET) {
    total += 1;
    RETree t(item.first);
    string result = "\n" + t.head.toString() + "\n";
    if (result == item.second) {
      success += 1;
      cout << "/" + escape(item.first) + "/ => passed" << endl;
    } else {
      cout << "/" + escape(item.first) + "/ => failed" << endl;
      cout << "expected: " << item.second << endl;
      cout << "got: " << result << endl;
    }
  }
  cout << "RE.cpp test summary: total(" << total << ") success(" << success << ") fail(" << total - success << ")" << endl;
}