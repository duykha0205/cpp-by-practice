# If-Init Statements & Constexpr If

C++17 introduced two powerful extensions to conditional statements. **If with initializer** (`if (init; cond)`) lets you declare a variable scoped to the `if/else` block. **Constexpr if** (`if constexpr`) evaluates conditions at compile time, enabling clean template code without SFINAE or tag dispatch.

## If with Initializer

### How it works

C++17 allows an initializer statement inside `if`: `if (init; condition)`. The variable declared in `init` is scoped to the entire `if/else if/else` chain — it doesn't leak into the surrounding scope. This is especially useful with functions that return status codes or iterators.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};

    // Variable 'it' is scoped to the if/else block
    if (auto it = std::find(v.begin(), v.end(), 30); it != v.end()) {
        std::cout << "Found: " << *it << std::endl;
        assert(*it == 30);
    } else {
        std::cout << "Not found" << std::endl;
    }
    // 'it' is not accessible here — no scope pollution

    // Works with simple types too
    if (int x = 42; x > 0) {
        std::cout << x << " is positive" << std::endl;
        assert(x == 42);
    }
    // 'x' is not accessible here

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | Syntax | Benefit |
|---------|--------|---------|
| If-init | `if (auto x = expr; cond)` | Variable scoped to `if/else` block |
| Scope | Variable visible in `if`, `else if`, and `else` | No leaking into surrounding scope |
| Common use | `if (auto it = find(...); it != end)` | Clean iterator/result checking |

### Practice

1\. 🌟🌟 **Fill:** Use an if-init statement to declare the variable inside the `if` and check the condition.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int compute_score() {
    return 85;
}

int main() {
    // Rewrite using if-init: declare 'score' inside the if
    // if (init; condition)
    if (__; score >= 60) {
        std::cout << "Passed with score: " << score << std::endl;
        assert(score == 85);
    } else {
        std::cout << "Failed with score: " << score << std::endl;
    }

    // 'score' should NOT be accessible here (scoped to if/else)

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Switch with Initializer

### How it works

Just like `if`, C++17 also allows an initializer in `switch` statements: `switch (init; expr)`. This is useful when you need to compute a value and immediately switch on it, without the variable leaking out.

```cpp
#include <iostream>
#include <cassert>

enum class Status { OK, Warning, Error };

Status get_status() { return Status::Warning; }

int main() {
    // Variable 's' is scoped to the switch block
    switch (Status s = get_status(); s) {
        case Status::OK:
            std::cout << "All good" << std::endl;
            break;
        case Status::Warning:
            std::cout << "Warning!" << std::endl;
            assert(s == Status::Warning);
            break;
        case Status::Error:
            std::cout << "Error!" << std::endl;
            break;
    }
    // 's' is not accessible here

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | Syntax | Benefit |
|---------|--------|---------|
| Switch-init | `switch (auto x = expr; x)` | Variable scoped to switch block |
| Scope | Variable visible in all `case` branches | Clean, no leaking |
| Common use | `switch (auto s = get_status(); s)` | Compute and switch in one line |

### Practice

2\. 🌟🌟 **Fill:** Use a switch-init statement to call `get_day()` and switch on the result in a single statement.

```cpp,editable
#include <iostream>
#include <cassert>

int get_day() {
    return 3;  // Wednesday
}

int main() {
    // Use switch with initializer: declare 'day' inside switch
    switch (__; day) {
        case 1: std::cout << "Monday" << std::endl; break;
        case 2: std::cout << "Tuesday" << std::endl; break;
        case 3:
            std::cout << "Wednesday" << std::endl;
            assert(day == 3);
            break;
        case 4: std::cout << "Thursday" << std::endl; break;
        case 5: std::cout << "Friday" << std::endl; break;
        default: std::cout << "Weekend" << std::endl; break;
    }
    // 'day' should NOT be accessible here

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Constexpr If

### How it works

`if constexpr (condition)` evaluates the condition at **compile time**. The branch that is not taken is **discarded** — it doesn't even need to be valid code for the given type. This is a game-changer for template programming, replacing many uses of SFINAE and tag dispatch.

```cpp
#include <iostream>
#include <string>
#include <type_traits>
#include <cassert>

template <typename T>
std::string describe(T value) {
    if constexpr (std::is_integral_v<T>) {
        return "integer: " + std::to_string(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "float: " + std::to_string(value);
    } else {
        return "other";
    }
}

template <typename T>
T double_value(T x) {
    if constexpr (std::is_arithmetic_v<T>) {
        return x * 2;
    } else {
        return x + x;  // works for strings via concatenation
    }
}

int main() {
    assert(describe(42) == "integer: 42");
    assert(describe(3.14).substr(0, 6) == "float:");

    assert(double_value(5) == 10);
    assert(double_value(1.5) == 3.0);
    assert(double_value(std::string("ab")) == "abab");

    std::cout << describe(42) << std::endl;
    std::cout << describe(3.14) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | `if constexpr` | Regular `if` |
|---------|----------------|-------------|
| Evaluation | Compile time | Runtime |
| Discarded branch | Not compiled, can be invalid for type | Must be valid code |
| Use case | Template specialization by type traits | Normal branching |
| Requires | Condition must be a constant expression | Any boolean expression |

### Practice

3\. 🌟🌟🌟 **Fill:** Use `if constexpr` and type traits to implement `to_string_value` that converts integers with `std::to_string` and returns string values as-is.

```cpp,editable
#include <iostream>
#include <string>
#include <type_traits>
#include <cassert>

template <typename T>
std::string to_string_value(T value) {
    // Use if constexpr: if T is integral, return std::to_string(value)
    // Otherwise, return value as-is (assume it's already a string)
    __
}

int main() {
    assert(to_string_value(42) == "42");
    assert(to_string_value(100) == "100");
    assert(to_string_value(std::string("hello")) == "hello");
    assert(to_string_value(std::string("world")) == "world");

    std::cout << to_string_value(42) << std::endl;
    std::cout << to_string_value(std::string("hello")) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

4\. 🌟🌟🌟 **Complete:** Write a template function `safe_divide` that uses `if constexpr` to handle integer division (checking for zero and returning 0) differently from floating-point division (returning infinity via normal division).

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cmath>
#include <cassert>

template <typename T>
T safe_divide(T a, T b) {
    // Use if constexpr:
    // - If T is integral: return 0 when b is 0, otherwise return a / b
    // - If T is floating point: just return a / b (produces inf for /0)
    __
}

int main() {
    // Integer division
    assert(safe_divide(10, 3) == 3);
    assert(safe_divide(10, 0) == 0);     // protected from division by zero
    assert(safe_divide(100, 5) == 20);

    // Floating-point division
    assert(std::abs(safe_divide(10.0, 3.0) - 3.333333) < 0.001);
    assert(std::isinf(safe_divide(1.0, 0.0)));  // returns infinity

    std::cout << "safe_divide(10, 3) = " << safe_divide(10, 3) << std::endl;
    std::cout << "safe_divide(10, 0) = " << safe_divide(10, 0) << std::endl;
    std::cout << "safe_divide(10.0, 3.0) = " << safe_divide(10.0, 3.0) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
