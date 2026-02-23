# Numbers

Numbers are the most fundamental data type in programming. C++ offers a rich set of integer and floating-point types with precise control over size, signedness, and precision. Understanding how numbers are stored and how arithmetic works — including overflow and precision pitfalls — is critical for writing correct programs.

## Integer Types

### How it works

C++ provides several integer types with minimum guaranteed sizes. The `unsigned` modifier restricts a type to non-negative values only. For exact-size guarantees, use the fixed-width types from `<cstdint>`.

```cpp
#include <iostream>
#include <cstdint>
#include <cassert>

int main() {
    // Built-in integer types
    short s = 32767;                              // at least 16 bits
    int i = 2'147'483'647;                        // at least 16 bits, typically 32
    long l = 2'147'483'647L;                      // at least 32 bits
    long long ll = 9'000'000'000'000'000'000LL;   // at least 64 bits

    // Signed vs unsigned
    int signed_val = -42;            // can hold negatives
    unsigned int unsigned_val = 42;  // only non-negative (0 and above)

    // Fixed-width types — exact sizes, from <cstdint>
    int8_t  i8  = 127;                            // exactly 1 byte
    int16_t i16 = 32767;                          // exactly 2 bytes
    int32_t i32 = 2'147'483'647;                  // exactly 4 bytes
    int64_t i64 = 9'000'000'000'000'000'000LL;    // exactly 8 bytes

    assert(sizeof(int8_t) == 1);
    assert(sizeof(int16_t) == 2);
    assert(sizeof(int32_t) == 4);
    assert(sizeof(int64_t) == 8);

    std::cout << "signed_val = " << signed_val << std::endl;
    std::cout << "unsigned_val = " << unsigned_val << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Type | Minimum size | Typical size | Range (signed) |
|------|-------------|-------------|----------------|
| `short` | 16 bits | 16 bits | -32 768 to 32 767 |
| `int` | 16 bits | 32 bits | -2.1 billion to 2.1 billion |
| `long` | 32 bits | 32/64 bits | Platform-dependent |
| `long long` | 64 bits | 64 bits | ±9.2 × 10¹⁸ |
| `unsigned` | Same as signed | Same | 0 to 2× the signed max |
| `int8_t` … `int64_t` | Exact | Exact | Guaranteed exact width |

### Practice

1. 🌟 Unsigned integers cannot hold negative values. Fix the code.

```cpp,editable
// Fix the error: unsigned can't hold negative values
#include <iostream>
#include <cassert>

int main() {
    int x = 5;
    unsigned int y = -1;

    assert(x == 5);
    assert(y == 1);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 Fill in the correct sizes for fixed-width integer types.

```cpp,editable
// Fill the blanks with the correct values
#include <iostream>
#include <cassert>
#include <cstdint>

int main() {
    assert(sizeof(int8_t) == __);   // bytes
    assert(sizeof(int16_t) == __);  // bytes
    assert(sizeof(int32_t) == __);  // bytes
    assert(sizeof(int64_t) == __);  // bytes

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Integer Overflow

### How it works

Signed integer overflow is **undefined behavior** — the compiler assumes it never happens and may optimize based on that assumption. Unsigned overflow is well-defined: it wraps around modulo 2ⁿ.

```cpp
#include <iostream>
#include <climits>
#include <cstdint>
#include <cassert>

int main() {
    // Signed overflow is UNDEFINED BEHAVIOR — never do this:
    // int x = INT_MAX;
    // int y = x + 1;  // UB! Anything can happen.

    // Safe: cast to a larger type BEFORE the arithmetic
    long long safe = static_cast<long long>(INT_MAX) + 1;
    assert(safe == 2'147'483'648LL);

    // Unsigned overflow is well-defined: wraps around
    unsigned int u = 0;
    u = u - 1;  // wraps to UINT_MAX
    assert(u == UINT_MAX);

    std::cout << "safe = " << safe << std::endl;
    std::cout << "u (wrapped) = " << u << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Scenario | Behavior |
|----------|----------|
| Signed overflow (`INT_MAX + 1`) | **Undefined behavior** — never rely on it |
| Unsigned overflow (`0u - 1`) | Well-defined — wraps to `UINT_MAX` |
| Prevention | Cast to a larger type **before** the operation |

### Practice

3. 🌟🌟 Signed integer overflow is undefined behavior. Fix the code so `y` holds `INT_MAX + 1` safely.

```cpp,editable
// Fix: cast to a larger type BEFORE the addition
#include <iostream>
#include <climits>
#include <cassert>

int main() {
    int x = INT_MAX;
    long long y = x + 1;  // Still UB! Why?

    assert(y == static_cast<long long>(INT_MAX) + 1);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Number Literals

### How it works

Integer literals can be written in decimal, hexadecimal (prefix `0x`), octal (prefix `0`), or binary (prefix `0b`). Digit separators (`'`) improve readability.

```cpp
#include <iostream>
#include <cassert>

int main() {
    int decimal = 255;
    int hex     = 0xFF;          // base 16
    int octal   = 0377;          // base 8
    int binary  = 0b11111111;    // base 2

    // All represent the same value
    assert(decimal == hex);
    assert(hex == octal);
    assert(octal == binary);
    assert(decimal == 255);

    // Digit separators (C++14) for readability
    int million = 1'000'000;
    assert(million == 1000000);

    std::cout << "All equal 255!" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Prefix | Base | Example | Decimal value |
|--------|------|---------|---------------|
| *(none)* | 10 (decimal) | `255` | 255 |
| `0x` | 16 (hex) | `0xFF` | 255 |
| `0` | 8 (octal) | `0377` | 255 |
| `0b` | 2 (binary) | `0b11111111` | 255 |
| `'` | Digit separator | `1'000'000` | 1000000 |

### Practice

4. 🌟🌟 All these literals represent the same value. Fill the blank.

```cpp,editable
// Fill the blank to make the assert pass
#include <iostream>
#include <cassert>

int main() {
    int decimal = 255;
    int hex = 0xFF;
    int octal = 0377;
    int binary = 0b11111111;

    assert(decimal == hex);
    assert(hex == octal);
    assert(octal == binary);
    assert(decimal == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Floating-Point

### How it works

C++ has three floating-point types: `float` (4 bytes), `double` (8 bytes, the default), and `long double` (platform-dependent, ≥ 8 bytes). Use the `f` suffix for `float` literals and `L` for `long double`.

```cpp
#include <iostream>
#include <cassert>

int main() {
    float f = 3.14f;         // 4 bytes, ~7 decimal digits precision
    double d = 3.14159265;   // 8 bytes, ~15 decimal digits precision
    long double ld = 3.14L;  // ≥8 bytes, platform-dependent

    assert(sizeof(float) == 4);
    assert(sizeof(double) == 8);

    // Without suffix, a literal is double by default
    auto a = 1.0;   // double
    auto b = 1.0f;  // float
    auto c = 1.0L;  // long double

    std::cout << "float: " << sizeof(f) << " bytes" << std::endl;
    std::cout << "double: " << sizeof(d) << " bytes" << std::endl;
    std::cout << "long double: " << sizeof(ld) << " bytes" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Type | Size | Precision | Suffix |
|------|------|-----------|--------|
| `float` | 4 bytes | ~7 digits | `f` |
| `double` | 8 bytes | ~15 digits | *(none — default)* |
| `long double` | ≥ 8 bytes | ≥ 15 digits | `L` |

### Practice

5. 🌟 Fill in the sizes for `float` and `double`.

```cpp,editable
// Fill the blanks
#include <iostream>
#include <cassert>

int main() {
    float f = 3.14f;
    double d = 3.14;
    long double ld = 3.14L;

    assert(sizeof(float) == __);    // bytes
    assert(sizeof(double) == __);   // bytes

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Floating-Point Comparison

### How it works

Floating-point numbers cannot represent all decimal fractions exactly. The classic example: `0.1 + 0.2` is *not* exactly `0.3`. Instead of `==`, compare using an epsilon (a small tolerance).

```cpp
#include <iostream>
#include <cmath>
#include <cassert>

int main() {
    double a = 0.1 + 0.2;

    // This FAILS — floating-point is not exact:
    // assert(a == 0.3);

    // Use epsilon for comparison
    constexpr double epsilon = 1e-9;
    assert(std::abs(a - 0.3) < epsilon);

    std::cout << "0.1 + 0.2 = " << a << std::endl;
    std::cout << "Difference from 0.3 = " << std::abs(a - 0.3) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| `0.1 + 0.2 != 0.3` | Floating-point can't represent all decimals exactly |
| `==` comparison | **Unreliable** for floating-point values |
| Epsilon comparison | `std::abs(a - b) < epsilon` — the standard approach |

### Practice

6. 🌟🌟 Floating-point comparison is tricky. Fix the assert.

```cpp,editable
// Fix the assert: 0.1 + 0.2 is NOT exactly 0.3
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
    double a = 0.1 + 0.2;
    assert(a == 0.3);  // This fails! Fix it.

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Type Casting

### How it works

Implicit conversions happen automatically (e.g., `int` → `double`), but can lose data when narrowing (e.g., `int` → `int8_t`). Use `static_cast<T>()` for explicit, safe conversions. Integer division truncates — cast at least one operand to `double` for fractional results.

```cpp
#include <iostream>
#include <cstdint>
#include <cmath>
#include <cassert>

int main() {
    // Implicit widening (safe — no data loss)
    int i = 42;
    double d = i;
    assert(d == 42.0);

    // Explicit cast with static_cast
    double pi = 3.14159;
    int truncated = static_cast<int>(pi);  // fractional part lost
    assert(truncated == 3);

    // Integer division vs floating-point division
    int a = 5, b = 2;
    int int_result = a / b;                          // 2 (truncated)
    double fp_result = static_cast<double>(a) / b;   // 2.5
    assert(int_result == 2);
    assert(std::abs(fp_result - 2.5) < 0.001);

    std::cout << "5 / 2 (int)    = " << int_result << std::endl;
    std::cout << "5 / 2 (double) = " << fp_result << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Conversion | Behavior |
|------------|----------|
| Widening (`int` → `double`) | Safe, automatic |
| Narrowing (`int` → `int8_t`) | May lose data — compiler warns |
| `static_cast<T>(expr)` | Explicit, checked at compile time |
| Integer division (`5 / 2`) | Truncates to `2` |
| Floating division | Cast an operand: `static_cast<double>(a) / b` |

### Practice

7. 🌟🌟 Narrowing conversion: 256 doesn't fit in `int8_t`. Fix it so `y == x`.

```cpp,editable
// Fix the narrowing conversion: int8_t range is -128 to 127
#include <iostream>
#include <cassert>
#include <cstdint>

int main() {
    int x = 256;
    int8_t y = x;  // Narrowing! 256 doesn't fit.

    assert(y == x);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

8. 🌟🌟 Integer division truncates. Fix the code so the result is `2.5`.

```cpp,editable
// Fix the code so result is 2.5, not 2
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
    int a = 5;
    int b = 2;
    double result = a / b;

    assert(std::abs(result - 2.5) < 0.001);

    std::cout << "result = " << result << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Arithmetic Operators

### How it works

C++ provides the standard arithmetic operators. Integer division truncates toward zero, and the modulo operator `%` gives the remainder.

```cpp
#include <iostream>
#include <cassert>

int main() {
    assert(3 + 4 == 7);     // addition
    assert(10 - 3 == 7);    // subtraction
    assert(4 * 5 == 20);    // multiplication
    assert(10 / 3 == 3);    // integer division (truncates)
    assert(10 % 3 == 1);    // modulo (remainder)

    // With doubles, division gives the full result
    double d = 10.0 / 3.0;
    assert(d > 3.33 && d < 3.34);

    std::cout << "10.0 / 3.0 = " << d << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Operator | Operation | Example | Result |
|----------|-----------|---------|--------|
| `+` | Addition | `3 + 4` | `7` |
| `-` | Subtraction | `10 - 3` | `7` |
| `*` | Multiplication | `4 * 5` | `20` |
| `/` | Division | `10 / 3` | `3` (integer) |
| `%` | Modulo | `10 % 3` | `1` |

### Practice

9. 🌟 Fill in the blanks with the correct results.

```cpp,editable
// Fill the blanks
#include <iostream>
#include <cassert>

int main() {
    assert(1 + 2 == __);
    assert(10 - 3 == __);
    assert(4 * 5 == __);
    assert(10 / 3 == __);   // integer division
    assert(10 % 3 == __);   // modulo

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
