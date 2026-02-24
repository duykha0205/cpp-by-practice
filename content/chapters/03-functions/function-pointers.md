# Function Pointers & Callbacks

A **function pointer** stores the address of a function and lets you call it indirectly. This is the foundation for callbacks — passing one function into another so the caller can customize behavior. Modern C++ adds `std::function` as a flexible, type-erased wrapper that works with function pointers, lambdas, and functors alike.

## Function Pointer Syntax

### How it works

A function pointer's type mirrors the function signature: `return_type (*name)(param_types)`. You assign it by using the function's name (which decays to a pointer) and call through it with normal call syntax.

```cpp
#include <iostream>
#include <cassert>

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

int main() {
    // Declare a function pointer
    int (*op)(int, int) = add;

    assert(op(3, 4) == 7);   // calls add

    // Reassign to a different function
    op = multiply;
    assert(op(3, 4) == 12);  // calls multiply

    // Explicit address-of is optional but clearer
    op = &add;
    assert(op(10, 5) == 15);

    std::cout << "op(3, 4) = " << op(3, 4) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Syntax | Meaning |
|--------|---------|
| `int (*fp)(int, int)` | Pointer to a function taking two `int`s, returning `int` |
| `fp = add;` | Assign function (name decays to pointer) |
| `fp = &add;` | Explicit address-of — same effect |
| `fp(3, 4)` | Call through pointer |
| `(*fp)(3, 4)` | Explicit dereference call — same effect |

### Practice

1\. 🌟 **Fill:** Declare a function pointer and assign it to `subtract`, then call it.

```cpp,editable
#include <iostream>
#include <cassert>

int subtract(int a, int b) {
    return a - b;
}

int main() {
    // Declare a function pointer 'op' and assign it to subtract
    __ = subtract;

    assert(op(10, 3) == 7);
    assert(op(100, 50) == 50);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Using Type Aliases for Function Pointers

### How it works

Function pointer syntax is notoriously hard to read. Type aliases (`using` or `typedef`) give the pointer type a clean name, making declarations and parameter lists much clearer.

```cpp
#include <iostream>
#include <cassert>

int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }

// Type alias for a function pointer
using BinaryOp = int (*)(int, int);

// Same thing with typedef (older style):
// typedef int (*BinaryOp)(int, int);

int apply(BinaryOp op, int a, int b) {
    return op(a, b);
}

int main() {
    BinaryOp op = add;
    assert(apply(op, 3, 4) == 7);

    op = multiply;
    assert(apply(op, 3, 4) == 12);

    std::cout << "apply(add, 3, 4) = " << apply(add, 3, 4) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Approach | Syntax |
|----------|--------|
| Raw pointer | `int (*op)(int, int) = add;` |
| `using` alias | `using BinaryOp = int (*)(int, int);` then `BinaryOp op = add;` |
| `typedef` alias | `typedef int (*BinaryOp)(int, int);` then `BinaryOp op = add;` |
| As parameter | `int apply(BinaryOp op, int a, int b)` — clean and readable |

### Practice

2\. 🌟🌟 **Fill:** Create a type alias for the function pointer and use it in the `apply` function signature.

```cpp,editable
#include <iostream>
#include <cassert>

int square(int x) { return x * x; }
int negate(int x) { return -x; }

// Create a type alias 'UnaryOp' for a pointer to a function
// that takes one int and returns an int
__

int apply(__ op, int x) {
    return op(x);
}

int main() {
    assert(apply(square, 5) == 25);
    assert(apply(negate, 7) == -7);

    std::cout << "apply(square, 5) = " << apply(square, 5) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::function

### How it works

`std::function<R(Args...)>` is a general-purpose, type-erased callable wrapper from `<functional>`. It can store function pointers, lambdas, and functors — anything callable with the matching signature. It has a small overhead compared to raw function pointers but is far more flexible.

```cpp
#include <iostream>
#include <functional>
#include <cassert>

int add(int a, int b) { return a + b; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

int main() {
    // Wraps a function pointer
    std::function<int(int, int)> op = add;
    assert(op(3, 4) == 7);

    // Wraps a lambda
    op = [](int a, int b) { return a - b; };
    assert(op(10, 3) == 7);

    // Wraps a functor
    std::function<int(int)> transform = Multiplier{3};
    assert(transform(5) == 15);

    std::cout << "op(10, 3) = " << op(10, 3) << std::endl;
    std::cout << "transform(5) = " << transform(5) << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | `std::function` | Raw function pointer |
|---------|-----------------|---------------------|
| Stores lambdas | Yes (even with captures) | No (only captureless) |
| Stores functors | Yes | No |
| Stores function ptrs | Yes | Yes |
| Overhead | Small (heap allocation possible) | None |
| Header | `<functional>` | None |

### Practice

3\. 🌟🌟 **Fill:** Use `std::function` to store a lambda and a function pointer in the same variable.

```cpp,editable
#include <iostream>
#include <functional>
#include <cassert>

int multiply(int a, int b) { return a * b; }

int main() {
    // Declare op as std::function that takes two ints and returns int
    __ op;

    // Assign a lambda that adds two ints
    op = [](int a, int b) { return a + b; };
    assert(op(3, 4) == 7);

    // Reassign to the multiply function
    op = multiply;
    assert(op(3, 4) == 12);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Callbacks

### How it works

A **callback** is a function passed as an argument to another function. The receiving function calls it at the appropriate time. This pattern is used everywhere: sorting comparators, event handlers, and applying transformations to collections.

```cpp
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

void for_each(const std::vector<int>& v,
              const std::function<void(int)>& callback) {
    for (int x : v) {
        callback(x);
    }
}

std::vector<int> transform(const std::vector<int>& v,
                           const std::function<int(int)>& fn) {
    std::vector<int> result;
    for (int x : v) {
        result.push_back(fn(x));
    }
    return result;
}

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // Print each element
    for_each(nums, [](int x) {
        std::cout << x << " ";
    });
    std::cout << std::endl;

    // Double each element
    auto doubled = transform(nums, [](int x) { return x * 2; });
    assert(doubled[0] == 2);
    assert(doubled[4] == 10);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Pattern | Description |
|---------|-------------|
| Callback | A function passed as an argument, called later by the receiver |
| Comparator | Callback that returns `bool` for ordering (e.g., `std::sort`) |
| Transform | Callback that maps a value to a new value |
| Event handler | Callback triggered when something happens |

### Practice

4\. 🌟🌟🌟 **Complete:** Write the `apply_to_all` function that takes a vector and a callback, and returns a new vector with the callback applied to each element.

```cpp,editable
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

// Write apply_to_all: takes a vector<int> and a function<int(int)>,
// returns a new vector with the function applied to each element
__

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};

    auto squared = apply_to_all(nums, [](int x) { return x * x; });
    assert(squared.size() == 5);
    assert(squared[0] == 1);
    assert(squared[1] == 4);
    assert(squared[2] == 9);
    assert(squared[4] == 25);

    auto negated = apply_to_all(nums, [](int x) { return -x; });
    assert(negated[0] == -1);
    assert(negated[4] == -5);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟🌟 **Complete:** Write a `compose` function that takes two `std::function<int(int)>` and returns a new function that applies the first, then the second.

```cpp,editable
#include <iostream>
#include <functional>
#include <cassert>

// Write compose(f, g) that returns a function h where h(x) = g(f(x))
__

int main() {
    std::function<int(int)> double_it = [](int x) { return x * 2; };
    std::function<int(int)> add_one = [](int x) { return x + 1; };

    auto double_then_add = compose(double_it, add_one);
    // double_then_add(3) = add_one(double_it(3)) = add_one(6) = 7

    assert(double_then_add(3) == 7);
    assert(double_then_add(5) == 11);
    assert(double_then_add(0) == 1);

    auto add_then_double = compose(add_one, double_it);
    // add_then_double(3) = double_it(add_one(3)) = double_it(4) = 8

    assert(add_then_double(3) == 8);
    assert(add_then_double(5) == 12);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
