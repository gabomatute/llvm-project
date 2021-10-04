//===--- TimeTravel.cpp - transform -----------------------------*- C++ -*-===//

#include "ExtendedDSL.h"
#include "TimeTravel.h"

#include <vector>

// FIX: Dependency graph is really weird...
#include "../../test/clang-tidy/checkers/Inputs/transform-time-travel/chrono.h"

namespace transform {

auto TimeTravel::Rule = applyFirst({
    makeRule(
        traverse(clang::TK_AsIs, callExpr(
            callee(functionDecl(
                hasName("::sleep"),
                hasParameter(0, hasType(T(long))))),
            hasArgument(0, expr().bind("arg")),
            argumentCountIs(1))),
        flatten(
            addInclude("step.h"),
            changeTo(node("arg"), mark(
                {"change", T(long), T(chrono::seconds)},
                node("arg")))),
        cat("Annotate `", node("arg"), "` to be updated.")),
    makeRule(
        traverse(clang::TK_AsIs, marked(
            {"change", T(long), T(struct chrono::duration<long>)},
            integerLiteral(hasType(T(int))).bind("lit"))),
        changeTo(cat(node("lit"), "_s")),
        cat("Use type-safe `", node("lit"), "_s` literal.")),
});

} // namespace transform
