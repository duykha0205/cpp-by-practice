# Exceptions

Exceptions are C++'s primary mechanism for reporting and handling errors at runtime. Instead of returning error codes, a function **throws** an exception object, and the runtime unwinds the call stack until it finds a matching **catch** block. This separates error-handling logic from the normal flow of the program.

## Try, Catch & Throw

The basic exception flow uses three keywords: `throw` to signal an error, `try` to mark a block where exceptions may occur, and `catch` to handle them. You can throw values of any type — integers, strings, or exception objects.

### How it works

```cpp
#include <iostream>
#include <string>

int divide(int a, int b) {
    if (b == 0)
        throw std::string("Division by zero!");
    return a / b;
}

int main() {
    try {
        std::cout << divide(10, 2) << std::endl;  // 5
        std::cout << divide(10, 0) << std::endl;  // throws
    } catch (const std::string& msg) {
        std::cout << "Error: " << msg << std::endl;
    }
    return 0;
}
```

| Keyword | Purpose |
|---------|---------|
| `throw expr` | Creates an exception object and begins stack unwinding |
| `try { ... }` | Marks a block where exceptions may be thrown |
| `catch (Type& e)` | Catches exceptions of the specified type |
| `catch (...)` | Catches any exception (catch-all) |

### Practice

1. :star: **Fill:** Complete the `try`/`catch` block so that the division-by-zero exception (thrown as an `int`) is caught.

```cpp,editable
#include <iostream>
#include <cassert>

int divide(int a, int b) {
    if (b == 0)
        throw -1;
    return a / b;
}

int main() {
    int result = 0;
    bool caught = false;

    __ {
        result = divide(10, 0);
    } __ (int e) {
        caught = true;
    }

    assert(caught == true);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Standard Exception Hierarchy

The `<stdexcept>` header provides a hierarchy of exception classes rooted at `std::exception`. Using these standard types makes your error handling consistent and interoperable with library code.

### How it works

```cpp
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
    try {
        std::vector<int> v = {1, 2, 3};
        std::cout << v.at(10) << std::endl;  // throws std::out_of_range
    } catch (const std::out_of_range& e) {
        std::cout << "Out of range: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
```

| Class | Parent | Typical use |
|-------|--------|-------------|
| `std::exception` | (root) | Base class; override `what()` |
| `std::runtime_error` | `std::exception` | Errors detectable only at runtime |
| `std::logic_error` | `std::exception` | Errors due to faulty logic / preconditions |
| `std::out_of_range` | `std::logic_error` | Index or iterator out of bounds |
| `std::invalid_argument` | `std::logic_error` | Bad function argument |

### Practice

2. :star::star: **Fill:** Throw the appropriate standard exception type for each error condition.

```cpp,editable
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

int parse_positive(const std::string& s) {
    int val = std::stoi(s);
    if (val <= 0)
        throw __(("Expected positive number, got: " + s));
    return val;
}

int get_element(const int* arr, int size, int index) {
    if (index < 0 || index >= size)
        throw __(("Index " + std::to_string(index) + " out of range"));
    return arr[index];
}

int main() {
    bool caught_invalid = false;
    try {
        parse_positive("-5");
    } catch (const std::invalid_argument& e) {
        caught_invalid = true;
    }
    assert(caught_invalid);

    bool caught_range = false;
    int arr[] = {10, 20, 30};
    try {
        get_element(arr, 3, 5);
    } catch (const std::out_of_range& e) {
        caught_range = true;
    }
    assert(caught_range);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Catching by Reference

Always catch exceptions **by const reference**. Catching by value causes **object slicing** — the derived part of the exception is lost. To rethrow the current exception without slicing, use a bare `throw;` statement.

### How it works

```cpp
#include <iostream>
#include <stdexcept>

void might_fail() {
    throw std::runtime_error("something went wrong");
}

void wrapper() {
    try {
        might_fail();
    } catch (const std::exception& e) {
        std::cout << "Logging: " << e.what() << std::endl;
        throw;  // rethrow the SAME exception (no slicing)
    }
}

int main() {
    try {
        wrapper();
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << std::endl;
    }
    return 0;
}
```

| Technique | Why |
|-----------|-----|
| `catch (const std::exception& e)` | Catches all derived types without slicing |
| `catch (std::exception e)` | BAD: slices derived data away |
| `throw;` | Rethrows the current exception object, preserving its dynamic type |
| `throw e;` | Throws a **copy** of `e` — may slice if `e` is a reference to a derived type |

### Practice

3. :star::star: **Fix:** The function catches by value (causing slicing) and rethrows with `throw e;` (also causing slicing). Fix both problems.

```cpp,editable
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

class DetailedError : public std::runtime_error {
    int code_;
public:
    DetailedError(const std::string& msg, int code)
        : std::runtime_error(msg), code_(code) {}
    int code() const { return code_; }
};

void process() {
    throw DetailedError("disk full", 28);
}

void middleware() {
    try {
        process();
    // BUG: catching by value causes slicing
    } catch (std::runtime_error e) {
        // BUG: rethrowing e by name slices the DetailedError
        throw e;
    }
}

int main() {
    bool caught_detailed = false;
    try {
        middleware();
    } catch (const DetailedError& e) {
        caught_detailed = true;
        assert(e.code() == 28);
        assert(std::string(e.what()) == "disk full");
    } catch (const std::exception& e) {
        // If slicing happened, we'd end up here instead
    }

    assert(caught_detailed);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## noexcept Specifier

The `noexcept` specifier tells the compiler (and other programmers) that a function will not throw exceptions. If a `noexcept` function does throw, `std::terminate` is called. The `noexcept` operator can test at compile time whether an expression might throw.

### How it works

```cpp
#include <iostream>
#include <type_traits>

void safe_func() noexcept {
    // guaranteed not to throw
}

void risky_func() {
    throw 42;
}

// Conditional noexcept: noexcept only if T is nothrow move constructible
template<typename T>
void swap_values(T& a, T& b) noexcept(std::is_nothrow_move_constructible_v<T>) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

int main() {
    std::cout << std::boolalpha;
    std::cout << noexcept(safe_func()) << std::endl;   // true
    std::cout << noexcept(risky_func()) << std::endl;   // false
    return 0;
}
```

| Feature | Syntax | Purpose |
|---------|--------|---------|
| `noexcept` specifier | `void f() noexcept` | Promises that `f` never throws |
| `noexcept(expr)` specifier | `void f() noexcept(condition)` | Conditionally noexcept |
| `noexcept(expr)` operator | `noexcept(f())` | Returns `true` if the expression is noexcept |
| Violation | If a `noexcept` function throws | `std::terminate()` is called |

### Practice

4. :star::star: **Fill:** Mark the function `noexcept` and use the `noexcept` operator to verify.

```cpp,editable
#include <iostream>
#include <cassert>

int safe_add(int a, int b) __ {
    return a + b;
}

int main() {
    assert(safe_add(3, 4) == 7);

    // The noexcept operator should return true for safe_add
    static_assert(__(safe_add(1, 2)), "safe_add should be noexcept");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Write a function that catches different exception types and returns a string describing what was caught. The function should handle `std::out_of_range`, `std::runtime_error`, `std::exception`, and unknown exceptions (catch-all).

```cpp,editable
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

std::string test_catch(int which) {
    try {
        switch (which) {
            case 1: throw std::out_of_range("oob");
            case 2: throw std::runtime_error("rt");
            case 3: throw std::logic_error("logic");
            case 4: throw 42;
        }
    // Add catch blocks in order: most specific first
    // Case 1 -> return "out_of_range"
    // Case 2 -> return "runtime_error"
    // Case 3 -> return "exception" (logic_error is-a exception)
    // Case 4 -> return "unknown"
    __
    }
    return "no exception";
}

int main() {
    assert(test_catch(1) == "out_of_range");
    assert(test_catch(2) == "runtime_error");
    assert(test_catch(3) == "exception");
    assert(test_catch(4) == "unknown");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
