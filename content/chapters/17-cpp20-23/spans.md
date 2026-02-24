# Spans

`std::span` (C++20) is a **non-owning view** over a contiguous sequence of elements. It does not allocate or copy data — it simply refers to memory owned by something else (an array, a `std::vector`, a C-style array). Think of it as a safer, more expressive replacement for the classic "pointer + size" pair.

> **Note:** Requires GCC 10+ or Clang 10+ with `-std=c++20`. MSVC 19.26+ supports it with `/std:c++20`. Include `<span>`.

## std::span Basics

A `std::span<T>` holds a pointer and a size. It can be constructed from a C-array, `std::array`, `std::vector`, or any contiguous range. It never owns the data it refers to.

### How it works

```cpp
#include <iostream>
#include <span>
#include <vector>
#include <array>

void print_span(std::span<const int> s) {
    for (int val : s) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    // From a C-array
    int c_arr[] = {1, 2, 3, 4, 5};
    print_span(c_arr);                      // 1 2 3 4 5

    // From a std::vector
    std::vector<int> vec = {10, 20, 30};
    print_span(vec);                        // 10 20 30

    // From a std::array
    std::array<int, 4> arr = {100, 200, 300, 400};
    print_span(arr);                        // 100 200 300 400

    // Explicit construction
    std::span<const int> sp(vec.data(), vec.size());
    print_span(sp);                         // 10 20 30

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::span<T>` | Non-owning, mutable view over contiguous `T` elements |
| `std::span<const T>` | Non-owning, read-only view |
| Dynamic extent | `std::span<T>` — size known at runtime (default) |
| Static extent | `std::span<T, N>` — size fixed at compile time |
| No ownership | Span does not allocate, copy, or free memory |
| Implicit conversion | Vectors, arrays, and C-arrays implicitly convert to `span` |

### Practice

1. 🌟🌟 **Fill:** Complete the function that accepts a `std::span` and returns the sum of all elements.

```cpp,editable
#include <iostream>
#include <span>
#include <vector>
#include <numeric>
#include <cassert>

int sum_elements(__) {  // parameter: a span of const int
    int total = 0;
    for (int val : s) {
        total += val;
    }
    return total;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    assert(sum_elements(vec) == 15);

    int arr[] = {10, 20, 30};
    assert(sum_elements(arr) == 60);

    std::array<int, 4> std_arr = {5, 5, 5, 5};
    assert(sum_elements(std_arr) == 20);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## span Operations

`std::span` provides a rich set of member functions for accessing elements and creating sub-views without any copying.

### How it works

```cpp
#include <iostream>
#include <span>
#include <vector>

int main() {
    std::vector<int> data = {10, 20, 30, 40, 50, 60, 70};
    std::span<int> s(data);

    // Basic accessors
    std::cout << "Size:  " << s.size()  << std::endl;  // 7
    std::cout << "Front: " << s.front() << std::endl;  // 10
    std::cout << "Back:  " << s.back()  << std::endl;  // 70
    std::cout << "s[2]:  " << s[2]      << std::endl;  // 30

    // Sub-spans
    auto first3 = s.first(3);     // {10, 20, 30}
    auto last2  = s.last(2);      // {60, 70}
    auto mid    = s.subspan(2, 3); // {30, 40, 50}

    std::cout << "first(3): ";
    for (int v : first3) std::cout << v << " ";  // 10 20 30
    std::cout << std::endl;

    std::cout << "last(2):  ";
    for (int v : last2) std::cout << v << " ";   // 60 70
    std::cout << std::endl;

    std::cout << "subspan(2,3): ";
    for (int v : mid) std::cout << v << " ";     // 30 40 50
    std::cout << std::endl;

    return 0;
}
```

| Operation | Returns | Detail |
|-----------|---------|--------|
| `s.size()` | `size_t` | Number of elements |
| `s.empty()` | `bool` | True if size is 0 |
| `s.front()` | `T&` | First element |
| `s.back()` | `T&` | Last element |
| `s[i]` | `T&` | Element at index `i` (no bounds check) |
| `s.data()` | `T*` | Pointer to underlying data |
| `s.first(n)` | `span<T>` | Sub-span of the first `n` elements |
| `s.last(n)` | `span<T>` | Sub-span of the last `n` elements |
| `s.subspan(offset, count)` | `span<T>` | Sub-span starting at `offset` with `count` elements |

### Practice

2. 🌟🌟 **Fill:** Use span operations to extract sub-views and verify their contents.

```cpp,editable
#include <iostream>
#include <span>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> data = {10, 20, 30, 40, 50};
    std::span<int> s(data);

    assert(s.size() == __);

    auto first_two = s.__(_);   // get the first 2 elements
    assert(first_two.size() == 2);
    assert(first_two[0] == 10);
    assert(first_two[1] == 20);

    auto last_three = s.__(_);  // get the last 3 elements
    assert(last_three.size() == 3);
    assert(last_three[0] == 30);

    auto middle = s.__(__, __); // subspan starting at index 1, length 3
    assert(middle.size() == 3);
    assert(middle[0] == 20);
    assert(middle[2] == 40);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## span with const

Using `std::span<const T>` creates a **read-only** view. This is the preferred way to pass array-like data to functions — it works with any contiguous container and clearly communicates that the function will not modify the data. It replaces the error-prone C pattern of `const T* ptr, size_t size`.

### How it works

```cpp
#include <iostream>
#include <span>
#include <vector>
#include <array>
#include <numeric>

// Read-only: accepts any contiguous container
double average(std::span<const double> values) {
    if (values.empty()) return 0.0;
    double sum = 0.0;
    for (double v : values) sum += v;
    return sum / values.size();
}

// Mutable: can modify the elements through the span
void double_values(std::span<int> values) {
    for (int& v : values) {
        v *= 2;
    }
}

int main() {
    std::vector<double> grades = {85.5, 92.0, 78.5, 95.0};
    std::cout << "Average: " << average(grades) << std::endl;  // 87.75

    double c_grades[] = {70.0, 80.0, 90.0};
    std::cout << "Average: " << average(c_grades) << std::endl;  // 80

    std::vector<int> nums = {1, 2, 3};
    double_values(nums);
    // nums is now {2, 4, 6}
    for (int n : nums) std::cout << n << " ";
    std::cout << std::endl;

    return 0;
}
```

| Pattern | Signature | Use when |
|---------|-----------|----------|
| Read-only | `func(std::span<const T>)` | Function only reads the data |
| Mutable | `func(std::span<T>)` | Function needs to modify elements |
| Old C-style | `func(const T* ptr, size_t n)` | Legacy code; prefer `span` instead |
| Fixed-size | `func(std::span<T, N>)` | Size known at compile time |

### Practice

3. 🌟🌟 **Fill:** Write a function using `std::span<const int>` that finds and returns the maximum element.

```cpp,editable
#include <iostream>
#include <span>
#include <vector>
#include <cassert>

int find_max(__) {  // parameter: a read-only span of int
    int max_val = s[0];
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] > max_val) {
            max_val = s[i];
        }
    }
    return max_val;
}

int main() {
    std::vector<int> v1 = {3, 7, 2, 9, 4};
    assert(find_max(v1) == 9);

    int arr[] = {100, 50, 75};
    assert(find_max(arr) == 100);

    std::array<int, 3> sa = {-5, -1, -10};
    assert(find_max(sa) == -1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟🌟 **Complete:** Write two functions: `zero_out` takes a mutable span and sets all elements to zero, and `dot_product` takes two read-only spans and computes their dot product.

```cpp,editable
#include <iostream>
#include <span>
#include <vector>
#include <cassert>

void zero_out(__) {  // parameter: a mutable span of int
    __;  // set every element to 0
}

int dot_product(__, __) {  // two read-only spans of int
    __;  // compute and return the dot product (sum of a[i]*b[i])
}

int main() {
    // Test zero_out
    std::vector<int> data = {5, 10, 15, 20};
    zero_out(data);
    for (int v : data) {
        assert(v == 0);
    }

    // Test dot_product
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {4, 5, 6};
    // dot product = 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    assert(dot_product(a, b) == 32);

    int arr1[] = {2, 3};
    int arr2[] = {4, 5};
    // dot product = 2*4 + 3*5 = 8 + 15 = 23
    assert(dot_product(arr1, arr2) == 23);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
