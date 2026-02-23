# Lambda Expressions

Lambdas are **anonymous functions** you can define inline. They are the backbone of modern C++ — used with STL algorithms, callbacks, and anywhere you need a short, throwaway function.

## Lambda Basics

A lambda has the syntax `[capture](params) { body }`. It can be stored in a variable with `auto` and called like a regular function.

### How it works

```cpp
#include <iostream>

int main() {
    auto greet = [](const std::string& name) {
        return "Hello, " + name + "!";
    };

    std::cout << greet("World") << std::endl;   // Hello, World!

    auto add = [](int a, int b) { return a + b; };
    std::cout << add(3, 4) << std::endl;        // 7
    return 0;
}
```

| Part | Syntax | Purpose |
|------|--------|---------|
| Capture | `[]` | Variables from enclosing scope to use inside the lambda |
| Parameters | `(int a, int b)` | Function arguments |
| Body | `{ return a + b; }` | The function logic |
| Return type | Deduced automatically (or `-> Type` to be explicit) |

### Practice

1. 🌟 **Fill:** Create a lambda that takes two ints and returns their sum.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    auto add = __;

    assert(add(3, 4) == 7);
    assert(add(10, 20) == 30);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Lambdas with STL Algorithms

Lambdas shine when passed to STL algorithms like `std::sort`, `std::find_if`, and `std::transform`. They let you define the comparison or transformation right at the call site.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    std::vector<std::string> words = {"banana", "fig", "apple", "kiwi"};

    // Sort by string length
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.size() < b.size();
              });

    for (const auto& w : words)
        std::cout << w << " ";  // fig kiwi apple banana
    std::cout << std::endl;
    return 0;
}
```

| Algorithm | Lambda role |
|-----------|------------|
| `std::sort` | Comparator: returns `true` if first arg should come before second |
| `std::find_if` | Predicate: returns `true` for the element to find |
| `std::transform` | Unary op: maps each element to a new value |

### Practice

2. 🌟🌟 **Fill:** Write a lambda comparator that sorts strings by length (shortest first).

```cpp,editable
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<std::string> words = {"banana", "fig", "apple", "kiwi"};

    std::sort(words.begin(), words.end(), __);

    assert(words[0] == "fig");
    assert(words[3] == "banana");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Captures

Lambdas can **capture** variables from the enclosing scope. Capture by value (`[x]`) makes a copy; capture by reference (`[&x]`) lets the lambda modify the original.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 5, 3, 8, 2, 7, 4};
    int threshold = 4;

    // Capture threshold by value
    auto count = std::count_if(v.begin(), v.end(),
                                [threshold](int x) { return x > threshold; });
    std::cout << "Above " << threshold << ": " << count << std::endl;  // 3

    // Capture sum by reference to accumulate
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) { sum += x; });
    std::cout << "Sum: " << sum << std::endl;  // 30
    return 0;
}
```

| Capture | Syntax | Effect |
|---------|--------|--------|
| By value | `[x]` | Copy of `x` at lambda creation time |
| By reference | `[&x]` | Reference to `x`; lambda can modify it |
| All by value | `[=]` | Copy all referenced locals |
| All by reference | `[&]` | Reference all referenced locals |
| Mix | `[=, &x]` | Copy all except `x` by reference |

### Practice

3. 🌟🌟 **Fill:** Capture the `threshold` variable so the lambda can use it.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {1, 5, 3, 8, 2, 7, 4};
    int threshold = 4;

    auto count = std::count_if(v.begin(), v.end(),
                                [__](int x) { return x > threshold; });

    assert(count == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟 **Fix:** The lambda captures `sum` by value, so the original `sum` never changes. Capture by reference instead.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};
    int sum = 0;

    // BUG: captures sum by value — original sum stays 0
    std::for_each(v.begin(), v.end(), [sum](int x) { sum += x; });

    assert(sum == 15);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Mutable Lambdas

By default, a lambda's captured-by-value variables are **const** inside the body. Adding `mutable` lets you modify the copy (but the original is still unchanged).

### How it works

```cpp
#include <iostream>

int main() {
    int counter = 0;

    auto increment = [counter]() mutable {
        return ++counter;  // modifies the lambda's own copy
    };

    std::cout << increment() << std::endl;  // 1
    std::cout << increment() << std::endl;  // 2
    std::cout << "Original: " << counter << std::endl;  // 0 (unchanged)
    return 0;
}
```

| Keyword | Effect |
|---------|--------|
| Without `mutable` | Captured-by-value vars are `const` — can't modify |
| With `mutable` | Can modify the lambda's internal copy |
| Original variable | Never affected (it was captured by value) |

### Practice

5. 🌟🌟🌟 **Fix:** The lambda tries to modify a captured-by-value variable but it's const by default. Add `mutable`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int counter = 0;

    // BUG: counter is captured by value and is const — can't modify it
    auto increment = [counter]() {
        return ++counter;
    };

    assert(increment() == 1);
    assert(increment() == 2);
    assert(counter == 0);  // original unchanged

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Generic Lambdas

Since C++14, lambda parameters can use `auto` to accept **any type**, making the lambda a template under the hood.

### How it works

```cpp
#include <iostream>
#include <string>

int main() {
    auto print = [](const auto& value) {
        std::cout << value << std::endl;
    };

    print(42);              // int
    print(3.14);            // double
    print(std::string("hello"));  // string
    return 0;
}
```

| Feature | Detail |
|---------|--------|
| `auto` parameter | Lambda becomes a template; compiler generates code for each type |
| `const auto&` | Accepts any type by const reference (no copy) |
| Multiple `auto` params | Each independently deduced |

### Practice

6. 🌟🌟 **Fill:** Use `auto` in the lambda parameter to make it work with any type.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    auto identity = [](__ x) { return x; };

    assert(identity(42) == 42);
    assert(identity(3.14) == 3.14);
    assert(identity(std::string("hi")) == "hi");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
