// RUN: %check_clang_tidy -std=c++17 %s transform-time-travel %t -- \
// RUN:   -- -I %S/Inputs/transform-time-travel

#include "chrono.h"
#include "step.h"

void sleep(long);
void sleep(chrono::seconds);

void f() {
  sleep(3);
  // CHECK-MESSAGES: [[@LINE-1]]:3: warning: Annotate `3` to be updated. [transform-time-travel]
  // CHECK-FIXES: $step::change<long, chrono::seconds>(3)

  sleep($step::change<long, chrono::seconds>(3));
  // CHECK-MESSAGES: [[@LINE-1]]:9: warning: Use type-safe `3_s` literal. [transform-time-travel]
  // CHECK-FIXES: 3_s
}
