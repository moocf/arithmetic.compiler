#pragma once
#include <sstream>
#include <string>

using namespace std;


struct Ast {
  virtual void tostr(ostream& o) = 0;
  string str() { stringstream o; tostr(o); return o.str(); }
  friend ostream& operator<<(ostream& o, Ast& a) { a.tostr(o); return o; }
};

struct Num : Ast {
  float n;
  Num(float n) : n(n) {}
  void tostr(ostream& o) { o<<n; }
};

struct Exp : Ast {
  char op;
  Ast *l, *r;
  Exp(char op, Ast *l, Ast *r) : op(op), l(l), r(r) {}
  void tostr(ostream& o) { o<<'('<<op<<' '<<*l<<' '<<*r<<')'; }
};
