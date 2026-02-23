# Strings

Strings are sequences of characters. C++ gives you two flavors: the legacy C-string (a null-terminated `char` array) and the modern `std::string` class. `std::string` manages its own memory, supports concatenation with `+`, and provides a rich set of methods for searching, slicing, and modifying text.

## std::string Basics

### How it works

`std::string` stores a dynamic sequence of characters. You can query its length, access individual characters by index, and check whether it is empty.

```cpp
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello";

    std::cout << "Length: " << s.length() << std::endl;  // 5
    std::cout << "First:  " << s[0]       << std::endl;  // H
    std::cout << "Empty?  " << s.empty()  << std::endl;  // 0 (false)
    return 0;
}
```

| Method | Returns |
|---|---|
| `.length()` / `.size()` | Number of characters |
| `[i]` | Character at index `i` (no bounds check) |
| `.at(i)` | Character at index `i` (throws on bad index) |
| `.empty()` | `true` if the string has zero characters |

### Practice

1\. 🌟 Fill the blanks with the correct values.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string s = "Hello";

    assert(s.length() == __);
    assert(s[0] == __);
    assert(s.empty() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## String Concatenation

### How it works

Use the `+` operator to join strings together. You can concatenate `std::string` objects, string literals, and even single characters.

```cpp
#include <iostream>
#include <string>

int main() {
    std::string greeting = "Hello" + std::string(" ") + "World";
    std::cout << greeting << std::endl;  // Hello World

    greeting += "!";
    std::cout << greeting << std::endl;  // Hello World!
    return 0;
}
```

| Operator | Effect |
|---|---|
| `s1 + s2` | Returns a new string that is the concatenation |
| `s += s2` | Appends `s2` to `s` in place |
| `" "` (literal) | Can be concatenated when at least one operand is `std::string` |

### Practice

2\. 🌟 Fill the blank so the result is `"Hello World"`.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string first = "Hello";
    std::string second = "World";
    std::string result = first + __ + second;

    assert(result == "Hello World");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## String Methods

### How it works

`std::string` provides methods for extracting substrings, finding patterns, and replacing text.

```cpp
#include <iostream>
#include <string>

int main() {
    std::string s = "Hello, World!";

    std::cout << s.substr(0, 5)  << std::endl;  // Hello
    std::cout << s.find("World") << std::endl;   // 7

    s.replace(7, 5, "C++");
    std::cout << s << std::endl;                 // Hello, C++!
    return 0;
}
```

| Method | Parameters | Returns / Effect |
|---|---|---|
| `.substr(pos, len)` | Start position, length | New string with the substring |
| `.find(str)` | String to search for | Index of first occurrence, or `std::string::npos` |
| `.replace(pos, len, str)` | Start, length to erase, replacement | Replaces characters in place |

### Practice

3\. 🌟🌟 Fill the blanks with the correct results of each method call.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string s = "Hello, World!";

    assert(s.substr(0, 5) == __);
    assert(s.find("World") == __);

    s.replace(7, 5, "C++");
    assert(s == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## C-strings vs std::string

### How it works

A C-string is a null-terminated `char` array. You **cannot** compare two C-strings with `==` because that compares pointer addresses, not content. Use `strcmp()` from `<cstring>` instead.

```cpp
#include <iostream>
#include <cstring>

int main() {
    char a[] = "hello";
    char b[] = "hello";

    // WRONG: compares addresses (likely false)
    if (a == b) std::cout << "equal (pointer)" << std::endl;

    // CORRECT: compares content
    if (std::strcmp(a, b) == 0) {
        std::cout << "equal (content)" << std::endl;  // this prints
    }
    return 0;
}
```

| Aspect | C-string (`char[]`) | `std::string` |
|---|---|---|
| Comparison | `strcmp(a, b) == 0` | `a == b` |
| Length | `strlen(a)` | `a.length()` |
| Memory | Fixed or manual allocation | Automatic |
| Null terminator | Required (`'\0'`) | Managed internally |

### Practice

4\. 🌟🌟 Fix: comparing C-strings with `==` compares addresses, not content. Use `strcmp`.

```cpp,editable
#include <iostream>
#include <cstring>
#include <cassert>

int main() {
    char a[] = "hello";
    char b[] = "hello";

    // This compares pointers, not content!
    assert(a == b);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Converting Between C-string and std::string

### How it works

You can construct a `std::string` from a C-string using the constructor, and get a C-string back with `.c_str()`.

```cpp
#include <iostream>
#include <string>
#include <cstring>

int main() {
    // C-string → std::string
    const char* cstr = "Hello";
    std::string cpp_str = cstr;    // implicit conversion

    // std::string → C-string
    const char* back = cpp_str.c_str();

    std::cout << back << std::endl;  // Hello
    std::cout << (std::strcmp(cstr, back) == 0) << std::endl;  // 1
    return 0;
}
```

| Direction | How |
|---|---|
| C-string → `std::string` | `std::string s = cstr;` or `std::string s(cstr);` |
| `std::string` → C-string | `s.c_str()` returns a `const char*` |

### Practice

5\. 🌟🌟 Fill the blanks to convert between C-string and `std::string`.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>
#include <cstring>

int main() {
    const char* cstr = "Hello";
    std::string cpp_str = __;          // C-string to std::string

    const char* back = cpp_str.__();   // std::string to C-string

    assert(std::strcmp(cstr, back) == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## String Iteration

### How it works

You can iterate over each character in a `std::string` with a range-based `for` loop, which is the cleanest way to process characters one by one.

```cpp
#include <iostream>
#include <string>

int main() {
    std::string text = "C++20";

    for (char c : text) {
        std::cout << c << " ";
    }
    std::cout << std::endl;  // C + + 2 0

    // Count digits
    int digits = 0;
    for (char c : text) {
        if (c >= '0' && c <= '9') digits++;
    }
    std::cout << "Digits: " << digits << std::endl;  // 2
    return 0;
}
```

| Technique | Syntax |
|---|---|
| Range-based for | `for (char c : text)` |
| By reference (modify) | `for (char& c : text)` |
| Index-based | `for (size_t i = 0; i < text.size(); i++)` |

### Practice

6\. 🌟🌟 Fill the blank: how many vowels are in `"Hello World"`?

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string text = "Hello World";
    int vowels = 0;

    for (char c : text) {
        c = std::tolower(c);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowels++;
        }
    }

    assert(vowels == __);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
