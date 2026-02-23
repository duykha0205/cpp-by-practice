# Dynamic Memory: new & delete

C++ gives you direct control over memory allocation on the **heap** using `new` and `delete`. This power comes with responsibility — every allocation must be paired with a deallocation, or you get memory leaks and undefined behaviour.

## Dynamic Allocation

`new` allocates memory on the heap and returns a pointer. `delete` frees it. Forgetting `delete` causes a **memory leak** — the memory stays allocated until the program ends.

### How it works

```cpp
#include <iostream>

int main() {
    int* p = new int(42);
    std::cout << *p << std::endl;  // 42
    delete p;                       // free the memory
    p = nullptr;                    // good practice: avoid dangling pointer
    return 0;
}
```

| Operation | Meaning |
|-----------|---------|
| `new T(args)` | Allocate one `T` on the heap, call constructor |
| `delete p` | Destroy object and free memory pointed to by `p` |
| Memory leak | Allocated memory that is never freed |
| `p = nullptr` | Prevents accidental use of the freed pointer |

### Practice

1. 🌟 **Fix:** This program leaks memory. Add the missing `delete` to free the allocation.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* p = new int(42);
    assert(*p == 42);

    // BUG: memory leak! The allocation is never freed.

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Array Allocation

`new[]` allocates an array on the heap; it **must** be freed with `delete[]` (not plain `delete`). Mismatching them is undefined behaviour.

### How it works

```cpp
#include <iostream>

int main() {
    int* arr = new int[5]{10, 20, 30, 40, 50};
    for (int i = 0; i < 5; ++i)
        std::cout << arr[i] << " ";  // 10 20 30 40 50
    std::cout << std::endl;

    delete[] arr;  // must use delete[] for arrays
    return 0;
}
```

| Operation | Use for |
|-----------|---------|
| `new T[n]` | Allocating an array of `n` elements |
| `delete[] p` | Freeing an array allocated with `new[]` |
| `delete p` (wrong!) | UB when `p` was allocated with `new[]` |

### Practice

2. 🌟🌟 **Fix:** `delete arr` should be `delete[] arr`. Also fill in the value of `arr[2]`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* arr = new int[5]{10, 20, 30, 40, 50};

    assert(arr[2] == __);

    delete arr;  // BUG: wrong delete for array

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Dangling Pointers

A **dangling pointer** points to memory that has already been freed. Dereferencing it is **undefined behaviour** — it might crash, return garbage, or appear to work. Always set pointers to `nullptr` after deletion.

### How it works

```cpp
#include <iostream>

int main() {
    int* p = new int(42);
    std::cout << *p << std::endl;  // 42
    delete p;

    // p is now dangling — DO NOT use it!
    // std::cout << *p;  // UNDEFINED BEHAVIOUR

    p = nullptr;  // safe: dereferencing nullptr is defined (crash, not UB)
    return 0;
}
```

| Term | Meaning |
|------|---------|
| Dangling pointer | Points to freed/invalid memory |
| Use-after-free | Reading/writing through a dangling pointer — UB |
| Fix | Set `p = nullptr` after `delete`; don't access freed memory |

### Practice

3. 🌟🌟 **Fix:** The code reads from `p` after deleting it (use-after-free). Remove the dangerous access and set the pointer to `nullptr`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* p = new int(42);
    assert(*p == 42);
    delete p;

    // BUG: use-after-free!
    std::cout << "Value: " << *p << std::endl;

    assert(p == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Double Free

Calling `delete` on the same pointer twice is **undefined behaviour** (typically a crash). Setting a pointer to `nullptr` after deletion prevents this, because `delete nullptr` is safe (a no-op).

### How it works

```cpp
#include <iostream>

int main() {
    int* p = new int(42);
    delete p;
    p = nullptr;  // makes the second delete safe

    delete p;     // OK: deleting nullptr is a no-op
    std::cout << "No crash!" << std::endl;
    return 0;
}
```

| Scenario | Outcome |
|----------|---------|
| `delete p; delete p;` | Undefined behaviour (crash or corruption) |
| `delete p; p = nullptr; delete p;` | Safe — second delete is a no-op |
| Best practice | Always set `p = nullptr` after `delete` |

### Practice

4. 🌟🌟🌟 **Fix:** This code deletes `p` twice, causing undefined behaviour. Fix it by setting `p` to `nullptr` after the first delete (or removing the second delete).

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int* p = new int(42);
    assert(*p == 42);

    delete p;
    // BUG: double free!
    delete p;

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
