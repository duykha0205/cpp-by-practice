# Inline & Constexpr Functions

C++ provides several ways to optimize and constrain function evaluation. **Inline** functions hint to the compiler to substitute the function body at the call site. **Constexpr** functions can be evaluated at compile time. **Consteval** (C++20) functions *must* be evaluated at compile time. **Static local variables** persist across function calls and are initialized exactly once.

## Inline Functions

### How it works

The `inline` keyword suggests the compiler replace a function call with the function body, avoiding call overhead. More importantly, `inline` allows a function to be defined in a header without violating the **One Definition Rule (ODR)** — each translation unit gets its own copy. The compiler may ignore the hint and not inline, or may inline functions even without the keyword.

```cpp
#include <iostream>
#include <cassert>

inline int square(int x) {
    return x * x;
}

inline int max_of(int a, int b) {
    return (a > b) ? a : b;
}

int main() {
    assert(square(5) == 25);
    assert(square(-3) == 9);
    assert(max_of(10, 20) == 20);

    std::cout << "square(5) = " << square(5) << std::endl;
    std::cout << "max_of(10, 20) = " << max_of(10, 20) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `inline` keyword | Hint to substitute function body at call site |
| ODR benefit | Allows function definition in headers without linker errors |
| Compiler decision | Compiler may ignore `inline` or inline non-`inline` functions |
| Best for | Small, frequently called functions |

### Practice

1\. 🌟 **Fill:** Add the `inline` keyword and fill the function body to return the cube of `x`.

```cpp,editable
#include <iostream>
#include <cassert>

// Add 'inline' and fill the body
__ int cube(int x) {
    __
}

int main() {
    assert(cube(3) == 27);
    assert(cube(2) == 8);
    assert(cube(0) == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Constexpr Functions

### How it works

A `constexpr` function **can** be evaluated at compile time if all its arguments are constant expressions. If called with runtime values, it behaves like a normal function. The result of a compile-time evaluation can be used as an array size, template argument, or `constexpr` variable initializer.

```cpp
#include <iostream>
#include <cassert>

constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int fibonacci(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    constexpr int fact5 = factorial(5);   // evaluated at compile time
    constexpr int fib7 = fibonacci(7);    // evaluated at compile time

    // Can also use result as array size
    constexpr int size = factorial(3);    // 6
    int arr[size] = {};                   // array of 6 elements

    assert(fact5 == 120);
    assert(fib7 == 13);
    assert(size == 6);

    // Also works with runtime values
    int n = 4;
    int result = factorial(n);  // evaluated at runtime
    assert(result == 24);

    std::cout << "5! = " << fact5 << ", fib(7) = " << fib7 << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Aspect | Detail |
|--------|--------|
| `constexpr` function | *Can* run at compile time if given constant args |
| Runtime fallback | Works as a normal function with non-constant args |
| Use case | Array sizes, template arguments, compile-time constants |
| Restrictions | Body must be valid for compile-time evaluation (no I/O, etc.) |

### Practice

2\. 🌟🌟 **Fill:** Make `sum_to` a `constexpr` function that returns the sum from 1 to n.

```cpp,editable
#include <iostream>
#include <cassert>

// Add constexpr and fill the body (use the formula: n*(n+1)/2)
__ int sum_to(int n) {
    __
}

int main() {
    constexpr int result = sum_to(10);
    assert(result == 55);

    constexpr int small = sum_to(1);
    assert(small == 1);

    // Use as array size to prove it's compile-time
    constexpr int sz = sum_to(4);  // 10
    int arr[sz] = {};
    assert(sz == 10);

    std::cout << "sum_to(10) = " << result << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Consteval Functions

### How it works

C++20 introduces `consteval` for **immediate functions** — they *must* be evaluated at compile time. Unlike `constexpr`, calling a `consteval` function with a runtime value is a compiler error. This is useful when you want to guarantee that a computation happens at compile time.

```cpp
#include <iostream>
#include <cassert>

consteval int square(int x) {
    return x * x;
}

consteval int compile_time_add(int a, int b) {
    return a + b;
}

int main() {
    constexpr int s = square(7);           // OK: compile-time argument
    constexpr int sum = compile_time_add(10, 20);  // OK

    // int n = 5;
    // int bad = square(n);  // ERROR: n is not a compile-time constant

    assert(s == 49);
    assert(sum == 30);

    std::cout << "square(7) = " << s << std::endl;
    std::cout << "10 + 20 = " << sum << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Keyword | Compile-time required? | Runtime allowed? |
|---------|----------------------|------------------|
| `inline` | No | Yes |
| `constexpr` | If args are constant | Yes (falls back to runtime) |
| `consteval` | Always | No (compiler error) |

### Practice

3\. 🌟🌟🌟 **Fix:** The function should only be callable at compile time. Change `constexpr` to `consteval` and fix the call that uses a runtime variable.

```cpp,editable
#include <iostream>
#include <cassert>

// Change this to consteval
constexpr int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

int main() {
    constexpr int a = power(2, 10);  // OK: compile-time
    assert(a == 1024);

    // Fix this: consteval cannot accept runtime variables
    // Either make it constexpr or use a literal
    int exp = 3;
    int b = power(2, exp);
    assert(b == 8);

    std::cout << "2^10 = " << a << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Static Local Variables

### How it works

A `static` local variable inside a function is initialized **once** — on the first call — and persists until the program ends. Subsequent calls reuse the same variable. This is useful for caching, counting calls, or one-time initialization.

```cpp
#include <iostream>
#include <cassert>

int call_counter() {
    static int count = 0;  // initialized once, persists across calls
    count++;
    return count;
}

int get_id() {
    static int next_id = 100;
    return next_id++;
}

int main() {
    assert(call_counter() == 1);
    assert(call_counter() == 2);
    assert(call_counter() == 3);

    assert(get_id() == 100);
    assert(get_id() == 101);
    assert(get_id() == 102);

    std::cout << "Counter: " << call_counter() << std::endl;  // 4
    std::cout << "Next ID: " << get_id() << std::endl;        // 103
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `static` local | Declared inside function, but lifetime is the whole program |
| Initialization | Happens once on first call, skipped on subsequent calls |
| Persistence | Value is preserved between function calls |
| Use case | Counters, caching, one-time setup |

### Practice

4\. 🌟🌟 **Fill:** Complete the function so it uses a static variable to track the running sum of all values passed to it.

```cpp,editable
#include <iostream>
#include <cassert>

int accumulate(int value) {
    // Declare a static local variable 'total' initialized to 0
    __
    total += value;
    return total;
}

int main() {
    assert(accumulate(10) == 10);
    assert(accumulate(20) == 30);
    assert(accumulate(5) == 35);
    assert(accumulate(15) == 50);

    std::cout << "Running total: " << accumulate(0) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟🌟 **Complete:** Write a function `make_unique_id` that returns a different string each time, using a static counter. The format should be `"ID-1"`, `"ID-2"`, etc.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

std::string make_unique_id() {
    // Use a static counter to generate "ID-1", "ID-2", etc.
    __
}

int main() {
    assert(make_unique_id() == "ID-1");
    assert(make_unique_id() == "ID-2");
    assert(make_unique_id() == "ID-3");

    std::cout << "Next: " << make_unique_id() << std::endl;  // ID-4
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
