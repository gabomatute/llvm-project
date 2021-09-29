#ifndef LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_STEP_H
#define LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_STEP_H

// Intermediate step stubs

namespace $step {
    // Mark expr that should be updated
    template <typename T, typename U> U& change(T&); // preserves assignement
    template <typename T, typename U> U change(T&&); // avoids making copies
    // TODO: Should this return `U&&`?

    // Mark expr that has been updated
    template <typename T, typename U> T& changed(U&); // preserves assignement
    template <typename T, typename U> T changed(U&&); // avoids making copies
}


// Sample specializations
// (allows the code to also run, which is useful, but not required or even possible)
//
// #include "chrono.h"
//
// namespace $step {
//     template <>
//     chrono::seconds $change<long, chrono::seconds>(long&& input) {
//         return chrono::seconds{input};
//     }
//
//     template <>
//     long $changed<long, chrono::seconds>(chrono::seconds&& input) {
//         return input.count(); // FIX: is it in the right units?
//     }
// }

#endif // LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_TRANSFORM_TIME_TRAVEL_STEP_H
