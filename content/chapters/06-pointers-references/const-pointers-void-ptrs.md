# Const Pointers, Void Pointers & nullptr

Understanding `const` correctness with pointers, the generic `void*` pointer, and the modern `nullptr` keyword are essential for writing safe and expressive C++ code. These concepts appear everywhere — from function signatures that promise not to modify data, to low-level APIs that accept any pointer type, to the type-safe null pointer that replaced the old `NULL` macro.

## Const Correctness with Pointers

### How it works

The position of `const` relative to the `*` determines what is constant. Read the declaration **right to left** to decode it: `const int* p` reads as "p is a pointer to a const int", while `int* const p` reads as "p is a const pointer to an int".

```cpp
#include <iostream>

int main() {
    int x = 10, y = 20;

    // 1. Pointer to const int — can read, cannot modify value through pointer
    const int* ptr1 = &x;
    // *ptr1 = 30;       // ERROR: value is read-only
    ptr1 = &y;           // OK: pointer itself can change
    std::cout << "*ptr1 = " << *ptr1 << std::endl;  // 20

    // 2. Const pointer to int — pointer is fixed, value can change
    int* const ptr2 = &x;
    *ptr2 = 30;           // OK: value can be modified
    // ptr2 = &y;         // ERROR: pointer is const
    std::cout << "*ptr2 = " << *ptr2 << std::endl;  // 30

    // 3. Const pointer to const int — nothing can change
    const int* const ptr3 = &x;
    // *ptr3 = 40;        // ERROR: value is const
    // ptr3 = &y;         // ERROR: pointer is const
    std::cout << "*ptr3 = " << *ptr3 << std::endl;  // 30

    return 0;
}
```

| Declaration | Read right-to-left | Modify value? | Redirect pointer? |
|---|---|:---:|:---:|
| `const int* p` | p is a pointer to a const int | No | Yes |
| `int* const p` | p is a const pointer to an int | Yes | No |
| `const int* const p` | p is a const pointer to a const int | No | No |
| `int const* p` | Same as `const int* p` | No | Yes |

### Practice

1\. 🌟 **Fill:** the blanks to identify what each const pointer declaration allows.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int a = 10, b = 20;

    const int* p1 = &a;
    // Can we do *p1 = 30?  Fill "true" or "false"
    bool can_modify_value_p1 = __;
    // Can we do p1 = &b?
    bool can_redirect_p1 = __;

    int* const p2 = &a;
    // Can we do *p2 = 30?
    bool can_modify_value_p2 = __;
    // Can we do p2 = &b?
    bool can_redirect_p2 = __;

    assert(can_modify_value_p1 == false);
    assert(can_redirect_p1 == true);
    assert(can_modify_value_p2 == true);
    assert(can_redirect_p2 == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2\. 🌟🌟 **Fix:** each line that violates its const constraint. Remove or comment out the offending lines so the code compiles and the assertions pass.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 10, y = 20;

    const int* ptr1 = &x;
    *ptr1 = 50;           // ERROR: cannot modify through pointer-to-const
    ptr1 = &y;            // OK

    int* const ptr2 = &x;
    *ptr2 = 30;           // OK
    ptr2 = &y;            // ERROR: cannot redirect a const pointer

    const int* const ptr3 = &x;
    *ptr3 = 99;           // ERROR: both value and pointer are const
    ptr3 = &y;            // ERROR: both value and pointer are const

    assert(*ptr1 == 20);
    assert(*ptr2 == 30);
    assert(*ptr3 == 30);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Void Pointers

### How it works

A `void*` is a generic pointer that can hold the address of any data type. However, because it has no type information, you **must** cast it back to the correct type before dereferencing. Void pointers are commonly used in C-style APIs and low-level memory functions.

```cpp
#include <iostream>

int main() {
    int x = 42;
    double d = 3.14;

    void* vp = &x;         // void* can point to any type
    // std::cout << *vp;    // ERROR: can't dereference void*

    // Must cast before dereferencing
    int* ip = static_cast<int*>(vp);
    std::cout << *ip << std::endl;  // 42

    vp = &d;                // can reassign to a different type
    double* dp = static_cast<double*>(vp);
    std::cout << *dp << std::endl;  // 3.14

    return 0;
}
```

| Aspect | Details |
|---|---|
| Declaration | `void* vp;` |
| Assigning | Can store address of any type without cast |
| Dereferencing | **Cannot** dereference directly — must cast first |
| Casting | Use `static_cast<T*>(vp)` to convert back |
| Arithmetic | **Cannot** do pointer arithmetic on `void*` (size unknown) |
| Use cases | Generic APIs, memory allocators, type erasure |

### Practice

3\. 🌟🌟 **Complete:** cast the void pointer to the correct type and dereference it to retrieve the stored value.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int x = 100;
    double d = 2.718;
    char c = 'A';

    void* vp;

    vp = &x;
    int val_int = *static_cast<__*>(vp);
    assert(val_int == 100);

    vp = &d;
    double val_dbl = *static_cast<__*>(vp);
    assert(val_dbl > 2.71 && val_dbl < 2.72);

    vp = &c;
    char val_chr = *static_cast<__*>(vp);
    assert(val_chr == 'A');

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## nullptr vs NULL vs 0

### How it works

In legacy C++, `NULL` is typically defined as `0` (an integer), which can cause ambiguity when overloaded functions accept both `int` and pointer types. C++11 introduced `nullptr`, which has its own type `std::nullptr_t` and converts only to pointer types — never to integers.

```cpp
#include <iostream>

void process(int value) {
    std::cout << "int: " << value << std::endl;
}

void process(int* ptr) {
    std::cout << "pointer: " << ptr << std::endl;
}

int main() {
    // process(NULL);      // AMBIGUOUS: NULL is 0, matches both overloads
    process(nullptr);      // Calls process(int*) — unambiguous
    process(0);            // Calls process(int)

    int* p = nullptr;
    if (p == nullptr) {
        std::cout << "p is null" << std::endl;
    }

    // nullptr has type std::nullptr_t
    std::nullptr_t np = nullptr;
    int* q = np;           // OK: nullptr_t converts to any pointer type
    // int n = np;         // ERROR: nullptr_t does not convert to int

    return 0;
}
```

| Null representation | Type | Ambiguity risk |
|---|---|---|
| `0` | `int` | High — may match integer overload |
| `NULL` | Implementation-defined (usually `0` or `0L`) | High — same issue |
| `nullptr` | `std::nullptr_t` | None — only matches pointer types |

### Practice

4\. 🌟🌟 **Fix:** replace `NULL` and `0` with `nullptr` where a null pointer is intended, so the correct overload is called.

```cpp,editable
#include <iostream>
#include <cassert>

int overloaded(int value) {
    return 1;  // integer version
}

int overloaded(int* ptr) {
    return 2;  // pointer version
}

int main() {
    // Fix: these should call the pointer version (return 2)
    int result1 = overloaded(NULL);   // ambiguous or wrong overload
    int result2 = overloaded(0);      // calls int version, not pointer

    assert(result1 == 2);
    assert(result2 == 2);

    // Also fix this null pointer initialization
    int* p = 0;
    assert(p == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟🌟 **Complete:** write a function that safely dereferences a pointer using const correctness and nullptr checks. Fill in the function signature and body.

```cpp,editable
#include <iostream>
#include <cassert>

// Fill: write a function safe_deref that:
// - Takes a pointer-to-const-int (so it cannot modify the value)
// - Takes a default_value (int) to return if the pointer is null
// - Returns the dereferenced value, or default_value if null
__ safe_deref(__, int default_value) {
    if (__ == __) {
        return __;
    }
    return __;
}

int main() {
    int x = 42;
    const int y = 99;

    assert(safe_deref(&x, -1) == 42);
    assert(safe_deref(&y, -1) == 99);
    assert(safe_deref(nullptr, -1) == -1);
    assert(safe_deref(nullptr, 0) == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
