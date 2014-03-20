#include <map>
#include <stack>
#include <functional>
#include <utility>
#include <stdlib.h>
#include "calc.h"

using namespace std;

double calc(char* str) {
  stack<double> s;  stack< pair<int, char> > ops;

  auto p = [&s, &ops] (function<double (double, double)>& f)
    {double r=s.top();s.pop();r=f(s.top(),r);s.pop();s.push(r);ops.pop();};

  map< char, pair< int, function<double (double, double)> > > m =
    {{'+', {1, [](double a, double b){return a+b;}}},{'-', {1, [](double a, double b){return a-b;}}},
     {'*', {2, [](double a, double b){return a*b;}}},{'/', {2, [](double a, double b){return a/b;}}}};

  const int order = 2; int level = 0;
  for (char* sp = str;; ++sp) {
    while (*sp == '(') {level += order; ++sp;}

    s.push(strtod(sp, &sp));

    while (*sp == ')') {level -= order; ++sp;}

    if (!*sp) {while(!ops.empty()) p(m[ops.top().second].second); break;}

    const int op =  m[*sp].first + level;
    while (!ops.empty() && ops.top().first >= op) p(m[ops.top().second].second);

    ops.push(make_pair(op, *sp));
  }

  return s.top();
}
