// RUN: %check_clang_tidy -std=c++23-or-later %s bugprone-unchecked-expected-access %t -- -- -I %S/Inputs/unchecked-expected-access

#include "std/types/expected.h"

struct Foo {
  void foo() const {}
};

std::expected<int, int> make();

// Unchecked accesses.

void unchecked_value_access(std::expected<int, int> e) {
  e.value();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void unchecked_deref_operator_access(std::expected<int, int> e) {
  *e;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void unchecked_arrow_operator_access(std::expected<Foo, int> e) {
  e->foo();
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void const_ref_unchecked_value_access(const std::expected<int, int> &e) {
  e.value();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void const_ref_unchecked_deref_operator_access(
    const std::expected<int, int> &e) {
  *e;
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void const_ref_unchecked_arrow_operator_access(
    const std::expected<Foo, int> &e) {
  e->foo();
  // CHECK-MESSAGES: :[[@LINE-1]]:4: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void unchecked_access_to_function_result() {
  auto e = make();
  e.value();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void access_in_else_branch(std::expected<int, int> e) {
  if (e.has_value()) {
  } else {
    e.value();
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
  }
}

void access_in_negated_check(std::expected<int, int> e) {
  if (!e) {
    *e;
    // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
  }
}

int access_in_ternary_with_negated_check(std::expected<int, int> e) {
  return !e ? *e : 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:15: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void void_unchecked_value_access(std::expected<void, int> e) {
  e.value();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

// Checked accesses.

void checked_access_with_has_value(std::expected<Foo, int> e) {
  if (e.has_value()) {
    e.value();
    *e;
    e->foo();
  }
}

void checked_access_with_operator_bool(std::expected<int, int> e) {
  if (e) {
    e.value();
    *e;
  }
}

void checked_access_after_early_return(std::expected<int, int> e) {
  if (!e)
    return;
  e.value();
}

int checked_access_after_early_return_with_has_value(
    std::expected<int, int> e) {
  if (!e.has_value())
    return 0;
  return *e;
}

int checked_access_in_ternary(std::expected<int, int> e) {
  return e ? *e : 0;
}

int checked_access_in_ternary_with_has_value(std::expected<int, int> e) {
  return e.has_value() ? e.value() : 0;
}

void value_or_is_not_an_access(std::expected<int, int> e) {
  e.value_or(0);
}

// Unchecked error accesses.

void unchecked_error_access(std::expected<int, int> e) {
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

void const_ref_unchecked_error_access(const std::expected<int, int> &e) {
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

void unchecked_error_access_to_function_result() {
  auto e = make();
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

void error_access_in_operator_bool_check(std::expected<int, int> e) {
  if (e) {
    e.error();
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
  }
}

void error_access_in_has_value_check(std::expected<int, int> e) {
  if (e.has_value()) {
    e.error();
    // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
  }
}

void error_access_after_early_return(std::expected<int, int> e) {
  if (!e)
    return;
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

int error_access_in_ternary(std::expected<int, int> e) {
  return e ? e.error() : 0;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

void void_unchecked_error_access(std::expected<void, int> e) {
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

// Checked error accesses.

void checked_error_access_with_operator_bool(std::expected<int, int> e) {
  if (!e) {
    e.error();
  }
}

void checked_error_access_with_has_value(std::expected<int, int> e) {
  if (!e.has_value()) {
    e.error();
  }
}

void checked_error_access_in_else_branch(std::expected<int, int> e) {
  if (e) {
  } else {
    e.error();
  }
}

int checked_error_access_after_early_return(std::expected<int, int> e) {
  if (e)
    return 0;
  return e.error();
}

int checked_error_access_in_ternary(std::expected<int, int> e) {
  return e ? 0 : e.error();
}

void checked_value_and_error_access(std::expected<int, int> e) {
  if (e) {
    *e;
  } else {
    e.error();
  }
}

void void_checked_error_access(std::expected<void, int> e) {
  if (!e)
    e.error();
}

void error_or_is_not_an_access(std::expected<int, int> e) {
  e.error_or(0);
}

void unexpected_error_is_not_diagnosed() {
  std::unexpected<int> u(1);
  u.error();
}
