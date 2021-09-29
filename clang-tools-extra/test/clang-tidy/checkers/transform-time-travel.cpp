// RUN: %check_clang_tidy -std=c++17 %s transform-time-travel %t -- \
// RUN:   -- -I %S/Inputs/transform-time-travel

#include "chrono.h"

void sleep(long);
void sleep(chrono::seconds);

void f() {
  sleep(3);
  // CHECK-MESSAGES: [[@LINE-1]]:9: warning: Annotate `3` to be updated. [transform-time-travel]
  // CHECK-FIXES: $step::change<long, chrono::seconds>(3)
}
