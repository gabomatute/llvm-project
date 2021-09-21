//===--- DurationAdditionCheck.cpp - clang-tidy----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DurationAdditionCheck.h"
#include "DurationRewriter.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/FixIt.h"
#include "clang/Tooling/Transformer/RewriteRule.h"
#include "clang/Tooling/Transformer/Stencil.h"

using namespace clang::ast_matchers;
using namespace clang::transformer;

namespace clang {
namespace tidy {
namespace abseil {

// M: absl::ToUnix[Units]($lhs) + absl::ToUnix[Units]($rhs)
// >> absl::ToUnix[Units]($lhs + $rhs)

// M: absl::ToUnix[Units]($lhs) + $rhs
// >> absl::ToUnix[Units]($lhs + absl::Hours($rhs))

// M: $lhs + absl::ToUnix[Units]($rhs)
// >> absl::ToUnix[Units](absl::Hours($lhs) + $rhs)

auto DurationAdditionRule = applyFirst({
    // ToUnixMinutes
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixHours"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixHours"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixHours(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixHours"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixHours(", expression("lhs"), " + absl::Hours(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixHours"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixHours(absl::Hours(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    // ToUnixMinutes
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMinutes"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMinutes"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMinutes(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMinutes"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixMinutes(", expression("lhs"), " + absl::Minutes(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMinutes"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMinutes(absl::Minutes(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    // ToUnixSeconds
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixSeconds"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixSeconds"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixSeconds(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixSeconds"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixSeconds(", expression("lhs"), " + absl::Seconds(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixSeconds"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixSeconds(absl::Seconds(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    // ToUnixMillis
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMillis"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMillis"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMillis(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMillis"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixMillis(", expression("lhs"), " + absl::Milliseconds(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMillis"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMillis(absl::Milliseconds(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    // ToUnixMicros
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMicros"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMicros"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMicros(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMicros"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixMicros(", expression("lhs"), " + absl::Microseconds(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixMicros"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixMicros(absl::Microseconds(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    // ToUnixNanos
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixNanos"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixNanos"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixNanos(", expression("lhs"), " + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixNanos"))), hasArgument(0, expr().bind("lhs"))))),
        hasRHS(expr().bind("rhs"))),
      changeTo(
        cat("absl::ToUnixNanos(", expression("lhs"), " + absl::Nanoseconds(", node("rhs"), "))")),
      cat("perform addition in the duration domain")),
    makeRule(
      binaryOperator(
        hasOperatorName("+"),
        hasLHS(expr().bind("lhs")),
        hasRHS(ignoringParenImpCasts(callExpr(callee(functionDecl(hasName("::absl::ToUnixNanos"))), hasArgument(0, expr().bind("rhs")))))),
      changeTo(
        cat("absl::ToUnixNanos(absl::Nanoseconds(", node("lhs"), ") + ", expression("rhs"), ")")),
      cat("perform addition in the duration domain")),
});

DurationAdditionCheck::DurationAdditionCheck(StringRef Name,
                                             ClangTidyContext *Context)
    : TransformerClangTidyCheck(DurationAdditionRule, Name, Context) {}

} // namespace abseil
} // namespace tidy
} // namespace clang
