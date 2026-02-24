# Memory Pitfalls & Best Practices

Even with smart pointers available, C++ programmers must understand common memory bugs — dangling pointers, use-after-free, and double frees. This chapter shows how to recognize these pitfalls and use modern tools like `make_unique`, `make_shared`, and custom deleters to write safer code.

## Common Memory Bugs

The three classic memory bugs are **dangling pointers** (pointing to freed memory), **use-after-free** (dereferencing a dangling pointer), and **double free** (calling `delete` twice on the same pointer). All three cause undefined behavior and can crash your program — or worse, silently corrupt data.

### How it works

```cpp
#include <iostream>

int main() {
    // Dangling pointer: p still points to freed memory
    int* p = new int(42);
    delete p;
    // p is now dangling — DO NOT use *p

    // Double free: deleting the same pointer twice
    int* q = new int(10);
    delete q;
    // delete q;  // CRASH: undefined behavior

    // Prevention: set pointer to nullptr after delete
    int* r = new int(99);
    delete r;
    r = nullptr;  // safe — deleting nullptr is a no-op
    delete r;     // OK, does nothing

    std::cout << "No crashes!" << std::endl;
    return 0;
}
```

| Bug | Cause | Prevention |
|-----|-------|------------|
| Dangling pointer | Pointer still held after `delete` | Set to `nullptr` after delete, or use smart pointers |
| Use-after-free | Dereferencing a pointer after its memory is freed | Never access deleted memory; use smart pointers |
| Double free | Calling `delete` on the same pointer twice | Set to `nullptr` after delete; use `unique_ptr` |
| Memory leak | Forgetting to call `delete` | Use smart pointers with RAII |

### Practice

1. :star::star: **Fix:** The code uses a pointer after it has been deleted, causing undefined behavior. Reorganize the code so the pointer is used before deletion, and set it to `nullptr` after.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* p = new int(42);
    delete p;

    // BUG: use-after-free — p was already deleted
    int value = *p;
    assert(value == 42);

    // BUG: p is not nullified, risk of dangling pointer
    assert(p == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. :star::star: **Fix:** The code deletes the same pointer twice. Fix it by setting the pointer to `nullptr` after the first delete.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* p = new int(100);
    int* q = p;  // q and p point to the same memory

    delete p;
    // BUG: double free — q still points to the deleted memory
    delete q;

    // Fix: ensure both pointers are safe after delete
    assert(p == nullptr);
    assert(q == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## make_unique & make_shared

Prefer `std::make_unique` and `std::make_shared` over raw `new`. They are **exception-safe** (no leak if a constructor throws), avoid redundant type names, and clearly express intent. There is almost never a reason to use raw `new` in modern C++.

### How it works

```cpp
#include <iostream>
#include <memory>

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

int main() {
    // Prefer this:
    auto p1 = std::make_unique<Point>(1.0, 2.0);
    auto p2 = std::make_shared<Point>(3.0, 4.0);

    // Over this (raw new):
    // std::unique_ptr<Point> p3(new Point(5.0, 6.0));  // works but less safe

    std::cout << "p1: (" << p1->x << ", " << p1->y << ")" << std::endl;
    std::cout << "p2: (" << p2->x << ", " << p2->y << ")"
              << " [count=" << p2.use_count() << "]" << std::endl;
    return 0;
}
```

| Feature | `make_unique` / `make_shared` | Raw `new` |
|---------|-------------------------------|-----------|
| Exception safety | Yes — no leak if constructor throws | Possible leak in complex expressions |
| Type repetition | No — type deduced from template arg | Must repeat type: `new Point(...)` |
| `shared_ptr` allocation | Single allocation for object + control block | Two allocations |
| Custom deleter support | No — use constructor directly | Yes |

### Practice

3. :star::star: **Fill:** Replace the raw `new` calls with `make_unique` and `make_shared`.

```cpp,editable
#include <iostream>
#include <memory>
#include <string>
#include <cassert>

int main() {
    // Replace raw new with make_unique
    std::unique_ptr<std::string> s1(_);

    // Replace raw new with make_shared
    std::shared_ptr<int> n1(__);

    assert(*s1 == "hello");
    assert(*n1 == 42);
    assert(n1.use_count() == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Custom Deleters

Sometimes you need a smart pointer to clean up a resource that isn't freed with `delete` — for example, a C file handle, a database connection, or a custom allocator. Both `unique_ptr` and `shared_ptr` accept **custom deleters** that run instead of the default `delete`.

### How it works

```cpp
#include <iostream>
#include <memory>
#include <cstdio>

int main() {
    // unique_ptr with lambda deleter for FILE*
    auto file_deleter = [](FILE* f) {
        if (f) {
            std::cout << "Closing file" << std::endl;
            std::fclose(f);
        }
    };
    std::unique_ptr<FILE, decltype(file_deleter)> file(
        std::fopen("test.txt", "w"), file_deleter);

    // shared_ptr with lambda deleter (simpler syntax)
    std::shared_ptr<int> p(new int(42), [](int* ptr) {
        std::cout << "Custom delete for " << *ptr << std::endl;
        delete ptr;
    });

    return 0;
}
```

| Smart Pointer | Custom Deleter Syntax | Notes |
|---------------|----------------------|-------|
| `unique_ptr<T, Deleter>` | Deleter is part of the type | Use `decltype(lambda)` for lambda deleters |
| `shared_ptr<T>` | Deleter passed to constructor | Type-erased — deleter is not part of the type |
| Lambda deleter | `[](T* p) { /* cleanup */ }` | Most common modern style |

### Practice

4. :star::star::star: **Complete:** Create a `unique_ptr` with a custom lambda deleter that logs a message and then deletes the integer.

```cpp,editable
#include <iostream>
#include <memory>
#include <cassert>
#include <string>

int main() {
    std::string log_message;

    auto deleter = [&log_message](int* p) {
        log_message = "deleted " + std::to_string(*p);
        delete p;
    };

    {
        __ ptr(__);
    }
    // ptr is out of scope — deleter should have run

    assert(log_message == "deleted 99");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Create a `shared_ptr<int>` with a custom deleter that increments a counter each time it deletes. Copy the shared_ptr so the counter only increments once (when the last copy is destroyed).

```cpp,editable
#include <iostream>
#include <memory>
#include <cassert>

int main() {
    int delete_count = 0;

    {
        std::shared_ptr<int> p1(__, __);

        assert(p1.use_count() == 1);

        auto p2 = p1;  // copy — count goes to 2
        assert(p1.use_count() == 2);

        auto p3 = p2;  // copy — count goes to 3
        assert(p1.use_count() == 3);

        // All three go out of scope here — deleter called once
    }

    assert(delete_count == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
