# Template Specialization & SFINAE

Template specialization lets you provide custom implementations for specific types, while **SFINAE** (Substitution Failure Is Not An Error) controls which templates participate in overload resolution. Modern C++17 offers `if constexpr` as a cleaner alternative for many SFINAE patterns.

## Partial Specialization

**Partial specialization** provides a different template implementation when some (but not all) template arguments match a pattern. A common use is specialising a class template for pointer types, reference types, or specific combinations of arguments.

### How it works

```cpp
#include <iostream>
#include <string>

// Primary template
template<typename T>
class TypeInfo {
public:
    static std::string describe() { return "general type"; }
};

// Full specialization for int
template<>
class TypeInfo<int> {
public:
    static std::string describe() { return "int"; }
};

// Partial specialization for any pointer type
template<typename T>
class TypeInfo<T*> {
public:
    static std::string describe() {
        return "pointer to " + TypeInfo<T>::describe();
    }
};

int main() {
    std::cout << TypeInfo<double>::describe() << std::endl;   // general type
    std::cout << TypeInfo<int>::describe() << std::endl;      // int
    std::cout << TypeInfo<int*>::describe() << std::endl;     // pointer to int
    std::cout << TypeInfo<double*>::describe() << std::endl;  // pointer to general type
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Primary template | The generic version: `template<typename T> class X { ... }` |
| Full specialization | `template<> class X<int> { ... }` — one specific type |
| Partial specialization | `template<typename T> class X<T*> { ... }` — a pattern of types |
| Matching order | Compiler picks the most specific specialization that matches |

### Practice

1. 🌟🌟 **Fill:** Write a partial specialization of `Storage` for pointer types that dereferences the pointer to return the value.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

// Primary template: stores a value directly
template<typename T>
class Storage {
    T value_;
public:
    Storage(T val) : value_(val) {}
    T get() const { return value_; }
};

// Partial specialization for pointer types
template<typename T>
class Storage<T*> {
    T* ptr_;
public:
    Storage(T* p) : ptr_(p) {}
    __ // implement get(): dereference ptr_ and return the value (type T)
};

int main() {
    Storage<int> s1(42);
    assert(s1.get() == 42);

    int x = 99;
    Storage<int*> s2(&x);
    assert(s2.get() == 99);

    x = 200;
    assert(s2.get() == 200);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟🌟 **Complete:** Write a full specialization of `Formatter` for `bool` that returns `"true"` or `"false"`, and a partial specialization for pointer types that shows the pointed-to value.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

// Primary template
template<typename T>
class Formatter {
public:
    static std::string format(const T& val) {
        return std::to_string(val);
    }
};

// Full specialization for bool
__  // template<> class Formatter<bool> with format() returning "true" or "false"

// Partial specialization for pointer types
__  // template<typename T> class Formatter<T*> with format() that dereferences and delegates to Formatter<T>

int main() {
    assert(Formatter<int>::format(42) == "42");
    assert(Formatter<double>::format(3.140000) == std::to_string(3.14));

    assert(Formatter<bool>::format(true) == "true");
    assert(Formatter<bool>::format(false) == "false");

    int x = 77;
    assert(Formatter<int*>::format(&x) == "77");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## SFINAE Basics

**Substitution Failure Is Not An Error** means that when the compiler tries to substitute template arguments and the result is invalid, that particular overload is silently removed from consideration instead of causing an error. `std::enable_if` is the classic tool for leveraging SFINAE.

### How it works

```cpp
#include <iostream>
#include <type_traits>

// Only enabled when T is an integral type
template<typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
classify(T) {
    return "integer";
}

// Only enabled when T is a floating-point type
template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
classify(T) {
    return "floating-point";
}

int main() {
    std::cout << classify(42)    << std::endl;  // integer
    std::cout << classify(3.14)  << std::endl;  // floating-point
    // classify("hello");  // ERROR: no matching overload
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| SFINAE | Invalid substitution removes the overload, not a hard error |
| `std::enable_if<cond, T>` | Defines `::type` as `T` only when `cond` is true |
| `std::is_integral<T>` | Type trait — true for int, long, char, bool, etc. |
| `std::is_floating_point<T>` | Type trait — true for float, double, long double |

### Practice

3. 🌟🌟🌟 **Fill:** Use `std::enable_if` to create an `absolute` function that works for both integral and floating-point types but with different implementations.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>
#include <cmath>

// For integral types: use conditional negation
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
absolute(T val) {
    __ // return val < 0 ? -val : val
}

// For floating-point types: use std::fabs
template<typename T>
__ // enable_if for floating-point types, return type T
absolute(T val) {
    return std::fabs(val);
}

int main() {
    assert(absolute(-5) == 5);
    assert(absolute(3) == 3);
    assert(absolute(0) == 0);

    assert(std::abs(absolute(-3.14) - 3.14) < 0.001);
    assert(std::abs(absolute(2.5) - 2.5) < 0.001);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## if constexpr as Modern Alternative

C++17 introduced `if constexpr`, which evaluates conditions at **compile time** inside templates. The false branch is discarded entirely, so it does not need to be valid for the given type. This replaces many SFINAE patterns with straightforward, readable code.

### How it works

```cpp
#include <iostream>
#include <type_traits>
#include <string>

template<typename T>
std::string classify(T val) {
    if constexpr (std::is_integral_v<T>) {
        return "integer: " + std::to_string(val);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "float: " + std::to_string(val);
    } else {
        return "other";
    }
}

int main() {
    std::cout << classify(42)    << std::endl;  // integer: 42
    std::cout << classify(3.14)  << std::endl;  // float: 3.140000
    std::cout << classify("hi")  << std::endl;  // other
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `if constexpr (cond)` | Condition evaluated at compile time |
| Discarded branch | The false branch is **not instantiated** — may contain invalid code for `T` |
| `std::is_integral_v<T>` | C++17 shorthand for `std::is_integral<T>::value` |
| Replaces SFINAE | One function body instead of multiple `enable_if` overloads |

### Practice

4. 🌟🌟 **Fill:** Use `if constexpr` to write a single `to_debug_string` function that handles integers, floating-point numbers, and `std::string` differently.

```cpp,editable
#include <iostream>
#include <string>
#include <type_traits>
#include <cassert>

template<typename T>
std::string to_debug_string(const T& val) {
    if constexpr (__) {  // check if T is integral
        return "int:" + std::to_string(val);
    } else if constexpr (__) {  // check if T is floating-point
        return "float:" + std::to_string(val);
    } else if constexpr (std::is_same_v<T, std::string>) {
        return "str:" + val;
    } else {
        return "unknown";
    }
}

int main() {
    assert(to_debug_string(42) == "int:42");
    assert(to_debug_string(std::string("hi")) == "str:hi");

    std::string flt = to_debug_string(3.14);
    assert(flt.substr(0, 6) == "float:");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Use `if constexpr` to write a `container_sum` function that works for both arrays (using a size parameter) and standard containers (using `.begin()` / `.end()`). Use `std::is_array_v` to distinguish.

```cpp,editable
#include <iostream>
#include <vector>
#include <type_traits>
#include <numeric>
#include <cassert>

template<typename T>
auto container_sum(const T& container) {
    if constexpr (std::is_array_v<T>) {
        // For C-style arrays: use std::accumulate with std::begin/std::end
        __ // return std::accumulate(std::begin(container), std::end(container), 0);
    } else {
        // For STL containers: use .begin() / .end()
        using ValueType = typename T::value_type;
        __ // return std::accumulate(container.begin(), container.end(), ValueType{});
    }
}

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    assert(container_sum(arr) == 15);

    std::vector<int> vec = {10, 20, 30};
    assert(container_sum(vec) == 60);

    std::vector<double> dvec = {1.5, 2.5, 3.0};
    assert(std::abs(container_sum(dvec) - 7.0) < 0.001);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
