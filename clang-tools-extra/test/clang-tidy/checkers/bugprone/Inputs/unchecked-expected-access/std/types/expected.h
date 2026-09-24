#ifndef LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_STD_TYPES_EXPECTED_H_
#define LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_STD_TYPES_EXPECTED_H_

/// Mock of `std::expected`.
// TODO: move to checkers/Inputs/Headers/std/expected?
namespace std {

template <class E> class unexpected {
public:
  unexpected(const unexpected &) = default;
  unexpected(unexpected &&) = default;

  template <class Err = E> explicit unexpected(Err &&e);

  const E &error() const & noexcept;
  E &error() & noexcept;
  const E &&error() const && noexcept;
  E &&error() && noexcept;
};

template <class E> unexpected(E) -> unexpected<E>;

struct unexpect_t {
  explicit unexpect_t() = default;
};

inline constexpr unexpect_t unexpect{};

template <class T, class E> class expected {
public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  expected();
  expected(const expected &);
  expected(expected &&) noexcept;

  template <class U = T> expected(U &&v);

  template <class G> expected(const unexpected<G> &e);
  template <class G> expected(unexpected<G> &&e);

  template <class... Args>
  explicit expected(unexpect_t, Args &&...args);

  expected &operator=(const expected &);
  expected &operator=(expected &&) noexcept;

  const T *operator->() const noexcept;
  T *operator->() noexcept;

  const T &operator*() const & noexcept;
  T &operator*() & noexcept;
  const T &&operator*() const && noexcept;
  T &&operator*() && noexcept;

  explicit operator bool() const noexcept;
  bool has_value() const noexcept;

  const T &value() const &;
  T &value() &;
  const T &&value() const &&;
  T &&value() &&;

  const E &error() const & noexcept;
  E &error() & noexcept;
  const E &&error() const && noexcept;
  E &&error() && noexcept;

  template <class U> T value_or(U &&v) const &;
  template <class U> T value_or(U &&v) &&;

  template <class... Args> T &emplace(Args &&...args) noexcept;

  void swap(expected &rhs) noexcept;
};

template <class E> class expected<void, E> {
public:
  using value_type = void;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  expected() noexcept;
  expected(const expected &);
  expected(expected &&) noexcept;

  template <class G> expected(const unexpected<G> &e);
  template <class G> expected(unexpected<G> &&e);

  template <class... Args>
  explicit expected(unexpect_t, Args &&...args);

  expected &operator=(const expected &);
  expected &operator=(expected &&) noexcept;

  explicit operator bool() const noexcept;
  bool has_value() const noexcept;

  void operator*() const noexcept;

  void value() const &;
  void value() &&;

  const E &error() const & noexcept;
  E &error() & noexcept;
  const E &&error() const && noexcept;
  E &&error() && noexcept;

  void emplace() noexcept;

  void swap(expected &rhs) noexcept;
};

} // namespace std

#endif // LLVM_CLANG_TOOLS_EXTRA_TEST_CLANG_TIDY_CHECKERS_INPUTS_STD_TYPES_EXPECTED_H_
