/*
 * clang -O2 -c -S -emit-llvm
 *
 */

#include <math.h>

extern double val;

extern void foo(double *ptr);

int bar() {
  double *pval = &val;
  foo(pval);
  double result = sqrt(*pval);

  return result;
}
