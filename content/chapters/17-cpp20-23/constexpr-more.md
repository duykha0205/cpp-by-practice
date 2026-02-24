# Advanced Constexpr & Compile-Time Programming

C++20 greatly expands what you can do at compile time. Alongside the broadened `constexpr`, two new keywords arrive: `consteval` (guaranteed compile-time evaluation) and `constinit` (guaranteed constant initialization). Together they give you fine-grained control over when and how computations happen.

> **Note:** Requires GCC 10+ or Clang 10+ with `-std=c++20`. MSVC 19.29+ supports these features with `/std:c++20`.

## Constexpr Classes

In C++20, most language features are allowed inside `constexpr` functions and constructors — including dynamic allocation (within limits), `try`/`catch`, and virtual functions. This means entire classes can be constructed and used at compile time.

### How it works

```cpp
#include <iostream>
#include <array>

class Vec2 {
    double x_, y_;
public:
    constexpr Vec2(double x, double y) : x_(x), y_(y) {}
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }
    constexpr double dot(const Vec2& other) const {
        return x_ * other.x_ + y_ * other.y_;
    }
    constexpr Vec2 operator+(const Vec2& other) const {
        return Vec2(x_ + other.x_, y_ + other.y_);
    }
    constexpr bool operator==(const Vec2& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
};

int main() {
    // All computed at compile time
    constexpr Vec2 a(3.0, 4.0);
    constexpr Vec2 b(1.0, 2.0);
    constexpr Vec2 c = a + b;
    constexpr double d = a.dot(b);

    static_assert(c == Vec2(4.0, 6.0));
    static_assert(d == 11.0);  // 3*1 + 4*2

    std::cout << "c = (" << c.x() << ", " << c.y() << ")" << std::endl;
    std::cout << "dot = " << d << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `constexpr` constructor | Allows object creation at compile time |
| `constexpr` member function | Can be evaluated at compile time if all inputs are `constexpr` |
| `static_assert` | Compile-time check — proves the computation happened at compile time |
| C++20 additions | `constexpr` now supports `virtual`, `dynamic_cast`, `try`/`catch`, and more |

### Practice

1. 🌟🌟 **Fill:** Complete the `constexpr` class `Fraction` so that addition and equality work at compile time.

```cpp,editable
#include <iostream>
#include <cassert>

class Fraction {
    int num_, den_;
public:
    constexpr Fraction(int num, int den) : num_(num), den_(den) {}
    constexpr int num() const { return num_; }
    constexpr int den() const { return den_; }

    constexpr Fraction operator+(const Fraction& other) const {
        // a/b + c/d = (a*d + c*b) / (b*d)
        return Fraction(__,  __);
    }

    constexpr bool operator==(const Fraction& other) const {
        // Cross-multiply: a/b == c/d iff a*d == c*b
        return __;
    }
};

int main() {
    constexpr Fraction a(1, 2);  // 1/2
    constexpr Fraction b(1, 3);  // 1/3
    constexpr Fraction c = a + b;

    // 1/2 + 1/3 = (1*3 + 1*2) / (2*3) = 5/6
    static_assert(c.num() == 5);
    static_assert(c.den() == 6);
    static_assert(c == Fraction(5, 6));

    // Also works at runtime
    Fraction x(2, 5);
    Fraction y(3, 5);
    Fraction z = x + y;
    assert(z == Fraction(25, 25));  // 2*5+3*5=25, 5*5=25, equivalent to 1/1

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Fill:** Complete the `constexpr` function in the `Matrix2x2` class to multiply two matrices at compile time.

```cpp,editable
#include <iostream>
#include <cassert>

struct Matrix2x2 {
    double m[2][2];

    constexpr Matrix2x2(double a, double b, double c, double d)
        : m{{a, b}, {c, d}} {}

    constexpr double at(int r, int c) const { return m[r][c]; }

    constexpr Matrix2x2 operator*(const Matrix2x2& other) const {
        return Matrix2x2(
            __,  // row 0, col 0
            __,  // row 0, col 1
            __,  // row 1, col 0
            __   // row 1, col 1
        );
    }

    constexpr bool operator==(const Matrix2x2& other) const {
        return m[0][0] == other.m[0][0] && m[0][1] == other.m[0][1] &&
               m[1][0] == other.m[1][0] && m[1][1] == other.m[1][1];
    }
};

int main() {
    // | 1 2 |   | 5 6 |   | 1*5+2*7  1*6+2*8 |   | 19 22 |
    // | 3 4 | * | 7 8 | = | 3*5+4*7  3*6+4*8 | = | 43 50 |
    constexpr Matrix2x2 a(1, 2, 3, 4);
    constexpr Matrix2x2 b(5, 6, 7, 8);
    constexpr Matrix2x2 c = a * b;

    static_assert(c.at(0, 0) == 19);
    static_assert(c.at(0, 1) == 22);
    static_assert(c.at(1, 0) == 43);
    static_assert(c.at(1, 1) == 50);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Consteval Immediate Functions

`consteval` (C++20) marks a function as an **immediate function** — it **must** be evaluated at compile time. Unlike `constexpr` (which *may* run at compile time), a `consteval` function that cannot be evaluated at compile time is a **compile error**. Use it when you want to guarantee compile-time computation.

### How it works

```cpp
#include <iostream>

// consteval: MUST be evaluated at compile time
consteval int square(int n) {
    return n * n;
}

// constexpr: CAN be evaluated at compile time
constexpr int cube(int n) {
    return n * n * n;
}

int main() {
    constexpr int a = square(5);    // OK: compile-time
    static_assert(a == 25);

    constexpr int b = cube(3);      // OK: compile-time
    static_assert(b == 27);

    int x = 5;
    // int c = square(x);  // ERROR: x is not a compile-time constant
    int d = cube(x);       // OK: constexpr can also run at runtime
    std::cout << "d = " << d << std::endl;  // 125

    return 0;
}
```

| Keyword | Evaluated at | Error if runtime? |
|---------|-------------|-------------------|
| `constexpr` | Compile time if possible, runtime otherwise | No |
| `consteval` | **Always** compile time | Yes — compile error |
| `inline` | Runtime (but may be inlined) | No |

### Practice

3. 🌟🌟 **Fix:** The function `factorial` is marked `consteval` but is being called with a runtime variable. Fix the code so it compiles.

```cpp,editable
#include <iostream>
#include <cassert>

consteval long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    // BUG: 'n' is a runtime variable, but factorial is consteval
    int n = 10;
    long long result = factorial(n);

    assert(result == 3628800);

    static_assert(factorial(5) == 120);
    static_assert(factorial(0) == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟🌟 **Complete:** Write a `consteval` function `compile_time_hash` that computes a simple hash of a string literal at compile time using the djb2 algorithm.

```cpp,editable
#include <iostream>
#include <cassert>

consteval unsigned long long compile_time_hash(const char* str) {
    __;  // Implement djb2 hash:
         // Start with hash = 5381
         // For each character: hash = hash * 33 + character
         // Return the final hash
}

int main() {
    constexpr auto h1 = compile_time_hash("hello");
    constexpr auto h2 = compile_time_hash("hello");
    constexpr auto h3 = compile_time_hash("world");

    // Same string always produces the same hash
    static_assert(h1 == h2);

    // Different strings (almost certainly) produce different hashes
    static_assert(h1 != h3);

    // Verify a known djb2 value for "hello"
    // djb2("hello") = 210714636441
    static_assert(compile_time_hash("hello") == 210714636441ULL);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Constinit

`constinit` (C++20) ensures a variable is **initialized at compile time**, preventing the "static initialization order fiasco" (where global variables in different translation units have unpredictable initialization order). Unlike `constexpr`, `constinit` variables can be modified after initialization.

### How it works

```cpp
#include <iostream>

// constinit: initialized at compile time, but mutable afterward
constinit int global_count = 42;

// constinit with constexpr initializer function
constexpr int compute_initial() { return 100; }
constinit int config_value = compute_initial();

int main() {
    std::cout << global_count << std::endl;   // 42
    global_count = 99;                        // OK: constinit allows mutation
    std::cout << global_count << std::endl;   // 99

    std::cout << config_value << std::endl;   // 100
    config_value += 50;                       // OK
    std::cout << config_value << std::endl;   // 150
    return 0;
}
```

| Keyword | Compile-time init? | Mutable after init? | Scope |
|---------|-------------------|--------------------|----|
| `constexpr` | Yes | No (const) | Any |
| `constinit` | Yes | Yes | Static/thread-local only |
| `const` | Not guaranteed | No | Any |
| None | Not guaranteed | Yes | Any |

### Practice

5. 🌟🌟🌟 **Fix:** The code has two problems: (1) a global variable that should use `constinit` to guarantee compile-time initialization, and (2) a `constexpr` variable that needs to be modified later. Fix both.

```cpp,editable
#include <iostream>
#include <cassert>

constexpr int compute_default() { return 256; }

// BUG 1: This should be constinit to ensure compile-time initialization
// but still allow modification at runtime
constexpr int buffer_size = compute_default();

// BUG 2: This global counter should also be constinit (it needs
// compile-time init but must be modifiable)
constexpr int request_count = 0;

void handle_request() {
    request_count++;  // needs to modify this
}

int main() {
    assert(buffer_size == 256);
    buffer_size = 512;  // needs to modify this
    assert(buffer_size == 512);

    handle_request();
    handle_request();
    handle_request();
    assert(request_count == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
