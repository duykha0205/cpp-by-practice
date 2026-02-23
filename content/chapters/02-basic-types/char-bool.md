# Char, Bool and Void

Characters, booleans, and void are fundamental types in C++. A `char` stores a single character (and is actually a small integer under the hood), `bool` holds `true` or `false`, and `void` represents the absence of a type — used for functions that return nothing.

## Char

### How it works

A `char` is exactly 1 byte and stores a single character. Under the hood, it holds an integer — the character's ASCII code. This means you can do arithmetic on characters.

```cpp
#include <iostream>
#include <cassert>

int main() {
    char letter = 'A';
    char digit = '7';

    assert(sizeof(char) == 1);  // always 1 byte
    assert(letter == 'A');

    // char is actually a small integer
    int ascii_A = letter;
    assert(ascii_A == 65);

    std::cout << "'" << letter << "' has ASCII value " << ascii_A << std::endl;
    std::cout << "'" << digit << "' has ASCII value " << static_cast<int>(digit) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| `char` | Stores a single character, exactly 1 byte |
| Character literal | Written with single quotes: `'A'`, `'0'` |
| Integer underneath | `'A'` is stored as `65` (its ASCII code) |
| `sizeof(char)` | Always `1` by definition |

### Practice

1. 🌟 Fill in the blanks for `char` basics.

```cpp,editable
// Fill the blanks
#include <iostream>
#include <cassert>

int main() {
    char c = 'A';
    assert(c == __);
    assert(sizeof(char) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## ASCII Values

### How it works

Every character maps to an integer via the ASCII table. You can convert between `char` and `int` freely, and arithmetic on characters follows the ASCII values — `'A' + 1` gives `'B'`, and `c - '0'` converts a digit character to its numeric value.

```cpp
#include <iostream>
#include <cassert>

int main() {
    char c = 'A';
    int ascii = c;           // implicit conversion char → int
    assert(ascii == 65);     // 'A' = 65 in ASCII

    // Arithmetic on characters
    char next = c + 1;       // 'A' + 1 = 'B'
    assert(next == 'B');

    char lower = c + 32;     // uppercase to lowercase
    assert(lower == 'a');    // 'a' = 97 = 65 + 32

    // Convert digit character to numeric value
    char d = '5';
    int digit_value = d - '0';  // '5' - '0' = 53 - 48 = 5
    assert(digit_value == 5);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Expression | Value | Why |
|------------|-------|-----|
| `'A'` → `int` | `65` | ASCII code of `'A'` |
| `'A' + 1` | `'B'` (66) | Next letter in ASCII |
| `'A' + 32` | `'a'` (97) | Lowercase offset is 32 |
| `'5' - '0'` | `5` | Digit characters are consecutive in ASCII |

### Practice

2. 🌟🌟 Characters are integers underneath. Fill the blank.

```cpp,editable
// Fill the blank to make it work
#include <iostream>
#include <cassert>

int main() {
    char c = 'A';
    int ascii_value = c;

    assert(ascii_value == __);  // What is the ASCII value of 'A'?

    char next = c + 1;
    assert(next == 'B');

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Escape Characters

### How it works

Some characters can't be typed directly in a string — they need an **escape sequence** starting with `\`. Common ones include `\n` (newline), `\t` (tab), `\"` (double quote), and `\\` (literal backslash).

```cpp
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::cout << "Line 1\nLine 2" << std::endl;      // \n = newline
    std::cout << "Col1\tCol2" << std::endl;           // \t = tab
    std::cout << "She said \"Hi\"" << std::endl;      // \" = literal double quote
    std::cout << "Path: C:\\Users" << std::endl;      // \\ = literal backslash

    // Each escape sequence counts as ONE character
    std::string quote = "She said \"Hello\"";
    assert(quote.size() == 16);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Escape | Meaning | Example output |
|--------|---------|----------------|
| `\n` | Newline | Line break |
| `\t` | Tab | Horizontal tab |
| `\"` | Double quote | `"` |
| `\\` | Backslash | `\` |
| `\'` | Single quote | `'` |

### Practice

3. 🌟🌟 Fix the string so it contains double quotes around Hello.

```cpp,editable
// Fix: the string should read  She said "Hello"
#include <iostream>
#include <cassert>
#include <string>

int main() {
    std::string s = "She said Hello";
    assert(s == "She said \"Hello\"");
    std::cout << s << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Bool

### How it works

A `bool` holds one of two values: `true` or `false`. When converted to an integer, `true` becomes `1` and `false` becomes `0`. Despite holding only one bit of information, `sizeof(bool)` is `1` (one byte).

```cpp
#include <iostream>
#include <cassert>

int main() {
    bool is_ready = true;
    bool is_done = false;

    assert(is_ready == true);
    assert(is_done == false);

    // true converts to 1, false converts to 0
    assert(static_cast<int>(true) == 1);
    assert(static_cast<int>(false) == 0);

    assert(sizeof(bool) == 1);  // 1 byte

    std::cout << std::boolalpha;  // print "true"/"false" instead of 1/0
    std::cout << "is_ready: " << is_ready << std::endl;
    std::cout << "is_done: " << is_done << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| `true` | Converts to `1` |
| `false` | Converts to `0` |
| `sizeof(bool)` | `1` byte |
| `std::boolalpha` | Makes `cout` print `"true"` / `"false"` |

### Practice

4. 🌟 `bool` can be `true` or `false`. Fill the blanks.

```cpp,editable
// Fill the blanks
#include <iostream>
#include <cassert>

int main() {
    bool t = true;
    bool f = false;

    assert(t == __);    // true is 1
    assert(f == __);    // false is 0
    assert(sizeof(bool) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Bool Conversions

### How it works

Integers convert to `bool` by a simple rule: `0` is `false`, and **any nonzero value** is `true`. This works for negative numbers too.

```cpp
#include <iostream>
#include <cassert>

int main() {
    // Integer → bool: 0 is false, everything else is true
    assert(static_cast<bool>(0) == false);
    assert(static_cast<bool>(1) == true);
    assert(static_cast<bool>(-1) == true);
    assert(static_cast<bool>(42) == true);
    assert(static_cast<bool>(-999) == true);

    // In conditions, this happens implicitly
    int x = 42;
    if (x) {
        std::cout << "x is truthy" << std::endl;
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Value | `static_cast<bool>(value)` |
|-------|---------------------------|
| `0` | `false` |
| `1` | `true` |
| `-1` | `true` |
| `42` | `true` |
| Any nonzero | `true` |

### Practice

5. 🌟🌟 Integers convert to bool: 0 is false, anything else is true. Fill the blanks.

```cpp,editable
// Fill the blanks to make it work
#include <iostream>
#include <cassert>

int main() {
    assert(static_cast<bool>(0) == __);
    assert(static_cast<bool>(1) == __);
    assert(static_cast<bool>(-1) == __);
    assert(static_cast<bool>(42) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Logical Operators

### How it works

The logical operators `&&` (AND), `||` (OR), and `!` (NOT) work on boolean values. C++ uses **short-circuit evaluation**: in `a && b`, if `a` is false, `b` is never evaluated; in `a || b`, if `a` is true, `b` is never evaluated.

```cpp
#include <iostream>
#include <cassert>

int main() {
    // AND: both must be true
    assert((true && true) == true);
    assert((true && false) == false);

    // OR: at least one must be true
    assert((true || false) == true);
    assert((false || false) == false);

    // NOT: flips the value
    assert(!true == false);
    assert(!false == true);

    // Short-circuit evaluation
    int x = 0;
    bool result = (x != 0) && (10 / x > 1);  // 10/x is NOT evaluated
    assert(result == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Operator | Name | `true, true` | `true, false` | `false, false` |
|----------|------|-------------|---------------|----------------|
| `&&` | AND | `true` | `false` | `false` |
| `\|\|` | OR | `true` | `true` | `false` |
| `!` | NOT | `false` | — | `true` |

### Practice

6. 🌟🌟 Logical operators with bool. Fill the blanks.

```cpp,editable
// Fill the blanks
#include <iostream>
#include <cassert>

int main() {
    assert((true && false) == __);
    assert((true || false) == __);
    assert((!true) == __);
    assert((true && true) == __);
    assert((false || false) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Void

### How it works

`void` means "no type." It is used for functions that perform an action but don't return a value. A `void` function can use a bare `return;` to exit early, but must **not** return a value.

```cpp
#include <iostream>
#include <cassert>

void print_hello() {
    std::cout << "Hello, C++!" << std::endl;
    // no return value needed (or use bare 'return;')
}

int add(int a, int b) {
    return a + b;  // non-void functions MUST return a value
}

int main() {
    print_hello();
    int sum = add(3, 4);
    assert(sum == 7);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| `void` function | Returns nothing — cannot use `return expr;` |
| Bare `return;` | Allowed in `void` functions to exit early |
| Non-`void` function | Must return a value of the declared type |

### Practice

7. 🌟 A `void` function cannot return a value. Fix the error.

```cpp,editable
// Fix the error
#include <iostream>
#include <cassert>

void greet() {
    std::cout << "Hello, C++!" << std::endl;
    return 42;
}

int main() {
    greet();
    assert(true);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
