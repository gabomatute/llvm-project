//===--- ExtendedDSL.h - transform ------------------------------*- C++ -*-===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_EXTENDEDDSL_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_EXTENDEDDSL_H

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/Transformer/RewriteRule.h"
#include "clang/Tooling/Transformer/RangeSelector.h"
#include "clang/Tooling/Transformer/Stencil.h"

// FIX: Dependency graph is really weird...
// #include "../../test/clang-tidy/checkers/Inputs/transform-time-travel/step.h"

namespace transform {

using namespace clang::ast_matchers;
using namespace clang::transformer;

/// Please use the T macro to construct
template <typename T> struct type {
    type(const char* const name) : name(name) {}
    operator internal::Matcher<clang::QualType>() { return asString(name); }
    const char* const name;
};

template <typename T, typename U>
Stencil change(type<T> current, type<U> target, RangeSelector expr) {
    return cat("$step::change<", current.name, ", ", target.name, ">(", expr,")");
}

} // namespace transform

// TODO: add macro sugar for named value references?
#define T(t) ::transform::type<t>(#t)

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_EXTENDEDDSL_H
