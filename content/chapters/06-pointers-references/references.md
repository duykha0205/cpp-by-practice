# References

A reference in C++ is an alias — another name for an existing variable. Unlike pointers, references cannot be null, must be initialized at declaration, and do not require explicit dereferencing. They provide a cleaner and safer way to pass data to functions and work with indirection.

## Reference Basics

### How it works

```cpp
#include <iostream>

int main() {
    int x = 10;
    int& ref = x;     // ref is an alias for x — they share the same memory

    std::cout << "x = " << x << std::endl;       // 10
    std::cout << "ref = " << ref << std::endl;    // 10

    ref = 20;          // modifying ref modifies x
    std::cout << "x = " << x << std::endl;        // 20

    int y = 99;
    ref = y;           // copies y's VALUE into x (does NOT rebind ref to y)
    std::cout << "x = " << x << std::endl;        // 99

    return 0;
}
```

- `int& ref = x` creates a reference — `ref` and `x` refer to the same memory location
- A reference **must** be initialized when declared — `int& ref;` is a compile error
- Assigning to a reference changes the original variable's value
- A reference **cannot** be rebound to refer to a different variable after initialization

### Practice

1. 🌟 Fill in the blanks so the reference aliases `x`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10;
    int& ref = __;  // ref is a reference to x

    ref = 20;
    assert(x == __);  // x changed because ref is an alias

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟 Fix the error: a reference must be initialized at declaration.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10;
    int& ref;  // ERROR: references must be initialized

    ref = 20;
    assert(x == 20);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## References vs Pointers

### How it works

```cpp
#include <iostream>

// Pointer version — requires &, *, and explicit address handling
void swap_ptr(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Reference version — cleaner syntax, no null possible
void swap_ref(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    int x = 1, y = 2;

    swap_ptr(&x, &y);     // pointer version: must pass addresses
    std::cout << "After swap_ptr: x=" << x << ", y=" << y << std::endl;  // x=2, y=1

    swap_ref(x, y);        // reference version: natural call syntax
    std::cout << "After swap_ref: x=" << x << ", y=" << y << std::endl;  // x=1, y=2

    return 0;
}
```

| Feature | Pointer | Reference |
|---------|---------|-----------|
| Syntax | `int* ptr = &x; *ptr = 10;` | `int& ref = x; ref = 10;` |
| Can be null? | Yes (`nullptr`) | No |
| Can be reassigned? | Yes | No |
| Must be initialized? | No (but should be) | Yes |

### Practice

3. 🌟🌟 Rewrite the swap function using references instead of pointers. Update the call site too.

```cpp,editable
#include <iostream>
#include <cassert>

// Rewrite this function to use references instead of pointers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int x = 1, y = 2;
    swap(&x, &y);  // Update the call too

    assert(x == 2);
    assert(y == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Const References

### How it works

```cpp
#include <iostream>

int get_value() { return 42; }

int main() {
    int x = 10;

    const int& cref = x;          // const ref to lvalue — OK
    // cref = 20;                 // ERROR: can't modify through const ref

    const int& temp = get_value(); // const ref extends temporary's lifetime
    std::cout << "temp = " << temp << std::endl;  // 42

    // int& bad = get_value();    // ERROR: non-const ref can't bind to rvalue

    const int& lit = 100;         // const ref to literal — also OK
    std::cout << "lit = " << lit << std::endl;    // 100

    return 0;
}
```

- A `const` reference can bind to an **rvalue** (temporary) — the compiler extends the temporary's lifetime
- A non-const reference **cannot** bind to an rvalue — it would allow modifying a value about to disappear
- Use `const&` for function parameters to avoid unnecessary copies without allowing modification

### Practice

4. 🌟🌟 Fix the error: a non-const reference cannot bind to a temporary (rvalue).

```cpp,editable
#include <iostream>
#include <cassert>

int get_value() {
    return 42;
}

int main() {
    int& ref = get_value();  // ERROR: can't bind non-const ref to rvalue

    assert(ref == 42);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Dangling References

### How it works

```cpp
#include <iostream>

int& dangerous() {
    int x = 42;
    return x;           // WARNING: x is destroyed when function returns
}                        // The reference now dangles — undefined behavior!

int& safe() {
    static int x = 42;  // static variables live for the entire program
    return x;            // safe to return a reference
}

int main() {
    // int& bad = dangerous();  // undefined behavior — don't do this!

    int& good = safe();
    std::cout << "good = " << good << std::endl;  // 42

    return 0;
}
```

- A **dangling reference** refers to memory that has been freed (e.g., a local variable after the function returns)
- Using a dangling reference is **undefined behavior** — it may seem to work but is unreliable
- To return a reference safely, the object must outlive the function call (e.g., `static` variable, heap memory, or a reference parameter)

### Practice

5. 🌟🌟 Fix the dangling reference: `get_local()` returns a reference to a local variable that gets destroyed.

```cpp,editable
#include <iostream>
#include <cassert>

int& get_local() {
    int x = 42;
    return x;  // DANGER: x is destroyed when function returns!
}

int main() {
    int& ref = get_local();
    assert(ref == 42);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
