# Concepts (C++20)

Concepts are named **compile-time predicates** that constrain template parameters. They replace the cryptic SFINAE patterns of old with clear, readable requirements. When a concept is not satisfied, the compiler produces a clean error message pointing directly at the violated constraint.

## Requires Clause

The `requires` clause constrains a template so it only accepts types meeting certain conditions. C++20 provides standard concepts in `<concepts>` like `std::integral`, `std::floating_point`, `std::same_as`, and `std::convertible_to`.

### How it works

```cpp
#include <iostream>
#include <concepts>

// Only accept integral types
template <std::integral T>
T add(T a, T b) {
    return a + b;
}

// Using requires clause explicitly
template <typename T>
    requires std::floating_point<T>
T multiply(T a, T b) {
    return a * b;
}

// Trailing requires clause
template <typename T>
T negate(T x) requires std::signed_integral<T> {
    return -x;
}

int main() {
    std::cout << add(3, 4) << std::endl;          // 7 (int is integral)
    std::cout << multiply(2.5, 3.0) << std::endl; // 7.5 (double is floating_point)
    std::cout << negate(-5) << std::endl;          // 5
    // add(3.14, 2.0);  // ERROR: double is not integral
    return 0;
}
```

| Syntax | Where Constraint Goes |
|--------|----------------------|
| `template <std::integral T>` | Constraint in template parameter |
| `requires std::floating_point<T>` | Requires clause after template |
| `T f(T x) requires Concept<T>` | Trailing requires clause |
| Standard concepts | `std::integral`, `std::floating_point`, `std::same_as<T, U>`, `std::convertible_to<From, To>` |

### Practice

1. :star::star: **Fill:** Constrain the template so `square` only works with floating-point types and `absolute` only works with signed integral types.

```cpp,editable
#include <iostream>
#include <concepts>
#include <cassert>

template <__>
T square(T x) {
    return x * x;
}

template <typename T>
    requires __
T absolute(T x) {
    return x < 0 ? -x : x;
}

int main() {
    assert(square(3.0) == 9.0);
    assert(square(2.5f) == 6.25f);
    // square(3);  // Should not compile — int is not floating_point

    assert(absolute(-5) == 5);
    assert(absolute(-100L) == 100L);
    // absolute(3u);  // Should not compile — unsigned is not signed_integral

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Defining Custom Concepts

You can define your own concepts with the `concept` keyword. A concept evaluates to `true` or `false` at compile time. Compound requirements let you check that expressions are valid, that they return specific types, and that nested constraints hold.

### How it works

```cpp
#include <iostream>
#include <concepts>
#include <string>

// Simple concept: T must support + operator returning T
template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> std::convertible_to<T>;
};

// Concept with multiple requirements
template <typename T>
concept Printable = requires(T a, std::ostream& os) {
    { os << a } -> std::same_as<std::ostream&>;
};

// Concept with nested requirement
template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <Addable T>
T sum(T a, T b) {
    return a + b;
}

int main() {
    std::cout << sum(3, 4) << std::endl;
    std::cout << sum(1.5, 2.5) << std::endl;
    std::cout << sum(std::string("hello"), std::string(" world")) << std::endl;
    return 0;
}
```

| Feature | Syntax |
|---------|--------|
| Simple concept | `concept Name = expression;` |
| Requires expression | `requires(T a) { expr; }` |
| Compound requirement | `{ expr } -> ConceptOrType;` |
| Nested requirement | `requires Concept<T>;` |
| Combining concepts | `concept C = C1<T> && C2<T>;` or `C1<T> \|\| C2<T>` |

### Practice

2. :star::star: **Fill:** Define a concept `HasSize` that requires a type to have a `.size()` method returning something convertible to `std::size_t`.

```cpp,editable
#include <iostream>
#include <concepts>
#include <string>
#include <vector>
#include <cassert>

template <typename T>
concept HasSize = requires(T t) {
    __;
};

template <HasSize T>
std::size_t get_size(const T& container) {
    return container.size();
}

int main() {
    std::string s = "hello";
    std::vector<int> v = {1, 2, 3};

    assert(get_size(s) == 5);
    assert(get_size(v) == 3);
    // get_size(42);  // Should not compile — int has no .size()

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. :star::star::star: **Complete:** Define a concept `Arithmetic` that requires a type to support addition, subtraction, multiplication, and comparison with `<`. Then use it to constrain a `clamp` function.

```cpp,editable
#include <iostream>
#include <concepts>
#include <cassert>

template <typename T>
concept Arithmetic = requires(T a, T b) {
    __;
    __;
    __;
    __;
};

template <__>
T clamp(T value, T low, T high) {
    if (value < low) return low;
    if (high < value) return high;
    return value;
}

int main() {
    assert(clamp(5, 1, 10) == 5);
    assert(clamp(-3, 0, 100) == 0);
    assert(clamp(200, 0, 100) == 100);
    assert(clamp(3.14, 0.0, 2.0) == 2.0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Concepts vs SFINAE

Before C++20, templates were constrained using `std::enable_if` (SFINAE) — a technique that was powerful but notoriously unreadable. Concepts replace SFINAE with clear, declarative syntax. C++20 also introduces **abbreviated function templates**, where `auto` parameters constrained by a concept work like concise templates.

### How it works

```cpp
#include <iostream>
#include <concepts>
#include <type_traits>

// OLD: SFINAE with enable_if (hard to read)
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T old_double_it(T x) {
    return x * 2;
}

// NEW: Concept (clear and readable)
template <std::integral T>
T new_double_it(T x) {
    return x * 2;
}

// NEWEST: Abbreviated function template (C++20)
auto abbreviated_double(std::integral auto x) {
    return x * 2;
}

int main() {
    std::cout << old_double_it(5) << std::endl;
    std::cout << new_double_it(5) << std::endl;
    std::cout << abbreviated_double(5) << std::endl;
    return 0;
}
```

| Approach | Syntax | Readability |
|----------|--------|-------------|
| SFINAE | `enable_if_t<is_integral_v<T>>` | Poor — nested templates |
| Concept constraint | `template <std::integral T>` | Good — clear constraint |
| Abbreviated template | `std::integral auto x` | Best — concise and direct |
| Error messages | SFINAE: pages of template errors; Concepts: "constraint not satisfied" |

### Practice

4. :star::star: **Fix:** Replace the SFINAE-based `enable_if` with a clean concept constraint using the abbreviated function template syntax.

```cpp,editable
#include <iostream>
#include <concepts>
#include <type_traits>
#include <cassert>

// BUG: Rewrite using abbreviated function template with concept
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
T triple(T x) {
    return x * 3;
}

int main() {
    assert(triple(5) == 15);
    assert(triple(10) == 30);
    // triple(3.14);  // Should not compile

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Write an abbreviated function template `print_if_positive` that only accepts types satisfying a custom concept `SignedNumber` (must be signed integral or floating point). The function prints the value only if it is positive.

```cpp,editable
#include <iostream>
#include <concepts>
#include <cassert>
#include <sstream>

template <typename T>
concept SignedNumber = __;

auto print_if_positive(__ x) -> std::string {
    if (x > 0) {
        std::ostringstream oss;
        oss << x;
        return oss.str();
    }
    return "not positive";
}

int main() {
    assert(print_if_positive(42) == "42");
    assert(print_if_positive(-5) == "not positive");
    assert(print_if_positive(3.14) == "3.14");
    assert(print_if_positive(0) == "not positive");
    // print_if_positive(5u);  // Should not compile — unsigned

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
