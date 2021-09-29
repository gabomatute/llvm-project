//===--- TransformTidyModule.cpp - transform ------------------------------===//

#include "../ClangTidy.h"
#include "../ClangTidyModule.h"
#include "../ClangTidyModuleRegistry.h"
#include "TimeTravel.h"

namespace transform {

using namespace clang::tidy;

class TransformModule : public ClangTidyModule {
public:
  void addCheckFactories(ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<transform::TimeTravel>("transform-time-travel");
  }
};

// Register the TransformModule using this statically initialized variable.
static ClangTidyModuleRegistry::Add<TransformModule>
    X("transform-module", "Adds transform-related checks.");

} // namespace transform


namespace clang {
namespace tidy {

// This anchor is used to force the linker to link in the generated object file
// and thus register the TransformModule.
volatile int TransformModuleAnchorSource = 0;

} // namespace tidy
} // namespace clang
