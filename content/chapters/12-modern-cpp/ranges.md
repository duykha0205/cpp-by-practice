# Ranges & Views (C++20)

C++20 Ranges revolutionize how we work with sequences. Instead of passing iterator pairs, you pass the container directly. **Views** are lazy, composable adapters that transform data without copying. The **pipe operator** (`|`) lets you chain views into readable pipelines.

## Range-Based Algorithms

`std::ranges::` versions of classic algorithms accept a container directly — no more `.begin()` / `.end()`. They also support **projections**: a callable applied to each element before the algorithm sees it, eliminating the need for many custom comparators.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>

struct Student {
    std::string name;
    int grade;
};

int main() {
    std::vector<int> v = {5, 3, 1, 4, 2};

    // Sort the whole container — no iterators needed
    std::ranges::sort(v);
    // v is now {1, 2, 3, 4, 5}

    auto it = std::ranges::find(v, 3);
    std::cout << "Found: " << *it << std::endl;

    auto n = std::ranges::count(v, 4);
    std::cout << "Count of 4: " << n << std::endl;

    // Projection: sort students by grade
    std::vector<Student> students = {{"Alice", 90}, {"Bob", 75}, {"Carol", 88}};
    std::ranges::sort(students, {}, &Student::grade);
    // students sorted by grade ascending: Bob(75), Carol(88), Alice(90)

    return 0;
}
```

| Algorithm | Usage | Projection |
|-----------|-------|------------|
| `std::ranges::sort(r)` | Sorts range `r` in place | `std::ranges::sort(r, {}, proj)` |
| `std::ranges::find(r, val)` | Returns iterator to first match | `std::ranges::find(r, val, proj)` |
| `std::ranges::count(r, val)` | Counts occurrences of `val` | `std::ranges::count(r, val, proj)` |
| `std::ranges::min_element(r)` | Iterator to the smallest element | Supports projection |

### Practice

1. :star::star: **Fill:** Use `std::ranges` algorithms to sort, find, and count elements.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <cassert>

int main() {
    std::vector<int> v = {4, 2, 7, 1, 9, 3, 7};

    // Sort the vector using ranges
    __;

    assert(v[0] == 1);
    assert(v[6] == 9);

    // Find the value 7
    auto it = __;
    assert(it != v.end());
    assert(*it == 7);

    // Count how many 7s there are
    auto n = __;
    assert(n == 2);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. :star::star::star: **Complete:** Use a projection to sort a vector of pairs by their second element, then find a pair by projecting the first element.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>
#include <string>
#include <cassert>

int main() {
    std::vector<std::pair<std::string, int>> data = {
        {"cherry", 3}, {"apple", 1}, {"banana", 2}
    };

    // Sort by the int (second element) using a projection
    std::ranges::sort(data, {}, __);

    assert(data[0].first == "apple");
    assert(data[1].first == "banana");
    assert(data[2].first == "cherry");

    // Find the pair whose first element is "banana" using a projection
    auto it = std::ranges::find(data, __, __);
    assert(it != data.end());
    assert(it->second == 2);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Views

Views are **lazy adapters** — they don't copy or modify the underlying data. They describe a transformation that is only applied when you iterate. Common views include `std::views::filter`, `std::views::transform`, and `std::views::take`.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <ranges>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter: keep only even numbers
    auto evens = std::views::filter(v, [](int x) { return x % 2 == 0; });

    // transform: square each element
    auto squares = std::views::transform(v, [](int x) { return x * x; });

    // take: first 3 elements
    auto first3 = std::views::take(v, 3);

    for (int x : evens) std::cout << x << " ";     // 2 4 6 8 10
    std::cout << std::endl;

    for (int x : squares) std::cout << x << " ";   // 1 4 9 16 25 ... 100
    std::cout << std::endl;

    for (int x : first3) std::cout << x << " ";    // 1 2 3
    std::cout << std::endl;

    return 0;
}
```

| View | Purpose | Lazy? |
|------|---------|-------|
| `std::views::filter(r, pred)` | Keep elements where `pred` is true | Yes |
| `std::views::transform(r, fn)` | Apply `fn` to each element | Yes |
| `std::views::take(r, n)` | First `n` elements | Yes |
| `std::views::drop(r, n)` | Skip first `n` elements | Yes |
| `std::views::reverse` | Reverse the range | Yes |

### Practice

3. :star::star: **Fill:** Use views to filter and transform a vector of integers.

```cpp,editable
#include <iostream>
#include <vector>
#include <ranges>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Create a view of only odd numbers
    auto odds = std::views::filter(v, __);

    // Create a view that doubles each element
    auto doubled = std::views::transform(v, __);

    int odd_sum = 0;
    for (int x : odds) odd_sum += x;
    assert(odd_sum == 25);  // 1+3+5+7+9

    int first_doubled = *doubled.begin();
    assert(first_doubled == 2);  // 1*2

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Pipe Operator

The pipe operator (`|`) lets you **compose views** into readable left-to-right pipelines. Instead of nesting function calls, you chain transformations as `range | view1 | view2 | ...`.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <ranges>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Pipeline: keep evens, square them, take first 3
    auto result = v
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::transform([](int x) { return x * x; })
        | std::views::take(3);

    for (int x : result)
        std::cout << x << " ";  // 4 16 36
    std::cout << std::endl;

    return 0;
}
```

| Pattern | Meaning |
|---------|---------|
| `r \| views::filter(pred)` | Keep elements matching `pred` |
| `r \| views::transform(fn)` | Map `fn` over elements |
| `r \| views::take(n)` | Limit to first `n` results |
| `r \| v1 \| v2 \| v3` | Compose: apply `v1`, then `v2`, then `v3` |

### Practice

4. :star::star::star: **Fill:** Build a pipeline that filters numbers divisible by 3, multiplies them by 10, and takes the first 2 results.

```cpp,editable
#include <iostream>
#include <vector>
#include <ranges>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto pipeline = v
        | std::views::filter(__)
        | std::views::transform(__)
        | __;

    std::vector<int> result;
    for (int x : pipeline) result.push_back(x);

    assert(result.size() == 2);
    assert(result[0] == 30);   // 3 * 10
    assert(result[1] == 60);   // 6 * 10

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Build a pipeline that processes a vector of strings — filter strings longer than 3 characters, transform them to uppercase first character + rest, and collect the results.

```cpp,editable
#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <cctype>

int main() {
    std::vector<std::string> words = {"hi", "world", "ok", "ranges", "cpp", "views"};

    auto capitalize = [](std::string s) -> std::string {
        if (!s.empty()) s[0] = std::toupper(s[0]);
        return s;
    };

    auto pipeline = words
        | std::views::filter(__)
        | std::views::transform(__);

    std::vector<std::string> result;
    for (auto s : pipeline) result.push_back(s);

    assert(result.size() == 3);
    assert(result[0] == "World");
    assert(result[1] == "Ranges");
    assert(result[2] == "Views");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
