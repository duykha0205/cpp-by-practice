# Vector

`std::vector` is the most commonly used container in C++. It is a dynamic array that manages its own memory, grows automatically, and provides fast random access by index.

## Vector Basics

A vector stores elements contiguously in memory. You can query its size, access elements by index, check the last element, and test whether it is empty.

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    std::cout << "Size:  " << v.size()  << std::endl;  // 5
    std::cout << "First: " << v[0]      << std::endl;  // 1
    std::cout << "Last:  " << v.back()  << std::endl;  // 5
    std::cout << "Empty: " << std::boolalpha << v.empty() << std::endl;  // false
    return 0;
}
```

| Operation | Returns | Complexity |
|-----------|---------|------------|
| `v.size()` | Number of elements | O(1) |
| `v[i]` | Element at index `i` (no bounds check) | O(1) |
| `v.at(i)` | Element at index `i` (throws if out of range) | O(1) |
| `v.back()` | Last element | O(1) |
| `v.empty()` | `true` if size is 0 | O(1) |

### Practice

1. 🌟 **Fill:** Replace each `__` with the correct value.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    assert(v.size() == __);
    assert(v[0] == __);
    assert(v.back() == __);
    assert(v.empty() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Push & Pop

`push_back` appends an element to the end; `pop_back` removes the last element. Both are amortised O(1).

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    std::cout << "Size: " << v.size() << ", Back: " << v.back() << std::endl;  // 3, 30

    v.pop_back();
    std::cout << "Size: " << v.size() << ", Back: " << v.back() << std::endl;  // 2, 20
    return 0;
}
```

| Operation | Effect | Complexity |
|-----------|--------|------------|
| `push_back(x)` | Appends `x` to end; may reallocate | Amortised O(1) |
| `pop_back()` | Removes last element (undefined if empty) | O(1) |
| `emplace_back(args...)` | Constructs element in-place at end | Amortised O(1) |

### Practice

2. 🌟 **Fill:** After three `push_back`s and one `pop_back`, what are the size and last element?

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    assert(v.size() == __);

    v.pop_back();
    assert(v.size() == __);
    assert(v.back() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Iterating

The most readable way to iterate a vector is a **range-based for loop**. You can also use iterators for more control.

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Range-based for (read-only)
    for (int val : v)
        std::cout << val << " ";
    std::cout << std::endl;

    // Iterator-based
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    return 0;
}
```

| Style | Syntax | When to use |
|-------|--------|-------------|
| Range-based for | `for (int val : v)` | Simple iteration, no index needed |
| Range-based ref | `for (int& val : v)` | Need to modify elements in place |
| Iterator | `for (auto it = v.begin(); ...)` | Need position info (erase, insert) |

### Practice

3. 🌟🌟 **Fill:** Complete the range-based for loop to sum all elements.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;

    for (__ : v) {
        sum += val;
    }

    assert(sum == 15);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Insert & Erase

`insert` adds an element at any position; `erase` removes one. Both take iterators, and elements after the position are shifted, making them O(n).

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    v.insert(v.begin() + 2, 10);   // {1, 2, 10, 3, 4, 5}
    v.erase(v.begin() + 2);        // {1, 2, 3, 4, 5}

    for (int x : v) std::cout << x << " ";  // 1 2 3 4 5
    std::cout << std::endl;
    return 0;
}
```

| Operation | Syntax | Complexity |
|-----------|--------|------------|
| Insert at position | `v.insert(v.begin() + i, val)` | O(n) |
| Erase at position | `v.erase(v.begin() + i)` | O(n) |
| Erase range | `v.erase(first, last)` | O(n) |

### Practice

4. 🌟🌟 **Fill:** Insert 10 at index 2, then erase it.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    v.insert(v.begin() + __, __);
    assert(v[2] == 10);
    assert(v.size() == 6);

    v.erase(v.begin() + __);
    assert(v.size() == 5);
    assert(v[2] == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Sorting with Comparators

`std::sort` sorts a range in ascending order by default. Pass a **comparator** (lambda or function) to customise the order, such as sorting in descending order.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    // Ascending (default)
    std::sort(v.begin(), v.end());

    // Descending with a lambda comparator
    std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; });

    for (int x : v) std::cout << x << " ";  // 9 6 5 4 3 2 1 1
    std::cout << std::endl;
    return 0;
}
```

| Comparator | Meaning |
|------------|---------|
| `[](int a, int b) { return a < b; }` | Ascending (default) |
| `[](int a, int b) { return a > b; }` | Descending |
| `std::greater<int>{}` | Built-in descending comparator |

### Practice

5. 🌟🌟 **Fill:** Sort the vector in descending order by providing a lambda comparator.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

    std::sort(v.begin(), v.end(), __);

    assert(v.front() == 9);
    assert(v.back() == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
