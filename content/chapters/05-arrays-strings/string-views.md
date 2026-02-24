# String Views & String Operations

String views provide a lightweight, non-owning reference to a sequence of characters. Introduced in C++17, `std::string_view` avoids unnecessary copies when you only need to read a string. Combined with the standard library's string conversion functions (`stoi`, `stod`, `to_string`), you have a complete toolkit for parsing and formatting text data.

## std::string_view Basics

### How it works

`std::string_view` is a non-owning view into a contiguous sequence of characters. It can be constructed from a `std::string`, a string literal, or a `char*` with a length. Because it never copies the underlying data, it is cheap to create and pass around.

```cpp
#include <iostream>
#include <string>
#include <string_view>

int main() {
    std::string str = "Hello, World!";
    std::string_view sv1 = str;           // view into std::string
    std::string_view sv2 = "Hello";       // view into string literal

    std::cout << "sv1: " << sv1 << std::endl;           // Hello, World!
    std::cout << "sv2: " << sv2 << std::endl;           // Hello
    std::cout << "sv1 size: " << sv1.size() << std::endl; // 13
    std::cout << "sv2[0]: " << sv2[0] << std::endl;       // H
    return 0;
}
```

| Feature | Description |
|---|---|
| `std::string_view sv = str;` | Construct from `std::string` (no copy) |
| `std::string_view sv = "lit";` | Construct from string literal (no copy) |
| `.size()` / `.length()` | Number of characters in the view |
| `.empty()` | `true` if view has zero characters |
| `.data()` | Pointer to the underlying character array |
| `[i]` / `.at(i)` | Access character at index `i` |

### Practice

1\. 🌟 **Fill:** the blanks with the correct values for each `string_view` property.

```cpp,editable
#include <iostream>
#include <string>
#include <string_view>
#include <cassert>

int main() {
    std::string str = "C++ Programming";
    std::string_view sv = str;

    assert(sv.size() == __);
    assert(sv[0] == __);
    assert(sv.empty() == __);

    std::string_view sv2 = "Hello";
    assert(sv2.length() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## String View Operations

### How it works

`std::string_view` supports many of the same search and slicing operations as `std::string`. The `substr` method returns another `string_view` (not a new string), keeping things allocation-free. C++20 adds convenient `starts_with` and `ends_with` methods.

```cpp
#include <iostream>
#include <string_view>

int main() {
    std::string_view sv = "Hello, World!";

    std::cout << sv.substr(0, 5) << std::endl;       // Hello
    std::cout << sv.find("World") << std::endl;        // 7
    std::cout << sv.rfind('l') << std::endl;           // 10
    std::cout << sv.substr(7, 5) << std::endl;        // World

    // C++20: starts_with and ends_with
    std::cout << sv.starts_with("Hello") << std::endl; // 1
    std::cout << sv.ends_with("World!") << std::endl;  // 1

    // remove_prefix / remove_suffix modify the view (not the data)
    std::string_view trimmed = sv;
    trimmed.remove_prefix(7);    // "World!"
    trimmed.remove_suffix(1);    // "World"
    std::cout << trimmed << std::endl;  // World
    return 0;
}
```

| Method | Returns / Effect |
|---|---|
| `.substr(pos, len)` | A new `string_view` into the same data |
| `.find(str)` | Index of first occurrence, or `std::string_view::npos` |
| `.rfind(c)` | Index of last occurrence of character `c` |
| `.starts_with(prefix)` | `true` if the view begins with `prefix` (C++20) |
| `.ends_with(suffix)` | `true` if the view ends with `suffix` (C++20) |
| `.remove_prefix(n)` | Advances the start of the view by `n` characters |
| `.remove_suffix(n)` | Shrinks the view by `n` characters from the end |

### Practice

2\. 🌟🌟 **Fill:** the blanks with the correct results of each string_view operation.

```cpp,editable
#include <iostream>
#include <string_view>
#include <cassert>

int main() {
    std::string_view sv = "The quick brown fox";

    assert(sv.substr(4, 5) == __);
    assert(sv.find("brown") == __);
    assert(sv.rfind(' ') == __);

    std::string_view trimmed = sv;
    trimmed.remove_prefix(4);
    trimmed.remove_suffix(4);
    assert(trimmed == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3\. 🌟🌟 **Complete:** use `starts_with` and `ends_with` (C++20) to check string prefixes and suffixes.

```cpp,editable
#include <iostream>
#include <string_view>
#include <cassert>

int main() {
    std::string_view filename = "report_2024.csv";

    // Fill the blanks: check if filename starts with "report" and ends with ".csv"
    assert(filename.__(__) == true);
    assert(filename.__(__) == true);

    // Fill: what does it start/end with?
    assert(filename.starts_with(__) == false);  // does NOT start with "data"
    assert(filename.ends_with(__) == false);    // does NOT end with ".txt"

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## String Conversions

### How it works

The standard library provides functions to convert between strings and numeric types. These live in the `<string>` header.

```cpp
#include <iostream>
#include <string>

int main() {
    // String to number
    int i = std::stoi("42");
    double d = std::stod("3.14");
    long l = std::stol("1000000");

    std::cout << "int: " << i << std::endl;     // 42
    std::cout << "double: " << d << std::endl;  // 3.14
    std::cout << "long: " << l << std::endl;    // 1000000

    // Number to string
    std::string s1 = std::to_string(42);
    std::string s2 = std::to_string(3.14);

    std::cout << "s1: " << s1 << std::endl;  // 42
    std::cout << "s2: " << s2 << std::endl;  // 3.140000
    return 0;
}
```

| Function | Direction | Example |
|---|---|---|
| `std::stoi(str)` | String to `int` | `stoi("42")` returns `42` |
| `std::stod(str)` | String to `double` | `stod("3.14")` returns `3.14` |
| `std::stol(str)` | String to `long` | `stol("1000000")` returns `1000000` |
| `std::stof(str)` | String to `float` | `stof("2.5")` returns `2.5f` |
| `std::to_string(num)` | Number to `std::string` | `to_string(42)` returns `"42"` |

### Practice

4\. 🌟 **Fill:** the blanks to convert strings to numbers and back.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    int a = std::__("123");
    assert(a == 123);

    double b = std::__("3.14");
    assert(b > 3.13 && b < 3.15);

    std::string s = std::__(__);
    assert(s == "42");

    long c = std::__("999999");
    assert(c == 999999);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟 **Complete:** parse a comma-separated string of integers and compute their sum using string operations and conversions.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string data = "10,20,30,40";
    int sum = 0;

    // Parse the comma-separated values and add them to sum
    // Hint: use find(',') and substr() in a loop, then stoi()
    std::string remaining = data;
    while (true) {
        size_t pos = remaining.find(',');
        if (pos == std::string::npos) {
            sum += std::stoi(__);  // last number
            break;
        }
        sum += std::stoi(remaining.substr(__, __));  // number before comma
        remaining = remaining.substr(__ + __);       // skip past comma
    }

    assert(sum == 100);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
