struct S {
  int m;
};


static S A[4] = (S[4])0;
void f() {
  S v = {int(1)};
  A[int(0)] = v;
}

