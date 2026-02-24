# Error Codes & std::expected

Not every error needs to be an exception. For expected failure modes (file not found, invalid input, network timeout), **error codes** and **value-or-error return types** offer a lightweight, explicit alternative. C++23's `std::expected` brings this pattern into the standard library, giving you the best of both worlds.

## Return-Code Error Handling

The simplest error-handling strategy is returning a status code or using `std::optional` to signal "no value." This avoids the overhead of exceptions for errors that are part of normal program flow.

### How it works

```cpp
#include <iostream>
#include <optional>
#include <string>
#include <cstdlib>

// Classic C-style: return 0 for success, negative for error
int parse_int(const char* str, int& out) {
    char* end;
    long val = std::strtol(str, &end, 10);
    if (end == str) return -1;  // no digits found
    out = static_cast<int>(val);
    return 0;  // success
}

// Modern C++: use optional for nullable return
std::optional<int> safe_divide(int a, int b) {
    if (b == 0) return std::nullopt;
    return a / b;
}

int main() {
    int val;
    if (parse_int("42", val) == 0)
        std::cout << "Parsed: " << val << std::endl;  // 42

    auto result = safe_divide(10, 3);
    if (result.has_value())
        std::cout << "Result: " << result.value() << std::endl;  // 3
    else
        std::cout << "Cannot divide by zero" << std::endl;
    return 0;
}
```

| Pattern | Pros | Cons |
|---------|------|------|
| Return code (`int`) | Simple, zero overhead | Easy to ignore, can't return a value and error at once |
| `errno` | Global error code set by C library functions | Thread-safety concerns, easy to forget to check |
| `std::optional<T>` | Type-safe "value or nothing" | No error information — just "absent" |

### Practice

1. :star::star: **Fill:** Complete the function that uses `std::optional` to safely look up a value in an array. Return `std::nullopt` if the index is out of bounds.

```cpp,editable
#include <iostream>
#include <optional>
#include <vector>
#include <cassert>

std::optional<int> safe_at(const std::vector<int>& vec, int index) {
    if (index < 0 || index >= static_cast<int>(vec.size()))
        return __;
    return __;
}

int main() {
    std::vector<int> v = {10, 20, 30};

    auto r1 = safe_at(v, 1);
    assert(r1.has_value());
    assert(r1.value() == 20);

    auto r2 = safe_at(v, 5);
    assert(!r2.has_value());

    // value_or provides a default when empty
    assert(safe_at(v, 0).value_or(-1) == 10);
    assert(safe_at(v, 99).value_or(-1) == -1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::error_code

`std::error_code` (from `<system_error>`) is a portable way to represent system and library error codes. It pairs an integer code with an **error category**, so codes from different subsystems never collide. The `std::errc` enumeration provides portable names for common POSIX errors.

### How it works

```cpp
#include <iostream>
#include <system_error>
#include <fstream>
#include <cerrno>

std::error_code open_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open()) {
        return std::make_error_code(std::errc::no_such_file_or_directory);
    }
    // ... process file ...
    return {};  // empty error_code means success
}

int main() {
    auto ec = open_file("nonexistent.txt");

    if (ec) {  // non-zero error_code is truthy
        std::cout << "Error: " << ec.message() << std::endl;
        std::cout << "Code: " << ec.value() << std::endl;
        std::cout << "Category: " << ec.category().name() << std::endl;
    }

    // Compare against portable error conditions
    if (ec == std::errc::no_such_file_or_directory)
        std::cout << "File not found!" << std::endl;

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::error_code` | Holds an integer error code + a reference to an error category |
| `std::errc` | Enum with portable POSIX error names (e.g., `permission_denied`) |
| `std::make_error_code(errc)` | Constructs an `error_code` from an `errc` value |
| `ec.message()` | Returns a human-readable description of the error |
| Empty `error_code{}` | Represents "no error" (success); evaluates to `false` |

### Practice

2. :star::star: **Fill:** Complete the function that returns an `std::error_code`. Return a success code when the input is valid, and `std::errc::invalid_argument` when it is not.

```cpp,editable
#include <iostream>
#include <system_error>
#include <string>
#include <cassert>

std::error_code validate_port(int port) {
    if (port < 1 || port > 65535)
        return __;
    return __;  // success
}

int main() {
    auto ec1 = validate_port(8080);
    assert(!ec1);  // no error

    auto ec2 = validate_port(-1);
    assert(ec2);   // has error
    assert(ec2 == __);

    auto ec3 = validate_port(70000);
    assert(ec3);
    assert(ec3 == std::errc::invalid_argument);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::expected (C++23)

`std::expected<T, E>` is a vocabulary type that holds either a value of type `T` (the success case) or an error of type `E`. Unlike `std::optional`, it carries **error information**. Unlike exceptions, error handling is explicit and zero-overhead. C++23 also adds monadic operations (`and_then`, `or_else`, `transform`) for chaining.

### How it works

```cpp
// NOTE: std::expected requires C++23 support.
// Compile with: g++ -std=c++23 or clang++ -std=c++23
#include <iostream>
#include <expected>
#include <string>
#include <cmath>

enum class MathError {
    DivisionByZero,
    NegativeSqrt
};

std::string to_string(MathError e) {
    switch (e) {
        case MathError::DivisionByZero: return "division by zero";
        case MathError::NegativeSqrt:   return "square root of negative";
    }
    return "unknown";
}

std::expected<double, MathError> safe_divide(double a, double b) {
    if (b == 0.0)
        return std::unexpected(MathError::DivisionByZero);
    return a / b;
}

std::expected<double, MathError> safe_sqrt(double x) {
    if (x < 0.0)
        return std::unexpected(MathError::NegativeSqrt);
    return std::sqrt(x);
}

int main() {
    auto result = safe_divide(10.0, 3.0);
    if (result.has_value())
        std::cout << "Result: " << result.value() << std::endl;  // ~3.333

    auto err = safe_divide(1.0, 0.0);
    if (!err.has_value())
        std::cout << "Error: " << to_string(err.error()) << std::endl;

    // Monadic chaining with and_then (C++23)
    auto chained = safe_divide(100.0, 4.0)
        .and_then([](double val) { return safe_sqrt(val); });
    std::cout << "sqrt(100/4) = " << chained.value() << std::endl;  // 5
    return 0;
}
```

| Feature | Detail |
|---------|--------|
| `std::expected<T, E>` | Holds either a `T` value or an `E` error |
| `std::unexpected(e)` | Constructs the error state |
| `.has_value()` | Returns `true` if it holds a value |
| `.value()` | Returns the stored value (throws `bad_expected_access` if error) |
| `.error()` | Returns the stored error |
| `.value_or(default)` | Returns value if present, otherwise the default |
| `.and_then(f)` | If value, applies `f` (which returns `expected`); if error, propagates |
| `.transform(f)` | If value, applies `f` and wraps result in `expected`; if error, propagates |

### Practice

3. :star::star: **Fill:** Complete the function that uses `std::expected` to return either a parsed integer or an error string. Return `std::unexpected` with an appropriate message for invalid input.

```cpp,editable
// NOTE: Requires C++23. Compile with -std=c++23
#include <iostream>
#include <expected>
#include <string>
#include <cassert>

std::expected<int, std::string> parse_positive_int(const std::string& s) {
    int val;
    try {
        val = std::stoi(s);
    } catch (...) {
        return __("not a number");
    }
    if (val <= 0)
        return __("must be positive");
    return __;
}

int main() {
    auto r1 = parse_positive_int("42");
    assert(r1.has_value());
    assert(r1.value() == 42);

    auto r2 = parse_positive_int("-5");
    assert(!r2.has_value());
    assert(r2.error() == "must be positive");

    auto r3 = parse_positive_int("abc");
    assert(!r3.has_value());
    assert(r3.error() == "not a number");

    // value_or provides a default
    assert(parse_positive_int("10").value_or(-1) == 10);
    assert(parse_positive_int("xyz").value_or(-1) == -1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. :star::star::star: **Complete:** Write a pipeline of two functions using `std::expected` and the monadic `and_then` operation. The first function parses a string to an int, and the second validates that the int is within a range. Chain them so that errors propagate automatically.

```cpp,editable
// NOTE: Requires C++23. Compile with -std=c++23
#include <iostream>
#include <expected>
#include <string>
#include <cassert>

using Result = std::expected<int, std::string>;

Result parse_int(const std::string& s) {
    try {
        return std::stoi(s);
    } catch (...) {
        return std::unexpected("parse error");
    }
}

// Return a lambda that validates min <= val <= max
// The lambda should return Result
auto validate_range(int min, int max) {
    __
}

int main() {
    // Chain: parse then validate range [1, 100]
    auto result1 = parse_int("42").and_then(validate_range(1, 100));
    assert(result1.has_value());
    assert(result1.value() == 42);

    auto result2 = parse_int("200").and_then(validate_range(1, 100));
    assert(!result2.has_value());
    assert(result2.error() == "out of range");

    auto result3 = parse_int("abc").and_then(validate_range(1, 100));
    assert(!result3.has_value());
    assert(result3.error() == "parse error");

    auto result4 = parse_int("0").and_then(validate_range(1, 100));
    assert(!result4.has_value());
    assert(result4.error() == "out of range");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
