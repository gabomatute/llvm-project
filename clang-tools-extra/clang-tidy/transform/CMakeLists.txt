set(LLVM_LINK_COMPONENTS support)

add_clang_library(clangTidyTransformModule
    TransformTidyModule.cpp
    TimeTravel.cpp

    # LINK_LIBS
    # clangTidy
    # clangTidyUtils
    )

clang_target_link_libraries(clangTidyTransformModule
    PRIVATE
    clangTransformer
    )
