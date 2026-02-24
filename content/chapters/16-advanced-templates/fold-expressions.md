# Fold Expressions & Parameter Packs

Fold expressions (C++17) let you apply a binary operator across all elements of a template parameter pack in a single, concise expression. Combined with variadic templates introduced in C++11, they eliminate the need for recursive template expansions and make generic, type-safe functions dramatically simpler to write.

## Variadic Templates Review

A variadic template accepts an arbitrary number of template arguments using a **parameter pack**. You expand the pack with `...` and can query its size with `sizeof...`. Before C++17, processing packs required recursive helper functions.

### How it works

```cpp
#include <iostream>
#include <cstddef>

// Base case: no arguments
void print() {
    std::cout << std::endl;
}

// Recursive case: peel off the first argument, recurse on the rest
template<typename T, typename... Args>
void print(T first, Args... rest) {
    std::cout << first;
    if constexpr (sizeof...(rest) > 0)
        std::cout << ", ";
    print(rest...);
}

int main() {
    print(1, 2.5, "hello", 'c');  // 1, 2.5, hello, c
    std::cout << "Count: " << 4 << std::endl;
    return 0;
}
```

| Concept | Syntax | Detail |
|---------|--------|--------|
| Template parameter pack | `typename... Args` | Zero or more type parameters |
| Function parameter pack | `Args... args` | Corresponding function arguments |
| Pack expansion | `args...` | Expands to a comma-separated list |
| `sizeof...(Args)` | Returns the number of elements in the pack |
| Recursive processing | Peel first, recurse on rest | Classic C++11 pattern |

### Practice

1. 🌟🌟 **Fill:** Complete the variadic `count_args` function that returns the number of arguments passed to it using `sizeof...`.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename... Args>
std::size_t count_args(Args... args) {
    return __;
}

int main() {
    assert(count_args() == 0);
    assert(count_args(1) == 1);
    assert(count_args(1, 2.0, "three") == 3);
    assert(count_args('a', 'b', 'c', 'd', 'e') == 5);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Fold Expressions

C++17 fold expressions collapse a parameter pack with an operator in one line. There are four forms: unary left fold, unary right fold, binary left fold, and binary right fold. They cover addition, logical operations, comma-separated evaluation, and more.

### How it works

```cpp
#include <iostream>

// Unary left fold: (... op pack) => ((a1 op a2) op a3) op ...
template<typename... Args>
auto sum(Args... args) {
    return (... + args);
}

// Unary right fold: (pack op ...) => a1 op (a2 op (a3 op ...))
template<typename... Args>
auto sub_right(Args... args) {
    return (args - ...);
}

// Binary left fold: (init op ... op pack) — uses an initial value
template<typename... Args>
auto sum_from(int init, Args... args) {
    return (init + ... + args);
}

// Comma fold to call a function on each argument
template<typename... Args>
void print_all(Args... args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

int main() {
    std::cout << sum(1, 2, 3, 4)          << std::endl;  // 10
    std::cout << sub_right(1, 2, 3)       << std::endl;  // 1-(2-3) = 2
    std::cout << sum_from(100, 1, 2, 3)   << std::endl;  // 106
    print_all("hello", 42, 3.14);                         // hello 42 3.14
    return 0;
}
```

| Fold Form | Syntax | Expansion |
|-----------|--------|-----------|
| Unary left | `(... op pack)` | `((a1 op a2) op a3) op ...` |
| Unary right | `(pack op ...)` | `a1 op (a2 op (... op an))` |
| Binary left | `(init op ... op pack)` | `((init op a1) op a2) op ...` |
| Binary right | `(pack op ... op init)` | `a1 op (a2 op (... op init))` |
| Comma fold | `((expr), ...)` | Evaluates `expr` for each pack element |

### Practice

2. 🌟🌟 **Fill:** Write `product` using a fold expression that multiplies all arguments together.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename... Args>
auto product(Args... args) {
    return __;
}

int main() {
    assert(product(2, 3, 4) == 24);
    assert(product(1, 2, 3, 4, 5) == 120);
    assert(product(10, 10) == 100);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟 **Fill:** Write `all_true` using a fold expression with the `&&` operator that returns `true` only if all arguments are `true`.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename... Args>
bool all_true(Args... args) {
    return __;
}

int main() {
    assert(all_true(true, true, true) == true);
    assert(all_true(true, false, true) == false);
    assert(all_true(1, 1, 1) == true);
    assert(all_true(1, 0, 1) == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Practical Applications

Fold expressions and parameter packs unlock powerful, type-safe patterns: summing heterogeneous values, printing any number of arguments, checking compile-time conditions on all types, and building flexible APIs that accept any combination of types.

### How it works

```cpp
#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>

// Check at compile time that all types are arithmetic
template<typename... Ts>
constexpr bool all_arithmetic() {
    return (std::is_arithmetic_v<Ts> && ...);
}

// Concatenate any number of values into a string
template<typename... Args>
std::string concat(Args&&... args) {
    std::ostringstream oss;
    ((oss << args), ...);
    return oss.str();
}

// Sum only if all types are arithmetic (static_assert + fold)
template<typename... Args>
auto safe_sum(Args... args) {
    static_assert((std::is_arithmetic_v<Args> && ...),
                  "All arguments must be arithmetic");
    return (... + args);
}

int main() {
    static_assert(all_arithmetic<int, double, float>());
    static_assert(!all_arithmetic<int, std::string>());

    std::cout << concat("Name: ", "Alice", ", Age: ", 30) << std::endl;
    std::cout << safe_sum(1, 2.5, 3.0f) << std::endl;  // 6.5
    return 0;
}
```

| Pattern | Technique |
|---------|-----------|
| Type checking all params | `(std::is_trait_v<Ts> && ...)` fold |
| Safe operations | `static_assert` with fold over traits |
| Stream output | Comma fold with `<<` operator |
| String building | `std::ostringstream` with comma fold |

### Practice

4. 🌟🌟🌟 **Complete:** Write a `print_csv` function that prints all arguments separated by commas (no trailing comma). Use a fold expression with a helper technique to handle the separator.

```cpp,editable
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

template<typename... Args>
std::string print_csv(Args&&... args) {
    std::ostringstream oss;
    bool first = true;
    // Use a comma fold expression. For each arg:
    //   - if first, just print it and set first=false
    //   - otherwise, print "," then the arg
    (__ , ...);
    return oss.str();
}

int main() {
    assert(print_csv(1, 2, 3) == "1,2,3");
    assert(print_csv("hello", " ", "world") == "hello, ,world");
    assert(print_csv(42) == "42");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Write `all_integral` that checks at compile time whether every type in a parameter pack is integral, and `sum_checked` that uses `static_assert` with that check before computing the sum with a fold expression.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

// Returns true if all Ts are integral types
template<typename... Ts>
constexpr bool all_integral() {
    return __;
}

// Sums all arguments, but static_asserts that all are integral first
template<typename... Args>
auto sum_checked(Args... args) {
    __;  // static_assert that all Args are integral
    return __;  // fold expression to sum
}

int main() {
    static_assert(all_integral<int, long, short>());
    static_assert(all_integral<char, bool, unsigned>());
    static_assert(!all_integral<int, double>());
    static_assert(!all_integral<float>());

    assert(sum_checked(1, 2, 3, 4) == 10);
    assert(sum_checked(10, 20, 30) == 60);
    assert(sum_checked(100) == 100);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
