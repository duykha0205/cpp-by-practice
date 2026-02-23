# Move Semantics

Move semantics allow C++ to **transfer** resources (heap memory, file handles, etc.) from one object to another instead of copying them. This eliminates unnecessary deep copies and is a major performance feature of modern C++.

## Lvalues vs Rvalues

An **lvalue** has an identity (a name, an address you can take). An **rvalue** is a temporary with no persistent identity. Understanding this distinction is key to move semantics.

### How it works

```cpp
#include <iostream>
#include <string>

void process(int& x)        { std::cout << "lvalue ref: " << x << std::endl; }
void process(int&& x)       { std::cout << "rvalue ref: " << x << std::endl; }
void process(const int& x)  { std::cout << "const ref:  " << x << std::endl; }

int main() {
    int a = 10;
    process(a);       // lvalue ref: 10
    process(20);      // rvalue ref: 20

    const int b = 30;
    process(b);       // const ref: 30
    return 0;
}
```

| Category | Example | Can bind to `&` | Can bind to `&&` | Can bind to `const&` |
|----------|---------|:-:|:-:|:-:|
| Lvalue | `a`, `*ptr`, `arr[0]` | Yes | No | Yes |
| Rvalue | `42`, `a+b`, `func()` | No | Yes | Yes |
| Const lvalue | `const int b` | No | No | Yes |

### Practice

1. 🌟🌟 **Observe:** Run this code and observe which overload is called for each argument. No fix needed — just understand the output.

```cpp,editable
#include <iostream>

void identify(int& x)       { std::cout << "lvalue ref" << std::endl; }
void identify(int&& x)      { std::cout << "rvalue ref" << std::endl; }
void identify(const int& x) { std::cout << "const lvalue ref" << std::endl; }

int main() {
    int a = 10;
    const int b = 20;

    identify(a);     // ?
    identify(30);    // ?
    identify(b);     // ?

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::move

`std::move` doesn't actually move anything — it's a **cast** that turns an lvalue into an rvalue reference, signalling that the object's resources can be stolen. After a move, the source object is in a valid but unspecified state (typically empty).

### How it works

```cpp
#include <iostream>
#include <string>

int main() {
    std::string s1 = "Hello, World!";
    std::cout << "s1: " << s1 << std::endl;       // Hello, World!

    std::string s2 = std::move(s1);  // steal s1's buffer
    std::cout << "s2: " << s2 << std::endl;       // Hello, World!
    std::cout << "s1: '" << s1 << "'" << std::endl; // '' (empty after move)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::move(x)` | Casts `x` to `T&&` — does NOT move data |
| Move constructor | `T(T&& other)` — steals resources from `other` |
| After move | Source is valid but unspecified (usually empty) |
| When to use | Transferring ownership of expensive resources |

### Practice

2. 🌟🌟 **Observe:** Run the code to see that `s1` becomes empty after being moved into `s2`. No fix needed.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

int main() {
    std::string s1 = "Hello, World!";
    assert(s1 == "Hello, World!");

    std::string s2 = std::move(s1);
    assert(s2 == "Hello, World!");
    assert(s1.empty());  // s1 is empty after move

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Move Constructor

A **move constructor** transfers (steals) resources from a temporary or moved-from object instead of copying them. This is O(1) for pointer-based resources vs O(n) for a deep copy.

### How it works

```cpp
#include <iostream>
#include <cstring>
#include <utility>

class Buffer {
    char* data_;
    size_t size_;
public:
    Buffer(const char* str)
        : size_(std::strlen(str)), data_(new char[std::strlen(str) + 1]) {
        std::strcpy(data_, str);
    }

    // Move constructor: steal resources, null out source
    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    ~Buffer() { delete[] data_; }

    size_t size() const { return size_; }
    const char* c_str() const { return data_ ? data_ : ""; }
};

int main() {
    Buffer b1("Hello");
    std::cout << b1.c_str() << " (" << b1.size() << ")" << std::endl;  // Hello (5)

    Buffer b2(std::move(b1));
    std::cout << b2.c_str() << " (" << b2.size() << ")" << std::endl;  // Hello (5)
    std::cout << b1.c_str() << " (" << b1.size() << ")" << std::endl;  // (0)
    return 0;
}
```

| Move constructor step | Purpose |
|----------------------|---------|
| `data_(other.data_)` | Steal the pointer |
| `size_(other.size_)` | Steal the size |
| `other.data_ = nullptr` | Prevent source from freeing the stolen memory |
| `other.size_ = 0` | Leave source in a valid empty state |
| `noexcept` | Enables optimizations (e.g. `std::vector` reallocation uses move only if `noexcept`) |

### Practice

3. 🌟🌟🌟 **Fill:** Complete the move constructor — steal `data_` and `size_` from `other`, then null out the source.

```cpp,editable
#include <iostream>
#include <cstring>
#include <utility>
#include <cassert>

class Buffer {
    char* data_;
    size_t size_;
public:
    Buffer(const char* str)
        : size_(std::strlen(str)), data_(new char[std::strlen(str) + 1]) {
        std::strcpy(data_, str);
    }

    // Move constructor — fill in the blanks
    Buffer(Buffer&& other) noexcept
        : data_(__), size_(__) {
        __;  // null out other's data
        __;  // zero out other's size
    }

    ~Buffer() { delete[] data_; }

    size_t size() const { return size_; }
    const char* c_str() const { return data_ ? data_ : ""; }
};

int main() {
    Buffer b1("Hello");
    assert(b1.size() == 5);

    Buffer b2(std::move(b1));
    assert(b2.size() == 5);
    assert(std::strcmp(b2.c_str(), "Hello") == 0);
    assert(b1.size() == 0);
    assert(b1.c_str()[0] == '\0');

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Perfect Forwarding

**Perfect forwarding** preserves the value category (lvalue or rvalue) of arguments when passing them through a template function. It uses `T&&` (forwarding reference) and `std::forward<T>` to avoid unnecessary copies.

### How it works

```cpp
#include <iostream>
#include <string>
#include <utility>

void process(const std::string& s) { std::cout << "lvalue: " << s << std::endl; }
void process(std::string&& s)      { std::cout << "rvalue: " << s << std::endl; }

template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));  // forwards lvalue as lvalue, rvalue as rvalue
}

int main() {
    std::string s = "hello";
    wrapper(s);                    // lvalue: hello
    wrapper(std::string("world")); // rvalue: world
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `T&&` in template | Forwarding reference (not rvalue ref!) — binds to both |
| `std::forward<T>(arg)` | Casts back to original value category |
| Without `forward` | `arg` is always an lvalue inside the function (it has a name) |
| Use case | Factory functions, `emplace`, any generic wrapper |

### Practice

4. 🌟🌟🌟 **Fill:** Use `std::forward<T>(arg)` to perfectly forward the argument to `process`.

```cpp,editable
#include <iostream>
#include <string>
#include <utility>
#include <cassert>

std::string last_call;

void process(const std::string& s) { last_call = "lvalue"; }
void process(std::string&& s)      { last_call = "rvalue"; }

template<typename T>
void wrapper(T&& arg) {
    process(__);  // perfectly forward arg
}

int main() {
    std::string s = "hello";

    wrapper(s);
    assert(last_call == "lvalue");

    wrapper(std::string("world"));
    assert(last_call == "rvalue");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
