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
struct type {
    explicit type(std::string name) : name(std::move(name)) {}
    template <typename T> static auto from(std::string name) { return type{std::move(name)}; }
    operator internal::Matcher<clang::QualType>() { return asString(name); }
    auto rv_ref() { return type(name + " &&"); }
    std::string name;
};

struct type_update {
    std::string step;
    type from, to;
};

const std::string step_prefix = "$step::";

auto marked(type_update update, internal::Matcher<clang::Stmt> expr) {
    // TODO: should `update.to` match referenced or resolved alias?
    return callExpr(
        callee(functionDecl(
            hasName(step_prefix + update.step),
            hasTemplateArgument(0, refersToType(update.from)),
            hasTemplateArgument(1, refersToType(update.to)),
            // templateArgumentCountIs(2), // FIX
            hasParameter(0, hasType(update.from.rv_ref())),
            parameterCountIs(1))),
        hasArgument(0, expr),
        argumentCountIs(1));
}

Stencil mark(type_update update, RangeSelector expr) {
    return cat(step_prefix, update.step, "<", update.from.name, ", ", update.to.name, ">(", expr,")");
}

} // namespace transform

// TODO: add macro sugar for named value references?
#define T(t) ::transform::type::from<t>(#t)

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_TRANSFORM_EXTENDEDDSL_H
