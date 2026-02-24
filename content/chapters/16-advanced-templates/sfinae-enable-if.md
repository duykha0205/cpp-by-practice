# SFINAE & Enable If

SFINAE — Substitution Failure Is Not An Error — is a cornerstone of C++ template metaprogramming. When the compiler tries to substitute template arguments and the resulting code is ill-formed, it silently discards that overload instead of producing a hard error. This mechanism, combined with `std::enable_if` and `std::void_t`, gives you precise compile-time control over which function or class template gets selected.

## SFINAE Concept

During overload resolution, the compiler substitutes template arguments into each candidate. If substitution produces an invalid type or expression in the **immediate context** (return type, parameter types, or template argument list), that candidate is simply removed from the overload set rather than causing a compilation failure.

### How it works

```cpp
#include <iostream>
#include <type_traits>

// This overload is valid only when T has a nested type "value_type"
template<typename T>
typename T::value_type get_first(const T& container) {
    return *container.begin();
}

// Fallback for types without value_type
template<typename T>
T get_first(T value) {
    return value;
}

int main() {
    std::vector<int> v = {10, 20, 30};
    std::cout << get_first(v)  << std::endl;  // 10 — first overload
    std::cout << get_first(42) << std::endl;  // 42 — second overload (int has no value_type)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| SFINAE | Substitution Failure Is Not An Error |
| Immediate context | Return type, parameter types, template argument defaults |
| Hard error vs SFINAE | Errors inside function bodies are hard errors, not SFINAE |
| Overload set | Compiler tries all candidates; ill-formed ones are silently dropped |
| Fallback overload | A less-specialized overload that catches remaining types |

### Practice

1. 🌟🌟🌟 **Fill:** Complete the SFINAE-based overloads. The first should work for types that have a `size()` member function, returning the size. The second is a fallback that returns -1 for types without `size()`.

```cpp,editable
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <cassert>

// Overload for types with .size() — returns the size as int
template<typename T>
auto get_size(const T& val) -> decltype(__, int()) {
    return static_cast<int>(val.size());
}

// Fallback for types without .size()
int get_size(...) {
    return -1;
}

int main() {
    std::vector<int> v = {1, 2, 3};
    std::string s = "hello";

    assert(get_size(v) == 3);
    assert(get_size(s) == 5);
    assert(get_size(42) == -1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::enable_if Patterns

`std::enable_if<condition, T>` defines a member type `type` equal to `T` when `condition` is `true`. When `condition` is `false`, `type` does not exist, causing a substitution failure. This can be placed in the return type, as a template parameter default, or as a function parameter default.

### How it works

```cpp
#include <iostream>
#include <type_traits>

// Pattern 1: enable_if in return type
template<typename T>
std::enable_if_t<std::is_integral_v<T>, std::string>
type_name(T) {
    return "integral";
}

// Pattern 2: enable_if as default template parameter
template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
std::string type_name(T) {
    return "floating-point";
}

int main() {
    std::cout << type_name(42)   << std::endl;  // integral
    std::cout << type_name(3.14) << std::endl;  // floating-point
    return 0;
}
```

| Pattern | Syntax |
|---------|--------|
| Return type | `std::enable_if_t<cond, ReturnType> func(...)` |
| Template parameter | `template<typename T, typename = std::enable_if_t<cond>>` |
| `enable_if_t` | Shorthand for `typename std::enable_if<cond, T>::type` |
| Multiple overloads | Each overload has a mutually exclusive condition |

### Practice

2. 🌟🌟🌟 **Fill:** Complete two overloads of `to_string_value` using `enable_if_t` in the return type. The integral version should use `std::to_string`, and the floating-point version should format with a fixed precision message.

```cpp,editable
#include <iostream>
#include <string>
#include <type_traits>
#include <cassert>

// For integral types: return std::to_string(value)
template<typename T>
__ to_string_value(T value) {
    return std::to_string(value);
}

// For floating-point types: return "float:" + std::to_string(value)
template<typename T>
__ to_string_value(T value) {
    return "float:" + std::to_string(value);
}

int main() {
    std::string int_result = to_string_value(42);
    assert(int_result == "42");

    std::string dbl_result = to_string_value(3.14);
    assert(dbl_result.substr(0, 6) == "float:");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## void_t and Detection Idiom

`std::void_t<...>` maps any set of types to `void`. If any of the types in the parameter pack are ill-formed, the entire specialization is discarded via SFINAE. This makes it easy to check whether a type has a specific member type, member function, or supports a particular operation.

### How it works

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

// Primary template: T does not have a value_type
template<typename T, typename = void>
struct has_value_type : std::false_type {};

// Specialization: T has a value_type member
template<typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};

int main() {
    std::cout << has_value_type<std::vector<int>>::value << std::endl;  // 1
    std::cout << has_value_type<int>::value               << std::endl;  // 0
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::void_t<Ts...>` | Always `void` if all `Ts` are valid types; otherwise SFINAE |
| Primary template | Inherits `std::false_type` (default: trait is false) |
| Specialization | Uses `void_t` to check validity; inherits `std::true_type` |
| Detection idiom | General pattern for checking member types, functions, or operators |

### Practice

3. 🌟🌟🌟 **Fill:** Complete the `has_push_back` trait that detects whether a type has a `push_back` member function. Use `std::void_t` with `decltype` to check for the member.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <cassert>

// Primary template: does not have push_back
template<typename T, typename = void>
struct has_push_back : std::false_type {};

// Specialization: has push_back
template<typename T>
struct has_push_back<T, __> : std::true_type {};

int main() {
    static_assert(has_push_back<std::vector<int>>::value, "vector has push_back");
    static_assert(has_push_back<std::string>::value, "string has push_back");
    static_assert(!has_push_back<int>::value, "int has no push_back");
    static_assert(!has_push_back<double>::value, "double has no push_back");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Tag Dispatch

Tag dispatch selects an overload by passing a **tag type** — typically `std::true_type` or `std::false_type` — as an extra argument. The compiler picks the matching overload at compile time based on the tag, avoiding the complexity of `enable_if` for simple binary choices.

### How it works

```cpp
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <cstring>

// Implementation for trivially copyable types (fast memcpy)
template<typename T>
void fast_copy_impl(const T* src, T* dst, std::size_t n, std::true_type) {
    std::memcpy(dst, src, n * sizeof(T));
    std::cout << "memcpy path" << std::endl;
}

// Implementation for non-trivially copyable types (element-by-element)
template<typename T>
void fast_copy_impl(const T* src, T* dst, std::size_t n, std::false_type) {
    std::copy(src, src + n, dst);
    std::cout << "std::copy path" << std::endl;
}

template<typename T>
void fast_copy(const T* src, T* dst, std::size_t n) {
    fast_copy_impl(src, dst, n, std::is_trivially_copyable<T>{});
}

int main() {
    int a[] = {1, 2, 3};
    int b[3];
    fast_copy(a, b, 3);  // memcpy path
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Tag type | A type used only for overload selection (carries no data) |
| `std::true_type` / `std::false_type` | Standard tag types with a `value` member |
| `Trait{}` | Creates an instance of the tag — `std::is_integral<T>{}` is a `true_type` or `false_type` |
| Dispatch function | Public function that forwards to the correct `_impl` |

### Practice

4. 🌟🌟🌟 **Complete:** Implement tag dispatch for a `process` function. For integral types it should return the value doubled, and for floating-point types it should return the value halved. Write the two `_impl` overloads and the dispatch function.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

// Implementation for integral types: return value * 2
template<typename T>
T process_impl(T value, __) {
    return value * 2;
}

// Implementation for floating-point types: return value / 2
template<typename T>
T process_impl(T value, __) {
    return value / 2.0;
}

// Dispatch function
template<typename T>
T process(T value) {
    return process_impl(value, __);
}

int main() {
    assert(process(5) == 10);
    assert(process(10) == 20);

    double result = process(6.0);
    assert(result == 3.0);

    float fresult = process(8.0f);
    assert(fresult == 4.0f);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Combine `void_t` detection with tag dispatch. Write a `has_size` trait using `void_t`, then use tag dispatch to call `.size()` on containers or return `0` for non-containers.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <vector>
#include <string>
#include <cassert>

// Detection trait: does T have a .size() member?
template<typename T, typename = void>
struct has_size : std::false_type {};

template<typename T>
struct has_size<T, __> : std::true_type {};

// Implementation when T has .size()
template<typename T>
std::size_t get_length_impl(const T& val, std::true_type) {
    return __;
}

// Implementation when T does not have .size()
template<typename T>
std::size_t get_length_impl(const T&, std::false_type) {
    return __;
}

// Dispatch function
template<typename T>
std::size_t get_length(const T& val) {
    return get_length_impl(val, __);
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::string s = "hello";

    assert(get_length(v) == 5);
    assert(get_length(s) == 5);
    assert(get_length(42) == 0);
    assert(get_length(3.14) == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
