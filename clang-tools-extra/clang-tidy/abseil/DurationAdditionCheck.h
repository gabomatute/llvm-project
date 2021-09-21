//===--- DurationAdditionCheck.h - clang-tidy -------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ABSEIL_TIMEADDITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ABSEIL_TIMEADDITIONCHECK_H

#include "../ClangTidyCheck.h"
#include "../utils/TransformerClangTidyCheck.h"

namespace clang {
namespace tidy {
namespace abseil {

class DurationAdditionCheck : public utils::TransformerClangTidyCheck {
 public:
  DurationAdditionCheck(StringRef Name, ClangTidyContext *Context);
};

} // namespace abseil
} // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_ABSEIL_TIMEADDITIONCHECK_H
