# Function Templates

Function templates let you write a single function that works with any type. The compiler generates the actual function code for each type you use — giving you generic programming without sacrificing type safety or performance.

## Template Basics

A function template begins with `template<typename T>`. The compiler substitutes `T` with the concrete type at each call site, producing a fully typed function.

### How it works

```cpp
#include <iostream>

template<typename T>
T my_max(T a, T b) {
    return (a > b) ? a : b;
}

int main() {
    std::cout << my_max(3, 7)       << std::endl;  // 7   (T = int)
    std::cout << my_max(3.14, 2.72) << std::endl;  // 3.14 (T = double)
    std::cout << my_max('a', 'z')   << std::endl;  // z   (T = char)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<typename T>` | Declares a type parameter `T` |
| `typename` vs `class` | Interchangeable in template parameter lists |
| Instantiation | Compiler generates a concrete function for each unique `T` |

### Practice

1. 🌟 **Fill:** Complete `my_max` so it returns the larger of the two arguments.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename T>
T my_max(T a, T b) {
    return __;
}

int main() {
    assert(my_max(3, 7) == 7);
    assert(my_max(3.14, 2.72) == 3.14);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Type Deduction

When you call a function template, the compiler usually **deduces** `T` from the argument types. However, if arguments have different types the deduction can fail — you need either an explicit template argument or multiple type parameters.

### How it works

```cpp
#include <iostream>

template<typename T>
T add_same(T a, T b) {
    return a + b;
}

template<typename T1, typename T2>
auto add_any(T1 a, T2 b) {
    return a + b;  // return type deduced by auto
}

int main() {
    std::cout << add_same(1, 2)      << std::endl;  // 3 — both int, OK
    // add_same(1, 2.5);  // ERROR: T deduced as int AND double
    std::cout << add_same<double>(1, 2.5) << std::endl;  // 3.5 — explicit T
    std::cout << add_any(1, 2.5)     << std::endl;  // 3.5 — two type params
    return 0;
}
```

| Scenario | Solution |
|----------|----------|
| `add(1, 2)` — same types | Deduction works automatically |
| `add(1, 2.5)` — mixed types, one `T` | Fails: ambiguous deduction |
| `add<double>(1, 2.5)` | Explicit template argument resolves ambiguity |
| Two template params `T1`, `T2` | Each parameter deduced independently |

### Practice

2. 🌟🌟 **Fix:** `add(1, 2.5)` fails because `T` cannot be both `int` and `double`. Change the template to use two type parameters so mixed types work.

```cpp,editable
#include <iostream>
#include <cassert>

// BUG: single T cannot deduce int and double simultaneously
template<typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    auto result = add(1, 2.5);
    assert(result == 3.5);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Multiple Template Parameters

Templates can take more than one type parameter. This is useful for functions that operate on heterogeneous data, such as creating pairs or converting between types.

### How it works

```cpp
#include <iostream>
#include <utility>

template<typename T1, typename T2>
std::pair<T1, T2> make_my_pair(T1 a, T2 b) {
    return {a, b};
}

int main() {
    auto p = make_my_pair(42, 3.14);
    std::cout << p.first << ", " << p.second << std::endl;  // 42, 3.14

    auto q = make_my_pair(std::string("hi"), 100);
    std::cout << q.first << ", " << q.second << std::endl;  // hi, 100
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<typename T1, typename T2>` | Two independent type parameters |
| `std::pair<T1, T2>` | Standard library pair, holds two values |
| Return type | Can use `auto` (C++14+) or explicit `std::pair<T1,T2>` |

### Practice

3. 🌟🌟 **Fill:** Complete `make_pair` to return a `std::pair` containing `a` and `b`.

```cpp,editable
#include <iostream>
#include <utility>
#include <cassert>

template<typename T1, typename T2>
std::pair<T1, T2> make_pair(T1 a, T2 b) {
    return __;
}

int main() {
    auto p = make_pair(42, 3.14);
    assert(p.first == 42);
    assert(p.second == 3.14);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Template Specialization

Sometimes a generic template doesn't work correctly for a specific type. **Template specialization** lets you provide a custom implementation for that type while keeping the general template for everything else.

### How it works

```cpp
#include <iostream>
#include <cstring>

template<typename T>
bool is_equal(T a, T b) {
    return a == b;
}

template<>
bool is_equal<const char*>(const char* a, const char* b) {
    return std::strcmp(a, b) == 0;
}

int main() {
    std::cout << is_equal(1, 1) << std::endl;           // 1 (generic)
    std::cout << is_equal(1, 2) << std::endl;           // 0 (generic)
    std::cout << is_equal("hi", "hi") << std::endl;     // 1 (specialization)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<>` | Introduces a full specialization (no remaining type params) |
| `is_equal<const char*>` | Specialization target type |
| Why needed for `const char*` | `==` compares pointer addresses, not string content |
| `std::strcmp` | Returns 0 when strings are equal |

### Practice

4. 🌟🌟🌟 **Fill:** Add a template specialization of `is_equal` for `const char*` that uses `strcmp` to compare string content.

```cpp,editable
#include <iostream>
#include <cstring>
#include <cassert>

template<typename T>
bool is_equal(T a, T b) {
    return a == b;
}

// Add template specialization for const char*
__

int main() {
    assert(is_equal(1, 1) == true);
    assert(is_equal(1, 2) == false);

    const char* s1 = "hello";
    const char* s2 = "hello";
    const char* s3 = "world";
    assert(is_equal(s1, s2) == true);
    assert(is_equal(s1, s3) == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
