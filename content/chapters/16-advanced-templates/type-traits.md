# Type Traits

Type traits are compile-time introspection tools in `<type_traits>` that let you query and transform types. They form the foundation of template metaprogramming — enabling you to write generic code that adapts its behavior based on type properties without runtime overhead.

## Basic Type Traits

The standard library provides a rich set of type predicates such as `std::is_integral`, `std::is_floating_point`, and `std::is_same`. Each trait is a struct with a static `value` member (`true` or `false`). Since C++17, the `_v` shorthand (e.g., `std::is_integral_v<T>`) avoids typing `::value`.

### How it works

```cpp
#include <iostream>
#include <type_traits>

int main() {
    std::cout << std::is_integral<int>::value       << std::endl;  // 1
    std::cout << std::is_integral_v<double>         << std::endl;  // 0
    std::cout << std::is_floating_point_v<double>   << std::endl;  // 1
    std::cout << std::is_same_v<int, int>           << std::endl;  // 1
    std::cout << std::is_same_v<int, double>        << std::endl;  // 0
    return 0;
}
```

| Trait | What it checks |
|-------|---------------|
| `std::is_integral<T>` | Is `T` an integer type (int, char, bool, long, etc.)? |
| `std::is_floating_point<T>` | Is `T` a floating-point type (float, double, long double)? |
| `std::is_same<T, U>` | Are `T` and `U` exactly the same type? |
| `::value` | Static bool member of each trait |
| `_v` shorthand | `std::is_integral_v<T>` is equivalent to `std::is_integral<T>::value` |

### Practice

1. 🌟🌟 **Fill:** Use `std::is_integral_v` and `std::is_floating_point_v` to complete the compile-time checks.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

int main() {
    // Use the _v shorthand for each trait
    constexpr bool int_is_integral    = __;
    constexpr bool double_is_integral = __;
    constexpr bool double_is_float    = __;
    constexpr bool int_same_as_int    = __;
    constexpr bool int_same_as_long   = __;

    assert(int_is_integral    == true);
    assert(double_is_integral == false);
    assert(double_is_float    == true);
    assert(int_same_as_int    == true);
    assert(int_same_as_long   == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Type Transformations

Beyond querying types, the standard library can **transform** them. `std::remove_reference` strips `&` and `&&`, `std::remove_const` strips `const`, and `std::decay` applies the same transformations the compiler uses when passing by value. Each transformation exposes a `::type` member, with the `_t` shorthand available since C++14.

### How it works

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
void show_type_info() {
    using NoRef   = std::remove_reference_t<T>;
    using NoConst = std::remove_const_t<NoRef>;
    using Decayed = std::decay_t<T>;

    std::cout << "is_same<NoRef, int>:   " << std::is_same_v<NoRef, int>   << std::endl;
    std::cout << "is_same<NoConst, int>: " << std::is_same_v<NoConst, int> << std::endl;
    std::cout << "is_same<Decayed, int>: " << std::is_same_v<Decayed, int> << std::endl;
}

int main() {
    show_type_info<const int&>();  // All print 1
    return 0;
}
```

| Transformation | What it does |
|---------------|-------------|
| `std::remove_reference<T>::type` | Strips `&` and `&&` from `T` |
| `std::remove_const<T>::type` | Strips top-level `const` from `T` |
| `std::decay<T>::type` | Removes ref, const, and array/function-to-pointer conversions |
| `_t` shorthand | `std::remove_reference_t<T>` is equivalent to `typename std::remove_reference<T>::type` |

### Practice

2. 🌟🌟 **Fill:** Apply the correct type transformations so each assertion passes.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

int main() {
    // Remove the reference from int&
    using A = __;
    static_assert(std::is_same_v<A, int>, "A should be int");

    // Remove const from const double
    using B = __;
    static_assert(std::is_same_v<B, double>, "B should be double");

    // Decay const int& to int
    using C = __;
    static_assert(std::is_same_v<C, int>, "C should be int");

    // Decay an array type to a pointer
    using D = __;
    static_assert(std::is_same_v<D, int*>, "D should be int*");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Write a template function `are_same_decayed` that returns `true` if two types are the same after decay. This is useful when you want to compare types ignoring references and const qualifiers.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

template<typename T, typename U>
constexpr bool are_same_decayed() {
    return __;
}

int main() {
    static_assert(are_same_decayed<int, int>());
    static_assert(are_same_decayed<int&, int>());
    static_assert(are_same_decayed<const int&, int>());
    static_assert(are_same_decayed<int&&, int>());
    static_assert(!are_same_decayed<int, double>());

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Using Type Traits to Constrain Templates

Type traits become truly powerful when used to **constrain** templates. `static_assert` can reject invalid instantiations with a clear error message, while `std::enable_if` can selectively enable or disable overloads based on type properties.

### How it works

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
T safe_divide(T a, T b) {
    static_assert(std::is_arithmetic_v<T>,
                  "safe_divide requires an arithmetic type");
    return a / b;
}

// Only enabled for integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>, bool>
is_even(T value) {
    return value % 2 == 0;
}

int main() {
    std::cout << safe_divide(10, 3)   << std::endl;  // 3
    std::cout << safe_divide(10.0, 3.0) << std::endl;  // 3.33333
    std::cout << is_even(4) << std::endl;  // 1
    std::cout << is_even(7) << std::endl;  // 0
    // safe_divide(std::string("a"), std::string("b"));  // compile error
    // is_even(3.14);  // compile error: not integral
    return 0;
}
```

| Technique | Use case |
|-----------|---------|
| `static_assert(trait, msg)` | Hard error with a clear message if trait is false |
| `std::enable_if_t<cond, T>` | Returns `T` if `cond` is true; otherwise substitution failure |
| Return type SFINAE | `std::enable_if_t<cond, ReturnType>` as the return type |
| Template parameter SFINAE | `typename = std::enable_if_t<cond>` as a default template argument |

### Practice

4. 🌟🌟🌟 **Fill:** Add a `static_assert` that ensures `T` is an arithmetic type, and complete the `enable_if` return type so `absolute` only works for signed types.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

template<typename T>
T safe_multiply(T a, T b) {
    // Add a static_assert: T must be arithmetic
    __;
    return a * b;
}

template<typename T>
__ absolute(T value) {
    return value < 0 ? -value : value;
}

int main() {
    assert(safe_multiply(3, 4) == 12);
    assert(safe_multiply(2.5, 4.0) == 10.0);

    assert(absolute(-5) == 5);
    assert(absolute(-3.14) == 3.14);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Write two overloads of `describe` using `enable_if`: one for integral types that prints "integer", and one for floating-point types that prints "floating-point". Both should return a `std::string`.

```cpp,editable
#include <iostream>
#include <string>
#include <type_traits>
#include <cassert>

// Overload for integral types — return "integer"
template<typename T>
__ describe(T) {
    return "integer";
}

// Overload for floating-point types — return "floating-point"
template<typename T>
__ describe(T) {
    return "floating-point";
}

int main() {
    assert(describe(42) == "integer");
    assert(describe(true) == "integer");
    assert(describe(3.14) == "floating-point");
    assert(describe(2.0f) == "floating-point");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
