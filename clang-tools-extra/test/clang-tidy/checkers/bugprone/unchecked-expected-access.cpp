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

// emplace.

void emplace_then_access(std::expected<int, int> e) {
  e.emplace(1);
  e.value();
  *e;
}

void emplace_then_arrow_access(std::expected<Foo, int> e) {
  e.emplace();
  e->foo();
}

int emplace_if_empty(std::expected<int, int> e) {
  if (!e)
    e.emplace(0);
  return *e;
}

int emplace_through_pointer(std::expected<int, int> *p) {
  p->emplace(1);
  return **p;
}

void void_emplace_then_access(std::expected<void, int> e) {
  e.emplace();
  e.value();
}

void error_access_after_emplace(std::expected<int, int> e) {
  e.emplace(1);
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

int emplace_on_other_object(std::expected<int, int> a,
                            std::expected<int, int> b) {
  a.emplace(1);
  return *b;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

int emplace_on_one_path(std::expected<int, int> e, bool cond) {
  if (cond)
    e.emplace(1);
  return *e;
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
}

void void_error_access_after_emplace(std::expected<void, int> e) {
  e.emplace();
  e.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:5: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

// Comparison of two expected objects.

int equal_to_expected_with_value(std::expected<int, int> a,
                                 std::expected<int, int> b) {
  if (a == b && b)
    return *a;
  return 0;
}

int equal_to_expected_with_error(std::expected<int, int> a,
                                 std::expected<int, int> b) {
  if (a == b && !b)
    return a.error();
  return 0;
}

int equal_then_check_other(std::expected<int, int> a,
                           std::expected<int, int> b) {
  if (a == b) {
    if (b)
      return *a;
  }
  return 0;
}

int not_not_equal_to_expected_with_value(std::expected<int, int> a,
                                         std::expected<int, int> b) {
  if (a != b)
    return 0;
  if (b)
    return *a;
  return 0;
}

int equal_to_expected_with_other_value_type(std::expected<int, int> a,
                                            std::expected<long, int> b) {
  if (a == b && b)
    return *a;
  return 0;
}

void void_not_equal_to_expected_with_value(std::expected<void, int> a,
                                           std::expected<void, int> b) {
  if (a != b && a)
    b.error();
}

int equal_to_unchecked_expected(std::expected<int, int> a,
                                std::expected<int, int> b) {
  if (a == b)
    return *a;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
  return 0;
}

int not_equal_to_expected_with_value(std::expected<int, int> a,
                                     std::expected<int, int> b) {
  if (a != b && b)
    return *a;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
  return 0;
}

void not_equal_values_may_differ(std::expected<int, int> a,
                                 std::expected<int, int> b) {
  if (a != b && a)
    b.error();
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: unchecked access to 'std::expected' error [bugprone-unchecked-expected-access]
}

int equal_or_other_has_value(std::expected<int, int> a,
                             std::expected<int, int> b) {
  if (a == b || b)
    return *a;
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: unchecked access to 'std::expected' value [bugprone-unchecked-expected-access]
  return 0;
}
