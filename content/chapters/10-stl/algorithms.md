# STL Algorithms

The `<algorithm>` and `<numeric>` headers provide dozens of ready-made operations on ranges. Using them instead of hand-written loops makes code shorter, less error-prone, and often faster.

## find & count

`std::find` returns an iterator to the first matching element (or `end()` if not found). `std::count` returns how many times a value appears.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    auto it = std::find(v.begin(), v.end(), 3);
    if (it != v.end())
        std::cout << "Found 3 at index " << (it - v.begin()) << std::endl;  // 2

    int n = std::count(v.begin(), v.end(), 2);
    std::cout << "Count of 2: " << n << std::endl;  // 3
    return 0;
}
```

| Algorithm | Returns | Complexity |
|-----------|---------|------------|
| `std::find(first, last, value)` | Iterator to first match, or `last` | O(n) |
| `std::count(first, last, value)` | Number of matches | O(n) |
| `std::find_if(first, last, pred)` | Iterator to first element satisfying predicate | O(n) |

### Practice

1. 🌟 **Fill:** What does `*it` equal, and how many 2s are in the vector?

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 2, 4, 2, 5};

    auto it = std::find(v.begin(), v.end(), 3);
    assert(*it == __);

    int n = std::count(v.begin(), v.end(), 2);
    assert(n == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## transform

`std::transform` applies a function to each element of one range and writes the results to another (or the same) range. Think of it as a "map" operation.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::vector<int> result(v.size());

    std::transform(v.begin(), v.end(), result.begin(),
                   [](int x) { return x * 2; });

    for (int x : result) std::cout << x << " ";  // 2 4 6 8 10
    std::cout << std::endl;
    return 0;
}
```

| Parameter | Meaning |
|-----------|---------|
| `first, last` | Input range |
| `d_first` | Output iterator (can be same as input for in-place) |
| `unary_op` | Function/lambda applied to each element |

### Practice

2. 🌟🌟 **Fill:** Write a lambda that doubles each element.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::vector<int> result(v.size());

    std::transform(v.begin(), v.end(), result.begin(), __);

    assert(result[0] == 2);
    assert(result[4] == 10);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## accumulate

`std::accumulate` (from `<numeric>`) reduces a range to a single value by repeatedly applying a binary operation. The default operation is addition.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << std::endl;  // 15

    int product = std::accumulate(v.begin(), v.end(), 1,
                                  [](int acc, int x) { return acc * x; });
    std::cout << "Product: " << product << std::endl;  // 120
    return 0;
}
```

| Parameter | Meaning |
|-----------|---------|
| `first, last` | Input range |
| `init` | Initial accumulator value |
| `binary_op` (optional) | Custom operation (default is `+`) |

### Practice

3. 🌟🌟 **Fill:** Use `accumulate` with a lambda to compute the product of all elements.

```cpp,editable
#include <iostream>
#include <vector>
#include <numeric>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    int product = std::accumulate(v.begin(), v.end(), __, __);

    assert(product == 120);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## any_of / all_of / none_of

These algorithms test whether a predicate holds for **any**, **all**, or **none** of the elements in a range. They short-circuit as soon as the answer is determined.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {2, 4, 6, 8};

    bool all_even = std::all_of(v.begin(), v.end(),
                                [](int x) { return x % 2 == 0; });
    std::cout << "All even: " << std::boolalpha << all_even << std::endl;  // true

    bool any_neg = std::any_of(v.begin(), v.end(),
                               [](int x) { return x < 0; });
    std::cout << "Any negative: " << any_neg << std::endl;  // false

    bool none_odd = std::none_of(v.begin(), v.end(),
                                  [](int x) { return x % 2 != 0; });
    std::cout << "None odd: " << none_odd << std::endl;  // true
    return 0;
}
```

| Algorithm | Returns `true` when |
|-----------|-------------------|
| `all_of` | Every element satisfies the predicate |
| `any_of` | At least one element satisfies the predicate |
| `none_of` | No element satisfies the predicate |

### Practice

4. 🌟🌟 **Fill:** Write a lambda to check if all elements are even.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {2, 4, 6, 8, 10};

    bool result = std::all_of(v.begin(), v.end(), __);

    assert(result == true);

    v.push_back(3);
    result = std::all_of(v.begin(), v.end(),
                         [](int x) { return x % 2 == 0; });
    assert(result == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Erase-Remove Idiom

`std::remove` / `std::remove_if` don't actually erase elements — they shift wanted elements to the front and return an iterator to the new logical end. You then call `erase` on the remaining tail. This two-step pattern is called the **erase-remove idiom**.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    // Remove all odd numbers
    auto new_end = std::remove_if(v.begin(), v.end(),
                                   [](int x) { return x % 2 != 0; });
    v.erase(new_end, v.end());

    for (int x : v) std::cout << x << " ";  // 2 4 6 8
    std::cout << std::endl;
    return 0;
}
```

| Step | What happens |
|------|-------------|
| `remove_if(begin, end, pred)` | Shifts non-matching elements forward, returns new logical end |
| `v.erase(new_end, v.end())` | Actually removes the leftover elements from the container |
| C++20 alternative | `std::erase_if(v, pred)` does both steps in one call |

### Practice

5. 🌟🌟🌟 **Fill:** Use `remove_if` with a lambda to remove all odd numbers, then erase the tail.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

    auto new_end = std::remove_if(v.begin(), v.end(), __);
    v.erase(__, __);

    assert(v.size() == 4);
    assert(v[0] == 2);
    assert(v[3] == 8);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
