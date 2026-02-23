# Arrays

Arrays store a fixed-size sequence of elements of the same type in contiguous memory. C++ offers the traditional C-style array and the safer `std::array` wrapper. Understanding how arrays work — including their pitfalls like out-of-bounds access and pointer decay — is essential for working with collections of data.

## C-style Arrays

### How it works

A C-style array is declared with a fixed size known at compile time. Elements are accessed by index starting at 0. You can compute the number of elements with the `sizeof` trick.

```cpp
#include <iostream>

int main() {
    int arr[5] = {10, 20, 30, 40, 50};

    std::cout << "First: "  << arr[0] << std::endl;  // 10
    std::cout << "Last: "   << arr[4] << std::endl;  // 50

    int length = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Length: " << length << std::endl;   // 5
    return 0;
}
```

| Concept | Detail |
|---|---|
| Declaration | `int arr[5];` — 5 elements, uninitialized |
| Initialization | `int arr[5] = {1, 2, 3, 4, 5};` |
| Indexing | Zero-based: `arr[0]` to `arr[4]` |
| Size trick | `sizeof(arr) / sizeof(arr[0])` gives element count |

### Practice

1\. 🌟 Fill the blanks with the correct values.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    assert(arr[0] == __);
    assert(arr[4] == __);
    assert(sizeof(arr) / sizeof(arr[0]) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Array Bounds

### How it works

Accessing an index outside `0 .. size-1` is **undefined behavior** — the program may crash, produce garbage, or appear to work. C-style arrays perform no bounds checking.

```cpp
#include <iostream>

int main() {
    int arr[3] = {10, 20, 30};

    // Safe: indices 0, 1, 2
    for (int i = 0; i < 3; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;  // 10 20 30

    // arr[3] would be OUT OF BOUNDS — undefined behavior!
    return 0;
}
```

| Pitfall | Explanation |
|---|---|
| `i <= size` in loop | Off-by-one: accesses one element past the end |
| No runtime check | C-style arrays don't throw on bad index |
| Use `std::array::at()` | Throws `std::out_of_range` on invalid index |

### Practice

2\. 🌟🌟 Fix: the loop condition `i <= 3` goes out of bounds on an array of size 3.

```cpp,editable
#include <iostream>

int main() {
    int arr[3] = {10, 20, 30};

    for (int i = 0; i <= 3; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}
```

## std::array

### How it works

`std::array` (from `<array>`) is a fixed-size container that knows its own size and provides bounds-checked access via `.at()`.

```cpp
#include <iostream>
#include <array>

int main() {
    std::array<int, 4> arr = {10, 20, 30, 40};

    std::cout << "Size: "  << arr.size()  << std::endl;  // 4
    std::cout << "at(2): " << arr.at(2)   << std::endl;  // 30
    std::cout << "Front: " << arr.front() << std::endl;   // 10
    std::cout << "Back: "  << arr.back()  << std::endl;   // 40
    return 0;
}
```

| Method | Returns |
|---|---|
| `.size()` | Number of elements |
| `.at(i)` | Element at index `i` with bounds checking |
| `.front()` | First element |
| `.back()` | Last element |
| `[]` operator | Element at index — no bounds check |

### Practice

3\. 🌟🌟 Fill the blanks with the correct values returned by each method.

```cpp,editable
#include <iostream>
#include <array>
#include <cassert>

int main() {
    std::array<int, 4> arr = {10, 20, 30, 40};

    assert(arr.size() == __);
    assert(arr.at(2) == __);
    assert(arr.front() == __);
    assert(arr.back() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Multidimensional Arrays

### How it works

A 2D array is an array of arrays. You access elements with two indices: `matrix[row][col]`. A common example is the identity matrix where diagonal elements are 1 and everything else is 0.

```cpp
#include <iostream>

int main() {
    int matrix[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
```

| Concept | Detail |
|---|---|
| Declaration | `int m[rows][cols];` |
| Access | `m[i][j]` — row `i`, column `j` |
| Identity matrix | `m[i][j] == 1` when `i == j`, else `0` |

### Practice

4\. 🌟🌟 Fill the blanks to create a 3×3 identity matrix.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int matrix[3][3] = {
        {__, __, __},
        {__, __, __},
        {__, __, __}
    };

    // Verify it's an identity matrix
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) assert(matrix[i][j] == 1);
            else assert(matrix[i][j] == 0);
        }
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Passing Arrays to Functions

### How it works

When you pass a C-style array to a function it **decays** to a pointer — the function receives the memory address, not a copy. This means the function cannot determine the array's size from the pointer alone; you must pass the size separately.

```cpp
#include <iostream>

int sum(int arr[], int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    std::cout << "Sum: " << sum(numbers, size) << std::endl;  // 15
    return 0;
}
```

| Concept | Detail |
|---|---|
| Decay | `int arr[]` in a parameter is really `int* arr` |
| Size lost | `sizeof(arr)` inside the function gives pointer size, not array size |
| Pass size separately | Always pass the element count as a second argument |
| Alternative | Use `std::array` or `std::span` (C++20) to preserve size info |

### Practice

5\. 🌟🌟 Fill the blank: what is the sum of the array elements?

```cpp,editable
#include <iostream>
#include <cassert>

int sum(int arr[], int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        total += arr[i];
    }
    return total;
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    int size = sizeof(numbers) / sizeof(numbers[0]);

    assert(sum(numbers, size) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
