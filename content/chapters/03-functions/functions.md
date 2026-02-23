# Functions

Functions let you group reusable logic behind a name. They take inputs (parameters), do work, and optionally return a result. Mastering functions is the first step toward writing clean, modular C++ code.

## Function Basics

### How it works

A function has five parts: a return type, a name, a parameter list, a body, and a return statement. If a function is called before its definition, you need a **forward declaration** so the compiler knows it exists.

```cpp
#include <iostream>

// Forward declaration — lets main() call greet() before its definition
void greet(const std::string& name);

int add(int a, int b) {
    return a + b;
}

int main() {
    std::cout << add(3, 4) << std::endl;   // 7
    greet("Alice");                         // Hello, Alice!
    return 0;
}

void greet(const std::string& name) {
    std::cout << "Hello, " << name << "!" << std::endl;
}
```

| Component | Description |
|---|---|
| Return type | The type of value the function gives back (`int`, `void`, …) |
| Name | Identifier used to call the function |
| Parameters | Comma-separated inputs inside `()` |
| Body | Code block `{}` that runs when the function is called |
| Return statement | `return expr;` sends a value back to the caller |
| Forward declaration | Signature followed by `;` placed before first use |

### Practice

1\. 🌟 Fill the body so the function returns the sum of two integers.

```cpp,editable
#include <iostream>
#include <cassert>

int add(int a, int b) {
    __
}

int main() {
    assert(add(2, 3) == 5);
    assert(add(-1, 1) == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2\. 🌟 Fix: the function is used before it is declared. Add a forward declaration.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    assert(multiply(3, 4) == 12);
    std::cout << "Success!" << std::endl;
    return 0;
}

int multiply(int a, int b) {
    return a * b;
}
```

## Default Parameters

### How it works

You can give parameters default values. When the caller omits those arguments the defaults kick in. Default parameters must appear at the **end** of the parameter list.

```cpp
#include <iostream>

int power(int base, int exp = 2) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int main() {
    std::cout << power(3)      << std::endl;  // 9   (exp defaults to 2)
    std::cout << power(2, 10)  << std::endl;  // 1024
    return 0;
}
```

| Rule | Example |
|---|---|
| Defaults go at the end | `void f(int a, int b = 0)` ✔ |
| Cannot skip middle params | `void f(int a = 0, int b)` ✘ |
| Caller can override | `power(2, 10)` overrides default `exp` |

### Practice

3\. 🌟🌟 Fill the blank to give `power` a default exponent of 2.

```cpp,editable
#include <iostream>
#include <cassert>

int power(int base, int exp __) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int main() {
    assert(power(3) == 9);        // uses default exp=2
    assert(power(2, 10) == 1024);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Function Overloading

### How it works

C++ lets you define several functions with the **same name** as long as their parameter lists differ in number or type. The compiler picks the right one based on the arguments you pass.

```cpp
#include <iostream>
#include <cmath>

double area(double length, double width) {
    return length * width;
}

double area(double radius) {
    return M_PI * radius * radius;
}

int main() {
    std::cout << area(5.0, 3.0) << std::endl;  // 15      (rectangle)
    std::cout << area(1.0)      << std::endl;   // 3.14159 (circle)
    return 0;
}
```

| Rule | Detail |
|---|---|
| Different param count | `area(double)` vs `area(double, double)` |
| Different param types | `print(int)` vs `print(double)` |
| Return type alone is NOT enough | Two functions differing only in return type won't compile |

### Practice

4\. 🌟🌟 Create an overloaded `area()` for circles: `area(radius)` returns `PI * r * r`.

```cpp,editable
#include <iostream>
#include <cassert>
#include <cmath>

double area(double length, double width) {
    return length * width;
}

// Add an overload: area(double radius) that returns PI * r * r
__

int main() {
    assert(std::abs(area(5.0, 3.0) - 15.0) < 0.001);
    assert(std::abs(area(1.0) - M_PI) < 0.001);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Pass by Value vs Reference

### How it works

By default, C++ **copies** each argument into the function (pass by value). To let the function modify the original variable, pass by **reference** (`&`). To avoid the cost of copying without allowing modification, use **const reference** (`const &`).

```cpp
#include <iostream>
#include <string>

void increment(int& n) {          // reference — modifies the original
    n++;
}

void print(const std::string& s) { // const ref — no copy, no modification
    std::cout << s << std::endl;
}

int main() {
    int x = 5;
    increment(x);
    std::cout << x << std::endl;   // 6

    std::string msg = "Hello!";
    print(msg);                     // Hello!
    return 0;
}
```

| Passing style | Syntax | Copies? | Can modify original? |
|---|---|---|---|
| By value | `void f(int n)` | Yes | No |
| By reference | `void f(int& n)` | No | Yes |
| By const reference | `void f(const int& n)` | No | No |

### Practice

5\. 🌟🌟 Fix: `swap` doesn't work because parameters are passed by value. Change them to references.

```cpp,editable
#include <iostream>
#include <cassert>

void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 1, y = 2;
    swap(x, y);

    assert(x == 2);
    assert(y == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

6\. 🌟🌟 Fix: pass the string by `const` reference instead of by value to avoid an unnecessary copy.

```cpp,editable
#include <iostream>
#include <string>

void print_length(std::string s) {
    std::cout << "Length: " << s.length() << std::endl;
}

int main() {
    std::string long_text = "This is a very long string that we don't want to copy";
    print_length(long_text);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Recursion

### How it works

A recursive function calls itself. Every recursive function needs a **base case** (when to stop) and a **recursive case** (how to reduce the problem).

```cpp
#include <iostream>

int factorial(int n) {
    if (n <= 1) {       // base case
        return 1;
    }
    return n * factorial(n - 1);  // recursive case
}

int main() {
    std::cout << "5! = " << factorial(5) << std::endl;  // 120
    return 0;
}
```

| Concept | Role |
|---|---|
| Base case | Stops the recursion (`n <= 1 → return 1`) |
| Recursive case | Breaks the problem into a smaller sub-problem |
| Call stack | Each call adds a frame; too many calls cause stack overflow |

### Practice

7\. 🌟🌟🌟 Fill the blanks to compute factorial recursively: the base case and the recursive call.

```cpp,editable
#include <iostream>
#include <cassert>

int factorial(int n) {
    if (n <= __) {
        return __;
    }
    return n * factorial(__);
}

int main() {
    assert(factorial(0) == 1);
    assert(factorial(1) == 1);
    assert(factorial(5) == 120);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
