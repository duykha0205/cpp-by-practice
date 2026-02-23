# Pointers

Pointers are variables that store the memory address of another variable. They are one of the most powerful features in C++, enabling dynamic memory management, efficient data passing, and the construction of complex data structures like linked lists and trees.

## Pointer Basics

### How it works

```cpp
#include <iostream>

int main() {
    int x = 42;
    int* ptr = &x;       // ptr stores the address of x

    std::cout << "x = " << x << std::endl;              // 42
    std::cout << "&x = " << &x << std::endl;             // e.g., 0x7ffd...
    std::cout << "ptr = " << ptr << std::endl;            // same address
    std::cout << "*ptr = " << *ptr << std::endl;          // 42 (dereference)

    *ptr = 100;          // modify x through the pointer
    std::cout << "x after *ptr = 100: " << x << std::endl;  // 100

    return 0;
}
```

| Syntax | Meaning |
|--------|---------|
| `int* ptr` | Declare a pointer to `int` |
| `&x` | Address-of — returns the memory address of `x` |
| `*ptr` | Dereference — reads the value at the address |
| `*ptr = val` | Write a new value through the pointer |

### Practice

1. 🌟 Fill in the blank so that `ptr` points to `x`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 42;
    int* ptr = __;    // ptr should point to x

    assert(*ptr == 42);
    assert(ptr == &x);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟 Use the pointer to change `x` to 100.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 42;
    int* ptr = &x;

    __;  // Change x to 100 using ptr

    assert(x == 100);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Null Pointers

### How it works

```cpp
#include <iostream>

int main() {
    int* ptr = nullptr;   // pointer that points to nothing

    if (ptr != nullptr) {
        std::cout << *ptr << std::endl;
    } else {
        std::cout << "Pointer is null — skipping dereference." << std::endl;
    }

    int x = 42;
    ptr = &x;             // now it points somewhere valid

    if (ptr) {            // implicit bool conversion (nullptr → false)
        std::cout << "Value: " << *ptr << std::endl;  // 42
    }

    return 0;
}
```

- `nullptr` is the modern C++ null pointer literal (prefer over `NULL` or `0`)
- Dereferencing `nullptr` is **undefined behavior** — it typically crashes the program
- Pointers implicitly convert to `bool`: `nullptr` is `false`, any valid address is `true`

### Practice

3. 🌟🌟 Fix the bug: this program dereferences a null pointer, which is undefined behavior. Add a null check so the program runs safely.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* ptr = nullptr;

    // This line crashes! Add a null check before dereferencing
    std::cout << *ptr << std::endl;

    int x = 42;
    ptr = &x;
    assert(ptr != nullptr);
    assert(*ptr == 42);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Pointer Arithmetic

### How it works

```cpp
#include <iostream>

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;       // pointer to first element

    std::cout << "*ptr = " << *ptr << std::endl;             // 10
    std::cout << "*(ptr+1) = " << *(ptr + 1) << std::endl;  // 20
    std::cout << "*(ptr+3) = " << *(ptr + 3) << std::endl;  // 40

    // Traverse the array using pointer arithmetic
    for (int* p = arr; p < arr + 5; ++p) {
        std::cout << *p << " ";
    }
    std::cout << std::endl;  // 10 20 30 40 50

    return 0;
}
```

- Adding `n` to a pointer advances it by `n * sizeof(element)` bytes
- `*(ptr + n)` is equivalent to `ptr[n]`
- Pointer arithmetic is only valid within array bounds (or one-past-the-end)

### Practice

4. 🌟🌟 Fill in the blanks using your knowledge of pointer arithmetic.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    int* ptr = arr;

    assert(*ptr == __);
    assert(*(ptr + 2) == __);
    assert(*(ptr + 4) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Pointer to Pointer

### How it works

```cpp
#include <iostream>

int main() {
    int x = 42;
    int* p = &x;         // p points to x
    int** pp = &p;        // pp points to p

    std::cout << "x = " << x << std::endl;        // 42
    std::cout << "*p = " << *p << std::endl;       // 42
    std::cout << "**pp = " << **pp << std::endl;   // 42

    **pp = 99;            // modify x through two levels of indirection
    std::cout << "x = " << x << std::endl;         // 99

    return 0;
}
```

| Expression | Type | Value |
|------------|------|-------|
| `pp` | `int**` | Address of `p` |
| `*pp` | `int*` | Address of `x` (same as `p`) |
| `**pp` | `int` | Value of `x` |

### Practice

5. 🌟🌟🌟 Fill in the blank so that `pp` points to `p`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 42;
    int* p = &x;
    int** pp = __;

    assert(**pp == 42);
    assert(*pp == p);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Const Pointers

### How it works

```cpp
#include <iostream>

int main() {
    int x = 10, y = 20;

    // 1. Pointer to const int — can't modify the value through the pointer
    const int* ptr1 = &x;
    // *ptr1 = 30;         // ERROR: value is read-only
    ptr1 = &y;             // OK: pointer itself can be redirected
    std::cout << "*ptr1 = " << *ptr1 << std::endl;  // 20

    // 2. Const pointer to int — can't change where the pointer points
    int* const ptr2 = &x;
    *ptr2 = 30;            // OK: value can be modified
    // ptr2 = &y;          // ERROR: pointer is const
    std::cout << "*ptr2 = " << *ptr2 << std::endl;  // 30

    // 3. Const pointer to const int — can't modify either
    const int* const ptr3 = &x;
    // *ptr3 = 40;         // ERROR
    // ptr3 = &y;          // ERROR
    std::cout << "*ptr3 = " << *ptr3 << std::endl;  // 30

    return 0;
}
```

| Declaration | Modify value? | Redirect pointer? |
|-------------|:---:|:---:|
| `const int* ptr` | No | Yes |
| `int* const ptr` | Yes | No |
| `const int* const ptr` | No | No |

### Practice

6. 🌟🌟 Fix the two errors: each line violates its const constraint. Remove or comment out the offending lines.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10;
    int y = 20;

    const int* ptr1 = &x;   // pointer to const int
    *ptr1 = 30;              // ERROR: can't modify data through pointer-to-const

    int* const ptr2 = &x;   // const pointer to int
    ptr2 = &y;               // ERROR: can't redirect a const pointer

    assert(*ptr1 == 10);
    assert(*ptr2 == 10);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
