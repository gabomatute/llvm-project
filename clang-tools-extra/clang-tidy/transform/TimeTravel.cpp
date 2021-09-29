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
        changeTo(
            node("arg"),
            change(T(long), T(chrono::seconds), node("arg"))),
        cat("Annotate `", node("arg"), "` to be updated.")),
});

} // namespace transform
