// RUN: %clang_cc1 -fsyntax-only -verify -std=c23 %s
// RUN: %clang_cc1 -fsyntax-only -verify -fexperimental-new-constant-interpreter -std=c23 %s
// RUN: %clang_cc1 -fsyntax-only -verify -x c++ %s
// RUN: %clang_cc1 -fsyntax-only -verify -fexperimental-new-constant-interpreter -x c++ %s

// Previously crashed with an assertion in IntExprEvaluator::Success.
// https://github.com/llvm/llvm-project/issues/227733

constexpr char e = 1; // expected-note 2 {{previous definition is here}}
constexpr char e = 1; // expected-error {{redefinition of 'e'}}
constexpr unsigned e; // expected-error {{redefinition of 'e' with a different type: 'const unsigned int' vs 'const char'}}
int x = e;

// Test when all previous declarations in the redeclaration chain are invalid:
// nothing should crash and the redefinition chain should still compile cleanly.
invalid_type_1 a; // expected-error {{unknown type name 'invalid_type_1'}}
invalid_type_2 a; // expected-error {{unknown type name 'invalid_type_2'}}
int a;
int y = a;

// Test: valid declaration, followed by an invalid one, followed by a valid one,
// followed by a use. The use must still evaluate correctly.
constexpr int g = 1; // expected-note {{previous definition is here}}
invalid_type_3 g;    // expected-error {{unknown type name 'invalid_type_3'}} \
                        expected-error {{redefinition of 'g'}}
// A second valid redeclaration after an invalid one.
// In C23/C++ there are no tentative definitions for constexpr, so this
// would be a redefinition, but the invalid decl already consumed the name.
// We just check no crash occurs on the use below.
int z = g;
