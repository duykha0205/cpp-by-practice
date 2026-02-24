# Type Conversions & Initialization Styles

C++ offers several ways to initialize variables and convert between types. Modern C++ encourages **uniform initialization** with braces `{}` because it prevents accidental data loss (narrowing). Understanding type casts, narrowing rules, and C++17 structured bindings will help you write safer, more expressive code.

## Uniform Initialization

### How it works

Brace initialization (also called uniform initialization) uses `{}` to initialize any type. Its key advantage is that it **rejects narrowing conversions** — the compiler will error if data would be lost.

```cpp
#include <iostream>
#include <cassert>

int main() {
    // Direct brace initialization
    int a{42};
    double pi{3.14159};
    char c{'A'};

    // Copy initialization with braces
    int b = {10};

    // Brace init prevents narrowing — these would NOT compile:
    // int x{3.14};    // ERROR: double -> int is narrowing
    // char ch{300};   // ERROR: 300 doesn't fit in a char

    // But the old style allows it silently:
    int y = 3.14;      // Compiles! y becomes 3, data lost silently

    assert(a == 42);
    assert(b == 10);
    assert(y == 3);  // silent truncation happened

    std::cout << "a=" << a << " b=" << b << " pi=" << pi << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Syntax | Name | Prevents narrowing? |
|--------|------|---------------------|
| `int x{42};` | Direct brace init | Yes |
| `int x = {42};` | Copy brace init | Yes |
| `int x = 42;` | Copy initialization | No |
| `int x(42);` | Direct initialization | No |

### Practice

1\. 🌟 **Fix:** The variables use `=` initialization which allows narrowing. Change each to use brace initialization `{}`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    // Change each to brace initialization: Type name{value};
    int x = 42;
    double pi = 3.14;
    char c = 'A';

    assert(x == 42);
    assert(pi == 3.14);
    assert(c == 'A');

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Type Casting

### How it works

C++ provides `static_cast<T>(expr)` for safe, explicit conversions. The older C-style cast `(T)expr` still works but is dangerous — it can perform casts that `static_cast` would reject, including removing `const` or reinterpreting memory.

```cpp
#include <iostream>
#include <cassert>

int main() {
    double pi = 3.14159;

    // C-style cast — works but unsafe, no compile-time checks
    int truncated_c = (int)pi;

    // static_cast — preferred, intention is clear and checked
    int truncated_s = static_cast<int>(pi);

    assert(truncated_c == 3);
    assert(truncated_s == 3);

    // static_cast for int division to double division
    int a = 7, b = 2;
    double result = static_cast<double>(a) / b;  // 3.5, not 3

    assert(result == 3.5);

    std::cout << "Result: " << result << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Cast style | Syntax | Safety |
|------------|--------|--------|
| `static_cast` | `static_cast<int>(x)` | Compile-time checked, clear intent |
| C-style | `(int)x` | No checks, can do dangerous casts silently |
| `const_cast` | `const_cast<int*>(p)` | Removes `const` — use only when absolutely needed |
| `reinterpret_cast` | `reinterpret_cast<T*>(p)` | Reinterprets memory — very dangerous |

### Practice

2\. 🌟 **Fix:** Replace the C-style cast with `static_cast` to make the conversion explicit and safe.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    double temperature = 36.6;

    // Replace the C-style cast with static_cast
    int rounded_down = (int)temperature;

    assert(rounded_down == 36);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3\. 🌟🌟 **Fill:** Use `static_cast` to fix the integer division so the result is a `double`.

```cpp,editable
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
    int total = 7;
    int count = 2;

    // Without the cast, this does integer division (7/2 = 3)
    // Use static_cast on 'total' so the division produces 3.5
    double average = __ / count;

    assert(std::abs(average - 3.5) < 0.001);

    std::cout << "Average: " << average << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Narrowing Conversions

### How it works

A **narrowing conversion** happens when a value is converted to a type that cannot represent it exactly — for example, `double` to `int` (fractional part lost) or `int` to `char` (value may overflow). Brace initialization `{}` catches these at compile time.

```cpp
#include <iostream>
#include <cassert>

int main() {
    // These are narrowing — brace init would reject them:
    // int a{3.14};       // ERROR: double -> int narrows
    // short s{100000};   // ERROR: value too large for short

    // But = initialization silently allows narrowing:
    int a = 3.14;         // OK but a becomes 3
    short s = 40000;      // may overflow depending on platform

    // Safe: value fits in target type
    int b{42};            // OK: 42 fits in int
    char c{65};           // OK: 65 fits in char (is 'A')

    assert(a == 3);
    assert(b == 42);
    assert(c == 'A');

    std::cout << "Brace init keeps you safe from silent data loss!" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Conversion | Narrowing? | Why |
|------------|-----------|-----|
| `double` to `int` | Yes | Fractional part is lost |
| `int` to `char` | Yes (if value > 127) | Value may not fit |
| `int` to `long` | No | `long` is at least as wide |
| `0` to `bool` | No | `0` and `1` fit in `bool` |

### Practice

4\. 🌟🌟 **Fix:** The code uses `=` initialization which silently narrows. Change it to brace initialization and fix the value so it compiles without narrowing.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    // This silently truncates 3.7 to 3 — bad!
    // Change to brace init {} and fix the value to be a valid int
    int x = 3.7;

    assert(x == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Structured Bindings

### How it works

C++17 structured bindings let you unpack a `std::pair`, `std::tuple`, or struct into named variables in a single declaration using `auto [x, y] = expr;`. This makes code more readable than using `.first` / `.second` or `std::get<>`.

```cpp
#include <iostream>
#include <tuple>
#include <utility>
#include <cassert>

struct Point {
    double x;
    double y;
};

int main() {
    // Structured binding with pair
    std::pair<int, std::string> p{1, "Alice"};
    auto [id, name] = p;

    assert(id == 1);
    assert(name == "Alice");

    // Structured binding with tuple
    std::tuple<int, double, char> t{42, 3.14, 'X'};
    auto [a, b, c] = t;

    assert(a == 42);
    assert(c == 'X');

    // Structured binding with struct
    Point pt{1.5, 2.5};
    auto [px, py] = pt;

    assert(px == 1.5);
    assert(py == 2.5);

    std::cout << "id=" << id << " name=" << name << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Source | Syntax | Unpacks |
|--------|--------|---------|
| `std::pair` | `auto [a, b] = pair;` | `.first`, `.second` |
| `std::tuple` | `auto [a, b, c] = tuple;` | `get<0>`, `get<1>`, `get<2>` |
| Struct | `auto [x, y] = struct;` | Each public member in order |

### Practice

5\. 🌟🌟 **Fill:** Use a structured binding to unpack the pair and the struct.

```cpp,editable
#include <iostream>
#include <utility>
#include <cassert>

struct Color {
    int r;
    int g;
    int b;
};

int main() {
    std::pair<std::string, int> student{"Bob", 95};

    // Unpack the pair using structured binding
    auto [__, __] = student;

    assert(name == "Bob");
    assert(score == 95);

    Color red{255, 0, 0};

    // Unpack the struct using structured binding
    auto [__, __, __] = red;

    assert(r == 255);
    assert(g == 0);
    assert(b == 0);

    std::cout << name << " scored " << score << std::endl;
    std::cout << "Color: (" << r << "," << g << "," << b << ")" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
