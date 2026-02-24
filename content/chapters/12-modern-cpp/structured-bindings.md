# Structured Bindings, Optional, Variant & Any

C++17 introduced powerful vocabulary types that make your code more expressive and safer. **Structured bindings** let you unpack pairs, tuples, and structs into named variables. **`std::optional`** represents values that may or may not exist. **`std::variant`** is a type-safe union. **`std::any`** holds a value of any type.

## Structured Bindings

Structured bindings (C++17) let you declare multiple variables that bind to the elements of a pair, tuple, struct, or array using `auto [x, y, ...]` syntax. This eliminates the need for `.first`/`.second` or `std::get<>`.

### How it works

```cpp
#include <iostream>
#include <map>
#include <string>
#include <tuple>

int main() {
    // With std::pair
    std::pair<std::string, int> person{"Alice", 30};
    auto [name, age] = person;
    std::cout << name << " is " << age << std::endl;

    // With std::tuple
    auto tup = std::make_tuple(1, 3.14, std::string("hello"));
    auto [i, d, s] = tup;

    // With std::map iteration
    std::map<std::string, int> scores = {{"Alice", 95}, {"Bob", 87}};
    for (const auto& [student, score] : scores) {
        std::cout << student << ": " << score << std::endl;
    }
    return 0;
}
```

| Usage | Syntax | Binds To |
|-------|--------|----------|
| Pair | `auto [a, b] = pair;` | `.first`, `.second` |
| Tuple | `auto [a, b, c] = tuple;` | `std::get<0>`, `std::get<1>`, ... |
| Struct | `auto [x, y] = point;` | Public members in declaration order |
| Map loop | `for (auto& [k, v] : map)` | Each key-value pair |

### Practice

1. :star::star: **Fill:** Use structured bindings to unpack the pair and the map entries.

```cpp,editable
#include <iostream>
#include <map>
#include <string>
#include <cassert>

int main() {
    std::pair<std::string, int> p{"hello", 42};

    // Unpack the pair using structured bindings
    auto [__, __] = p;

    assert(word == "hello");
    assert(number == 42);

    std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
    int sum = 0;
    for (const auto& [__, __] : m) {
        sum += val;
    }

    assert(sum == 6);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::optional

`std::optional<T>` (C++17) holds either a value of type `T` or **nothing**. It replaces the error-prone pattern of using sentinel values (like `-1` or `nullptr`) to indicate "no result." Use `has_value()`, `value()`, and `value_or(default)` to access the contents safely.

### How it works

```cpp
#include <iostream>
#include <optional>
#include <string>

std::optional<std::string> find_user(int id) {
    if (id == 1) return "Alice";
    if (id == 2) return "Bob";
    return std::nullopt;  // not found
}

int main() {
    auto user = find_user(1);
    if (user.has_value()) {
        std::cout << "Found: " << user.value() << std::endl;
    }

    auto missing = find_user(99);
    std::cout << missing.value_or("Unknown") << std::endl;  // "Unknown"
    return 0;
}
```

| Operation | Result |
|-----------|--------|
| `std::optional<T> o;` | Empty optional (no value) |
| `o = value;` | Stores the value |
| `o = std::nullopt;` | Clears the optional |
| `o.has_value()` | `true` if it holds a value |
| `o.value()` | Returns the value (throws `bad_optional_access` if empty) |
| `o.value_or(default)` | Returns value if present, otherwise `default` |

### Practice

2. :star::star: **Fill:** Implement a safe division function that returns `std::nullopt` when dividing by zero.

```cpp,editable
#include <iostream>
#include <optional>
#include <cassert>

std::optional<double> safe_divide(double a, double b) {
    if (__) return __;
    return __;
}

int main() {
    auto r1 = safe_divide(10.0, 2.0);
    assert(r1.has_value());
    assert(r1.value() == 5.0);

    auto r2 = safe_divide(10.0, 0.0);
    assert(!r2.has_value());
    assert(r2.value_or(-1.0) == -1.0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::variant

`std::variant<Types...>` (C++17) is a **type-safe union** that holds exactly one value from a fixed set of types. Unlike a raw `union`, it tracks which type is active and prevents accessing the wrong one. Use `std::get<T>`, `std::holds_alternative<T>`, or `std::visit` to work with it.

### How it works

```cpp
#include <iostream>
#include <variant>
#include <string>

int main() {
    std::variant<int, double, std::string> v;

    v = 42;
    std::cout << std::get<int>(v) << std::endl;  // 42

    v = std::string("hello");
    std::cout << std::get<std::string>(v) << std::endl;  // hello

    // Type-safe visitation
    std::visit([](const auto& val) {
        std::cout << "Value: " << val << std::endl;
    }, v);

    return 0;
}
```

| Operation | Result |
|-----------|--------|
| `std::variant<int, double> v;` | Default-constructs the first type (`int`) |
| `v = 3.14;` | Switches active type to `double` |
| `std::get<double>(v)` | Returns value; throws `bad_variant_access` if wrong type |
| `std::holds_alternative<T>(v)` | `true` if `v` currently holds a `T` |
| `std::visit(visitor, v)` | Calls visitor with the currently held value |

### Practice

3. :star::star::star: **Complete:** Use `std::visit` with a visitor to convert a variant value to a string description.

```cpp,editable
#include <iostream>
#include <variant>
#include <string>
#include <cassert>

int main() {
    using Var = std::variant<int, double, std::string>;

    auto to_string = [](const auto& val) -> std::string {
        if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::string>) {
            return "string:" + val;
        } else {
            return __;
        }
    };

    Var v1 = 42;
    Var v2 = std::string("hello");

    assert(std::visit(__, v1) == "number:42");
    assert(std::visit(__, v2) == "string:hello");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::any

`std::any` (C++17) can hold a value of **any type**. Unlike `std::variant`, you don't need to list the allowed types up front. Use `std::any_cast<T>` to retrieve the value — it throws `std::bad_any_cast` if the type doesn't match.

### How it works

```cpp
#include <iostream>
#include <any>
#include <string>

int main() {
    std::any a = 42;
    std::cout << std::any_cast<int>(a) << std::endl;  // 42

    a = std::string("hello");
    std::cout << std::any_cast<std::string>(a) << std::endl;  // hello

    // Check type before casting
    if (a.type() == typeid(std::string)) {
        std::cout << "It's a string!" << std::endl;
    }

    a.reset();  // clear the value
    std::cout << "Has value: " << a.has_value() << std::endl;  // 0
    return 0;
}
```

| Operation | Result |
|-----------|--------|
| `std::any a = value;` | Stores any type |
| `std::any_cast<T>(a)` | Returns the value; throws `bad_any_cast` if wrong type |
| `a.type()` | Returns `std::type_info` of stored type |
| `a.has_value()` | `true` if a value is stored |
| `a.reset()` | Clears the stored value |

### Practice

4. :star::star: **Fill:** Store different types in `std::any` and cast them back correctly.

```cpp,editable
#include <iostream>
#include <any>
#include <string>
#include <cassert>

int main() {
    std::any a = 42;
    assert(__ == 42);

    a = 3.14;
    assert(__ == 3.14);

    a = std::string("cpp17");
    assert(__ == "cpp17");

    a.reset();
    assert(__ == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Build a simple type-safe container using `std::any` that stores a value and retrieves it only if the requested type matches, returning an `std::optional` instead of throwing.

```cpp,editable
#include <iostream>
#include <any>
#include <optional>
#include <string>
#include <cassert>

template <typename T>
std::optional<T> safe_any_cast(const std::any& a) {
    __
}

int main() {
    std::any a = 42;

    auto r1 = safe_any_cast<int>(a);
    assert(r1.has_value());
    assert(r1.value() == 42);

    auto r2 = safe_any_cast<std::string>(a);
    assert(!r2.has_value());

    a = std::string("hello");
    auto r3 = safe_any_cast<std::string>(a);
    assert(r3.has_value());
    assert(r3.value() == "hello");

    auto r4 = safe_any_cast<int>(a);
    assert(!r4.has_value());

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
