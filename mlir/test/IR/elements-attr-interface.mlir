// RUN: mlir-opt %s -test-elements-attr-interface -verify-diagnostics

// This test contains various `ElementsAttr` attributes, and tests the support
// for iterating the values of these attributes using various native C++ types.
// This tests that the abstract iteration of ElementsAttr works properly, and
// is properly failable when necessary.

// expected-error@below {{Test iterating `uint64_t`: 10, 11, 12, 13, 14}}
// expected-error@below {{Test iterating `APInt`: 10, 11, 12, 13, 14}}
// expected-error@below {{Test iterating `IntegerAttr`: 10 : i64, 11 : i64, 12 : i64, 13 : i64, 14 : i64}}
std.constant #test.i64_elements<[10, 11, 12, 13, 14]> : tensor<5xi64>

// expected-error@below {{Test iterating `uint64_t`: 10, 11, 12, 13, 14}}
// expected-error@below {{Test iterating `APInt`: 10, 11, 12, 13, 14}}
// expected-error@below {{Test iterating `IntegerAttr`: 10 : i64, 11 : i64, 12 : i64, 13 : i64, 14 : i64}}
std.constant dense<[10, 11, 12, 13, 14]> : tensor<5xi64>

// expected-error@below {{Test iterating `uint64_t`: unable to iterate type}}
// expected-error@below {{Test iterating `APInt`: unable to iterate type}}
// expected-error@below {{Test iterating `IntegerAttr`: unable to iterate type}}
std.constant opaque<"_", "0xDEADBEEF"> : tensor<5xi64>
