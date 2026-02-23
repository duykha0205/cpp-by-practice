# Variables

Variables are named storage locations in memory. In C++, every variable has a type that determines what values it can hold and how much memory it occupies. Understanding declaration, initialization, scope, and type deduction is essential for writing correct C++ programs.

## Declaration and Initialization

### How it works

Every variable must have a type. You should always initialize variables when you declare them — reading an uninitialized variable is **undefined behavior**.

```cpp
#include <iostream>
#include <cassert>

int main() {
    int age = 25;
    double pi = 3.14159;
    char grade = 'A';
    bool passed = true;

    // Declaration then assignment (two steps)
    int score;
    score = 100;  // assigned before use — OK

    assert(age == 25);
    assert(grade == 'A');
    assert(passed == true);
    assert(score == 100);

    std::cout << "Age: " << age << ", Pi: " << pi << std::endl;
    std::cout << "Grade: " << grade << ", Passed: " << passed << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Type | Holds | Example |
|------|-------|---------|
| `int` | Whole numbers | `42`, `-7` |
| `double` | Floating-point numbers | `3.14`, `-0.5` |
| `char` | Single character | `'A'`, `'9'` |
| `bool` | Boolean value | `true`, `false` |
| Uninitialized | Reading before assigning | **Undefined behavior** |

### Practice

1. 🌟 A variable must be initialized before use. Fix the code.

```cpp,editable
// Fix the error below with least amount of modification
#include <iostream>
#include <cassert>

int main() {
    int x;
    assert(x == 5);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟 Use the correct type for each variable.

```cpp,editable
// Fill the blanks to make it compile
#include <iostream>

int main() {
    __ x = 42;
    __ pi = 3.14;
    __ letter = 'A';
    __ flag = true;

    std::cout << x << " " << pi << " " << letter << " " << flag << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Const and Constexpr

### How it works

`const` makes a variable immutable after initialization — it can be set at runtime. `constexpr` goes further: the value must be computable at **compile time**. Functions called in a `constexpr` context must themselves be `constexpr`.

```cpp
#include <iostream>
#include <cassert>

constexpr int square(int n) {
    return n * n;
}

int main() {
    const int max_score = 100;       // immutable after initialization
    constexpr int board_size = 8;    // known at compile time
    constexpr int area = square(5);  // evaluated at compile time

    // max_score = 200;  // ERROR: can't modify a const
    // board_size = 10;  // ERROR: can't modify a constexpr

    assert(max_score == 100);
    assert(board_size == 8);
    assert(area == 25);

    std::cout << "Area of " << board_size << "x" << board_size
              << " = " << area << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Keyword | Meaning | Use when |
|---------|---------|----------|
| `const` | Cannot change after initialization | Runtime values that shouldn't change |
| `constexpr` | Must be computable at **compile time** | Array sizes, config constants |
| `constexpr` function | Can be evaluated at compile time | Pure computations with no side effects |

### Practice

3. 🌟 `const` makes a variable immutable. Fix the error.

```cpp,editable
// Fix the error
#include <iostream>
#include <cassert>

int main() {
    const int x = 10;
    x = 20;
    assert(x == 10);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟 `constexpr` requires a compile-time constant. Fix the error.

```cpp,editable
// Fix the error: constexpr requires a compile-time constant
#include <iostream>
#include <cassert>

int get_value() {
    return 42;
}

int main() {
    constexpr int x = get_value();
    assert(x == 42);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Scope

### How it works

A variable exists only within the block `{ }` where it is declared. When the block ends, the variable is destroyed. An inner variable with the same name as an outer one **shadows** the outer variable within that block.

```cpp
#include <iostream>
#include <cassert>

int main() {
    int x = 10;  // outer scope

    {
        int y = 20;  // inner scope — only visible inside these braces
        int x = 99;  // shadows the outer x

        assert(x == 99);  // inner x
        assert(y == 20);
    }
    // y is destroyed here; inner x is gone, outer x is back

    assert(x == 10);  // outer x, unchanged

    std::cout << "Outer x: " << x << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| Block scope | Variables live from declaration to the closing `}` |
| Lifetime | Variable is destroyed when its scope ends |
| Shadowing | An inner variable with the same name hides the outer one |

### Practice

5. 🌟 A variable is only accessible within its scope. Fix the error.

```cpp,editable
// Fix the error below with least amount of modification
#include <iostream>
#include <cassert>

int main() {
    int x = 10;
    {
        int y = 5;
        std::cout << "Inner: x=" << x << " y=" << y << std::endl;
    }
    std::cout << "Outer: x=" << x << " y=" << y << std::endl;
    assert(x == 10);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

6. 🌟🌟 Variables in inner scope can shadow outer ones. Fill the blanks.

```cpp,editable
// What does this print? Fill in the correct values.
#include <iostream>
#include <cassert>

int main() {
    int x = 5;
    {
        int x = 12;
        assert(x == __);
    }
    assert(x == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Auto Type Deduction

### How it works

The `auto` keyword tells the compiler to deduce the variable's type from its initializer. Be careful with brace initialization — `auto x = {42}` deduces `std::initializer_list<int>`, not `int`.

```cpp
#include <iostream>
#include <string>
#include <cassert>

int main() {
    auto i = 42;                        // int
    auto d = 3.14;                      // double
    auto s = std::string("hello");      // std::string
    auto b = true;                      // bool

    // auto with references
    int value = 10;
    auto& ref = value;  // int&
    ref = 20;
    assert(value == 20);  // modifying ref changes value

    // Pitfall: brace initialization
    // auto x = {42};   // std::initializer_list<int>, NOT int!
    auto x = 42;        // int — this is what you usually want

    assert(i == 42);
    assert(x == 42);
    std::cout << "i=" << i << " d=" << d << " s=" << s << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Syntax | Deduced type | Notes |
|--------|-------------|-------|
| `auto x = 42;` | `int` | Deduced from integer literal |
| `auto d = 3.14;` | `double` | Deduced from floating literal |
| `auto& r = x;` | `int&` | Reference to `x` |
| `auto x = {42};` | `std::initializer_list<int>` | Common pitfall — not `int`! |

### Practice

7. 🌟🌟 `auto` lets the compiler deduce the type. Fill the blanks.

```cpp,editable
// Fill the blanks with 'auto' to make it work
#include <iostream>
#include <string>

int main() {
    __ x = 42;          // int
    __ y = 3.14;        // double
    __ name = std::string("C++");

    std::cout << x << " " << y << " " << name << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

8. 🌟🌟 Be careful: `auto` with brace initialization. Fix the code.

```cpp,editable
// Fix the code so that x is an int, not std::initializer_list
#include <iostream>
#include <cassert>

int main() {
    auto x = {42};

    // We want x to be an int with value 42
    assert(x == 42);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Multiple Declarations

### How it works

You can declare multiple variables of the same type on a single line, separated by commas. Each variable can have its own initializer.

```cpp
#include <iostream>
#include <cassert>

int main() {
    int a = 1, b = 2, c = 3;
    double x = 1.5, y = 2.5;

    assert(a + b + c == 6);
    assert(x + y == 4.0);

    std::cout << "a=" << a << " b=" << b << " c=" << c << std::endl;
    std::cout << "x=" << x << " y=" << y << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Description |
|---------|-------------|
| Multiple declaration | Declare several variables of the same type, separated by commas |
| Each variable | Can have its own initializer |
| Readability | Keep it short — too many on one line hurts readability |

### Practice

9. 🌟 You can declare multiple variables of the same type. Fill the blank.

```cpp,editable
// Fill the blank to make it compile
#include <iostream>
#include <cassert>

int main() {
    int x = 1, y = 2, z = __;

    assert(x + y + z == 6);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
