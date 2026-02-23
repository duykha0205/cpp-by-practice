# Auto & Decltype

`auto` and `decltype` are type-deduction tools that let the compiler figure out types for you. They reduce verbosity, prevent type mismatches, and are essential for working with complex template expressions.

## Auto

`auto` deduces a variable's type from its initialiser. It works for any type the compiler can infer: integers, doubles, strings, iterators, lambdas, and more.

### How it works

```cpp
#include <iostream>

int main() {
    auto a = 42;        // int
    auto b = 3.14;      // double
    auto c = 'x';       // char
    auto d = "hello";   // const char*

    std::cout << sizeof(a) << std::endl;  // sizeof(int)
    std::cout << sizeof(b) << std::endl;  // sizeof(double)
    std::cout << sizeof(c) << std::endl;  // sizeof(char)
    return 0;
}
```

| Initialiser | Deduced type |
|-------------|-------------|
| `42` | `int` |
| `3.14` | `double` |
| `'x'` | `char` |
| `"hello"` | `const char*` |
| `{1,2,3}` | `std::initializer_list<int>` (with `auto`) |

### Practice

1. 🌟 **Fill:** What types does `auto` deduce? Fill in the `sizeof` comparisons.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    auto a = 42;
    auto b = 3.14;
    auto c = 'x';

    assert(sizeof(b) == sizeof(__));
    assert(sizeof(c) == sizeof(__));

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Auto with References and Const

`auto` **drops top-level `const` and references** by default. To preserve them, you must write `auto&`, `const auto&`, or `auto&&` explicitly.

### How it works

```cpp
#include <iostream>

int main() {
    int x = 10;

    auto a = x;          // int (copy)
    a = 100;
    std::cout << x << std::endl;  // 10 — x unchanged

    auto& b = x;         // int& (reference)
    b = 200;
    std::cout << x << std::endl;  // 200 — x changed

    const auto& c = x;   // const int& (read-only reference)
    // c = 300;           // ERROR: c is const
    return 0;
}
```

| Declaration | Deduced as | Modifies original? |
|-------------|-----------|-------------------|
| `auto a = x` | `int` (copy) | No |
| `auto& b = x` | `int&` (reference) | Yes |
| `const auto& c = x` | `const int&` | No (read-only) |

### Practice

2. 🌟🌟 **Fill:** After `auto a = x; a = 100;`, does `x` change? After `auto& c = x; c = 200;`, does `x` change?

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10;

    auto a = x;
    a = 100;
    assert(x == __);  // did x change?

    auto& c = x;
    c = 200;
    assert(x == __);  // did x change?

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Decltype

`decltype` inspects the declared type of an expression **without evaluating it**. Unlike `auto`, it preserves references and const qualifiers exactly.

### How it works

```cpp
#include <iostream>

int main() {
    int x = 10;
    double y = 3.14;

    decltype(x) a = 5;          // int
    decltype(x + y) b = 1.5;    // double (int + double → double)
    decltype((x)) c = x;        // int& (parenthesised lvalue → reference)

    std::cout << sizeof(a) << " " << sizeof(b) << std::endl;
    return 0;
}
```

| Expression | `decltype` result |
|------------|------------------|
| `decltype(x)` where `x` is `int` | `int` |
| `decltype(x + y)` where `y` is `double` | `double` (arithmetic promotion) |
| `decltype((x))` | `int&` (parenthesised lvalue gives a reference) |

### Practice

3. 🌟🌟 **Fill:** `decltype(x + y)` where `x` is `int` and `y` is `double` — what is the resulting type's size?

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10;
    double y = 3.14;

    decltype(x + y) c = 1.5;
    assert(sizeof(c) == sizeof(__));

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Trailing Return Type

When a function's return type depends on its parameters, you can use a **trailing return type**: `auto f(params) -> ReturnType`. Combined with `decltype`, this lets the compiler deduce the correct return type from the expression.

### How it works

```cpp
#include <iostream>

template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

int main() {
    auto r1 = add(1, 2.5);      // double
    auto r2 = add(3, 4);        // int
    std::cout << r1 << std::endl;  // 3.5
    std::cout << r2 << std::endl;  // 7
    return 0;
}
```

| Syntax | Meaning |
|--------|---------|
| `auto f(params) -> Type` | Trailing return type |
| `-> decltype(a + b)` | Return type is whatever `a + b` produces |
| Why trailing? | `a` and `b` aren't in scope before the parameter list |

### Practice

4. 🌟🌟🌟 **Fill:** Add the trailing return type using `decltype(a + b)`.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename T1, typename T2>
auto add(T1 a, T2 b) -> __ {
    return a + b;
}

int main() {
    auto r1 = add(1, 2.5);
    assert(r1 == 3.5);

    auto r2 = add(3, 4);
    assert(r2 == 7);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
