// RUN: %check_clang_tidy -std=c++17 %s transform-time-travel %t -- \
// RUN:   -- -I %S/Inputs/transform-time-travel

#include "chrono.h"
// CHECK-FIXES: {{^}}#include "step.h"

void sleep(long);
void sleep(chrono::seconds);

void f() {
  sleep(3);
  // CHECK-MESSAGES: [[@LINE-1]]:3: warning: Annotate `3` to be updated. [transform-time-travel]
  // CHECK-FIXES: $step::change<long, chrono::seconds>(3)
}
