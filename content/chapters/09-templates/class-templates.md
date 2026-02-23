# Class Templates

Class templates let you define a blueprint for a class where one or more types (or values) are parameters. The compiler generates a concrete class for each unique set of template arguments you use.

## Basic Class Template

A class template works just like a function template but for entire classes. You can parameterise the element type, container size, or any compile-time value.

### How it works

```cpp
#include <iostream>
#include <stdexcept>

template<typename T>
class Stack {
    static constexpr int MAX = 100;
    T data_[MAX];
    int top_ = -1;
public:
    void push(T val) {
        if (top_ >= MAX - 1) throw std::overflow_error("Stack full");
        data_[++top_] = val;
    }
    T pop() {
        if (top_ < 0) throw std::underflow_error("Stack empty");
        return data_[top_--];
    }
    bool empty() const { return top_ == -1; }
};

int main() {
    Stack<int> si;
    si.push(10);
    si.push(20);
    std::cout << si.pop() << std::endl;  // 20
    std::cout << si.pop() << std::endl;  // 10
    std::cout << std::boolalpha << si.empty() << std::endl;  // true

    Stack<std::string> ss;
    ss.push("hello");
    std::cout << ss.pop() << std::endl;  // hello
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<typename T>` | `T` is substituted with the actual type at instantiation |
| `Stack<int>` | Instantiates the template with `T = int` |
| `++top_` then store | Pre-increment moves top up, then writes the value |
| `top_--` after read | Returns current value, then decrements top |

### Practice

1. 🌟🌟 **Fill:** Complete `push`, `pop`, and `empty` for the `Stack` class template.

```cpp,editable
#include <iostream>
#include <cassert>
#include <stdexcept>

template<typename T>
class Stack {
    static constexpr int MAX = 100;
    T data_[MAX];
    int top_ = -1;
public:
    void push(T val) {
        __  // store val at the next position
    }
    T pop() {
        __  // return the top element and move top down
    }
    bool empty() const {
        __  // return true when stack has no elements
    }
};

int main() {
    Stack<int> s;
    assert(s.empty() == true);

    s.push(10);
    s.push(20);
    assert(s.empty() == false);
    assert(s.pop() == 20);
    assert(s.pop() == 10);
    assert(s.empty() == true);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Non-type Template Parameters

Templates can also take **compile-time values** (not just types). A common use is an integer `N` that sets the size of a fixed-size container, avoiding heap allocation entirely.

### How it works

```cpp
#include <iostream>

template<typename T, int N>
class FixedArray {
    T data_[N];
public:
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
    constexpr int size() const { return N; }
};

int main() {
    FixedArray<int, 5> arr;
    for (int i = 0; i < arr.size(); ++i)
        arr[i] = i * 10;

    for (int i = 0; i < arr.size(); ++i)
        std::cout << arr[i] << " ";  // 0 10 20 30 40
    std::cout << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `int N` | Non-type parameter — must be a compile-time constant |
| `FixedArray<int, 5>` | Creates a fixed-size array of 5 ints, no heap allocation |
| `constexpr` on `size()` | Allows use in compile-time contexts |
| Allowed non-type params | Integral types, pointers, references, enums (C++20: also floats, class types) |

### Practice

2. 🌟🌟🌟 **Fill:** Implement the `size()` method that returns the non-type template parameter `N`.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename T, int N>
class FixedArray {
    T data_[N]{};
public:
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
    constexpr int size() const { __ }
};

int main() {
    FixedArray<int, 5> arr;
    assert(arr.size() == 5);

    FixedArray<double, 3> darr;
    assert(darr.size() == 3);

    for (int i = 0; i < arr.size(); ++i)
        arr[i] = i * 10;

    assert(arr[0] == 0);
    assert(arr[4] == 40);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Variadic Templates

**Variadic templates** accept an arbitrary number of template arguments using a **parameter pack** (`typename... Args`). They are expanded recursively or with fold expressions (C++17) to process each argument.

### How it works

```cpp
#include <iostream>

// Base case: single argument
void print(auto last) {
    std::cout << last << std::endl;
}

// Recursive case: peel off first, recurse on rest
template<typename T, typename... Rest>
void print(T first, Rest... rest) {
    std::cout << first << " ";
    print(rest...);  // expand the parameter pack
}

int main() {
    print(1, 2.5, "hello", 'x');  // 1 2.5 hello x
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `typename... Rest` | Template parameter pack — zero or more types |
| `Rest... rest` | Function parameter pack — matching arguments |
| `rest...` | Pack expansion — passes remaining args to recursive call |
| Base case | A one-argument overload stops the recursion |

### Practice

3. 🌟🌟🌟 **Fill:** Complete the recursive `print` function by calling `print(rest...)` to expand the parameter pack.

```cpp,editable
#include <iostream>
#include <sstream>
#include <cassert>

std::ostringstream oss;

void print(auto last) {
    oss << last;
}

template<typename T, typename... Rest>
void print(T first, Rest... rest) {
    oss << first << " ";
    __  // expand the remaining arguments
}

int main() {
    print(1, 2.5, "hello");
    assert(oss.str() == "1 2.5 hello");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
