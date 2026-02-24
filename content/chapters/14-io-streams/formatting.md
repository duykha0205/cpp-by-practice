# Format & Print (C++20/23)

C++20 introduced `std::format` — a modern, type-safe formatting library inspired by Python's `str.format()`. It replaces the error-prone `printf` and clunky `std::ostringstream` patterns with clean, readable format strings. C++23 adds `std::print` and `std::println` for direct formatted output.

## std::format Basics

`std::format` takes a format string with `{}` placeholders and returns a `std::string`. Placeholders are filled in order, or you can use positional indices like `{0}` and `{1}` to control which argument goes where.

### How it works

```cpp
#include <iostream>
#include <format>
#include <string>

int main() {
    // Basic replacement
    std::string s1 = std::format("Hello, {}!", "World");
    std::cout << s1 << std::endl;  // Hello, World!

    // Multiple arguments
    std::string s2 = std::format("{} is {} years old", "Alice", 30);
    std::cout << s2 << std::endl;  // Alice is 30 years old

    // Positional arguments (reorder or reuse)
    std::string s3 = std::format("{1} before {0}", "second", "first");
    std::cout << s3 << std::endl;  // first before second

    return 0;
}
```

| Syntax | Meaning |
|--------|---------|
| `{}` | Next argument in order |
| `{0}`, `{1}` | Argument at position 0, 1, etc. |
| `std::format(fmt, args...)` | Returns a `std::string` with placeholders replaced |
| Mixed types | Ints, doubles, strings — all work seamlessly |

### Practice

1. 🌟🌟 **Fill:** Use `std::format` with positional arguments to build strings.

```cpp,editable
#include <iostream>
#include <format>
#include <string>
#include <cassert>

int main() {
    // Use automatic argument ordering
    std::string greeting = std::format(__, "C++", 20);
    assert(greeting == "C++ version 20");

    // Use positional arguments to reverse the order
    std::string reversed = std::format(__, "World", "Hello");
    assert(reversed == "Hello, World!");

    // Reuse the same argument twice
    std::string repeated = std::format(__, "ha");
    assert(repeated == "ha-ha");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Format Specifiers

Inside `{}`, a colon `:` introduces format specifiers that control width, alignment, fill, precision, and type. The syntax is `{[index]:[fill][align][width][.precision][type]}`.

### How it works

```cpp
#include <iostream>
#include <format>
#include <string>

int main() {
    // Width and alignment
    std::cout << std::format("[{:<10}]", "left") << std::endl;    // [left      ]
    std::cout << std::format("[{:>10}]", "right") << std::endl;   // [     right]
    std::cout << std::format("[{:^10}]", "center") << std::endl;  // [  center  ]

    // Fill character
    std::cout << std::format("[{:*>10}]", 42) << std::endl;       // [********42]

    // Integer bases
    std::cout << std::format("{:d} {:x} {:o} {:b}", 42, 42, 42, 42) << std::endl;
    // 42 2a 52 101010

    // Float precision
    std::cout << std::format("{:.2f}", 3.14159) << std::endl;     // 3.14

    return 0;
}
```

| Specifier | Meaning | Example |
|-----------|---------|---------|
| `<` | Left-align | `{:<10}` |
| `>` | Right-align (default for numbers) | `{:>10}` |
| `^` | Center-align | `{:^10}` |
| `[fill]` | Fill character (before alignment) | `{:*>10}` fills with `*` |
| `[width]` | Minimum field width | `{:10}` |
| `.precision` | Decimal places for floats | `{:.2f}` |
| `d` | Decimal integer | `{:d}` |
| `x` / `X` | Hexadecimal (lower/upper) | `{:x}` |
| `o` | Octal | `{:o}` |
| `b` / `B` | Binary (lower/upper) | `{:b}` |
| `f` | Fixed-point float | `{:.3f}` |

### Practice

2. 🌟🌟 **Fill:** Use format specifiers for alignment, width, and number bases.

```cpp,editable
#include <iostream>
#include <format>
#include <string>
#include <cassert>

int main() {
    // Right-align "hi" in a field of 10, filled with dots
    std::string s1 = std::format(__, "hi");
    assert(s1 == "........hi");

    // Format 255 as hexadecimal
    std::string s2 = std::format(__, 255);
    assert(s2 == "ff");

    // Format 3.14159 with exactly 2 decimal places
    std::string s3 = std::format(__, 3.14159);
    assert(s3 == "3.14");

    // Center-align 42 in a field of 9, filled with dashes
    std::string s4 = std::format(__, 42);
    assert(s4 == "---42----");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Build a formatted table of products using `std::format`. Each row should have a left-aligned name (width 12), a right-aligned integer quantity (width 5), and a right-aligned price with 2 decimal places (width 8).

```cpp,editable
#include <iostream>
#include <format>
#include <string>
#include <vector>
#include <cassert>

int main() {
    struct Product {
        std::string name;
        int quantity;
        double price;
    };

    std::vector<Product> products = {
        {"Apple", 50, 1.20},
        {"Banana", 120, 0.50},
        {"Cherry", 200, 3.75}
    };

    std::vector<std::string> rows;
    for (const auto& p : products) {
        // Format: name left-aligned 12 chars, qty right-aligned 5 chars,
        //         price right-aligned 8 chars with 2 decimals
        std::string row = std::format(__, p.name, p.quantity, p.price);
        rows.push_back(row);
    }

    assert(rows[0] == "Apple          50    1.20");
    assert(rows[1] == "Banana        120    0.50");
    assert(rows[2] == "Cherry        200    3.75");

    for (const auto& r : rows) {
        std::cout << r << std::endl;
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Formatting Custom Types

You can make your own types work with `std::format` by specializing `std::formatter<T>`. Implement `parse()` to handle format specifiers and `format()` to produce the output.

> **Note:** Custom formatter specialization requires a C++20-compliant compiler with full `<format>` support (GCC 13+, Clang 17+, MSVC 19.29+). Some older compilers may not support this feature.

### How it works

```cpp
#include <iostream>
#include <format>
#include <string>

struct Point {
    double x, y;
};

template <>
struct std::formatter<Point> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();  // no custom format specifiers
    }

    auto format(const Point& p, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

int main() {
    Point p{3.0, 4.0};
    std::cout << std::format("Point: {}", p) << std::endl;
    // Point: (3, 4)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::formatter<T>` | Template to specialize for custom type `T` |
| `parse(ctx)` | Parses the format spec (between `:` and `}`); return iterator past consumed chars |
| `format(val, ctx)` | Writes the formatted output; returns the output iterator |
| `std::format_to(out, fmt, args...)` | Like `format`, but writes to an output iterator |

### Practice

4. 🌟🌟🌟 **Complete:** Write a `std::formatter` specialization for a `Color` struct so it can be used with `std::format`.

> **Note:** This exercise requires C++20 `<format>` support. If your compiler does not support custom formatters, you may need GCC 13+, Clang 17+, or MSVC 19.29+.

```cpp,editable
#include <iostream>
#include <format>
#include <string>
#include <cassert>

struct Color {
    int r, g, b;
};

template <>
struct std::formatter<Color> {
    constexpr auto parse(std::format_parse_context& ctx) {
        __;
    }

    auto format(const Color& c, std::format_context& ctx) const {
        // Format as "rgb(R, G, B)"
        return std::format_to(__, __, c.r, c.g, c.b);
    }
};

int main() {
    Color red{255, 0, 0};
    Color white{255, 255, 255};

    std::string s1 = std::format("{}", red);
    std::string s2 = std::format("{}", white);

    assert(s1 == "rgb(255, 0, 0)");
    assert(s2 == "rgb(255, 255, 255)");

    // Use in a larger format string
    std::string s3 = std::format("Color is: {}", red);
    assert(s3 == "Color is: rgb(255, 0, 0)");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
