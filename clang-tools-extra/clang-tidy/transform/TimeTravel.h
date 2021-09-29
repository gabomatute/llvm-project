//===--- TimeTravel.h - transform -------------------------------*- C++ -*-===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_TIMETRAVEL_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_TIMETRAVEL_H

#include "../ClangTidy.h"
#include "../utils/TransformerClangTidyCheck.h"
#include "clang/Tooling/Transformer/RewriteRule.h"

namespace transform {

class TimeTravel : public clang::tidy::utils::TransformerClangTidyCheck {
public:
    TimeTravel(clang::StringRef Name, clang::tidy::ClangTidyContext *Context)
        : clang::tidy::utils::TransformerClangTidyCheck(Rule, Name, Context) {}

    static clang::transformer::RewriteRule Rule;
};

} // namespace transform

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_TIMETRAVEL_H
