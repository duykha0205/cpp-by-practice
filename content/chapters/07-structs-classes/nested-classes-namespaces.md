# Nested Classes & Namespaces

Nested classes let you define a class inside another class, keeping helper types tightly coupled with the type that uses them. Namespaces organize code into logical groups and prevent name collisions in large projects. Together, they are essential tools for structuring real-world C++ codebases.

## Nested Classes

### How it works

A nested class (inner class) is defined inside the scope of another class (outer class). It can access the outer class's static members and types directly, but it needs an explicit reference to access non-static members of a specific outer object.

```cpp
#include <iostream>
#include <string>

class LinkedList {
public:
    // Nested class — only meaningful in the context of LinkedList
    class Node {
    public:
        int data;
        Node* next;
        Node(int d) : data(d), next(nullptr) {}
    };

private:
    Node* head_;

public:
    LinkedList() : head_(nullptr) {}

    void push_front(int value) {
        Node* new_node = new Node(value);
        new_node->next = head_;
        head_ = new_node;
    }

    int front() const { return head_->data; }

    ~LinkedList() {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }
};

int main() {
    LinkedList list;
    list.push_front(10);
    list.push_front(20);

    std::cout << "Front: " << list.front() << std::endl;  // 20

    // Can also use the nested type directly
    LinkedList::Node standalone(42);
    std::cout << "Standalone: " << standalone.data << std::endl;  // 42
    return 0;
}
```

| Aspect | Details |
|---|---|
| Syntax | `class Outer { class Inner { ... }; };` |
| Access from outside | `Outer::Inner` |
| Inner accesses outer statics | Directly, without qualification |
| Inner accesses outer non-statics | Needs an explicit outer object reference |
| Use cases | Iterator types, Node types, implementation details |

### Practice

1\. 🌟🌟 **Complete:** define a nested `Pixel` class inside `Image` that stores r, g, b values. Fill in the nested class and the `getPixel` method.

```cpp,editable
#include <iostream>
#include <cassert>

class Image {
public:
    // Define a nested Pixel class with int r, g, b members
    // and a constructor that takes r, g, b
    __

private:
    Pixel pixels_[2];

public:
    Image() : pixels_{{255, 0, 0}, {0, 255, 0}} {}

    // Return a const reference to the pixel at the given index
    __

    int width() const { return 2; }
};

int main() {
    Image img;

    assert(img.getPixel(0).r == 255);
    assert(img.getPixel(0).g == 0);
    assert(img.getPixel(1).g == 255);
    assert(img.getPixel(1).b == 0);

    // Can also use the nested type directly
    Image::Pixel p(0, 0, 255);
    assert(p.b == 255);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Namespaces

### How it works

Namespaces group related declarations under a name, preventing collisions when different libraries define types or functions with the same name. C++17 introduced the compact nested namespace syntax `A::B::C`.

```cpp
#include <iostream>

namespace Math {
    double pi = 3.14159;

    double square(double x) {
        return x * x;
    }

    namespace Trig {
        double degrees_to_radians(double deg) {
            return deg * pi / 180.0;
        }
    }
}

// C++17 nested namespace shorthand (equivalent to namespace A { namespace B { ... } })
namespace Graphics::Color {
    struct RGB {
        int r, g, b;
    };
}

int main() {
    std::cout << Math::pi << std::endl;                        // 3.14159
    std::cout << Math::square(5.0) << std::endl;               // 25
    std::cout << Math::Trig::degrees_to_radians(180) << std::endl;  // 3.14159

    Graphics::Color::RGB red{255, 0, 0};
    std::cout << "R: " << red.r << std::endl;  // 255
    return 0;
}
```

| Feature | Syntax | Version |
|---|---|---|
| Namespace declaration | `namespace N { ... }` | C++98 |
| Nested namespace | `namespace A { namespace B { ... } }` | C++98 |
| Nested shorthand | `namespace A::B { ... }` | C++17 |
| Accessing members | `N::member` | C++98 |
| Reopening a namespace | Declare `namespace N { ... }` again to add more | C++98 |

### Practice

2\. 🌟 **Fill:** the blanks to access members from the defined namespaces.

```cpp,editable
#include <iostream>
#include <cassert>

namespace Physics {
    double gravity = 9.81;

    namespace Units {
        std::string acceleration = "m/s^2";
    }
}

namespace Chemistry::Elements {
    int hydrogen_atomic_number = 1;
    int oxygen_atomic_number = 8;
}

int main() {
    assert(__::gravity > 9.80);
    assert(__::__::acceleration == "m/s^2");
    assert(__::__::hydrogen_atomic_number == 1);
    assert(__::__::oxygen_atomic_number == 8);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3\. 🌟🌟 **Complete:** create a namespace `Geometry` with a nested namespace `Shapes` (using C++17 shorthand) containing a `Circle` struct with a `radius` member and an `area()` method.

```cpp,editable
#include <iostream>
#include <cassert>

// Define namespace Geometry::Shapes using C++17 shorthand
// containing a Circle struct with:
//   - double radius member
//   - double area() const method returning 3.14159 * radius * radius
__

int main() {
    Geometry::Shapes::Circle c{5.0};

    assert(c.radius == 5.0);
    double a = c.area();
    assert(a > 78.5 && a < 78.6);

    Geometry::Shapes::Circle c2{1.0};
    assert(c2.area() > 3.14 && c2.area() < 3.15);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Using Declarations & Directives

### How it works

A `using` directive (`using namespace N;`) imports all names from a namespace. A `using` declaration (`using N::x;`) imports only a specific name. Anonymous namespaces provide internal linkage (file-scope visibility), and inline namespaces (C++11) make their members available in the enclosing namespace.

```cpp
#include <iostream>
#include <string>

namespace Logging {
    void info(const std::string& msg) {
        std::cout << "[INFO] " << msg << std::endl;
    }
    void warn(const std::string& msg) {
        std::cout << "[WARN] " << msg << std::endl;
    }
}

// Anonymous namespace — internal linkage, like static at file scope
namespace {
    int internal_counter = 0;
}

namespace Library {
    inline namespace v2 {
        int version() { return 2; }
    }

    namespace v1 {
        int version() { return 1; }
    }
}

int main() {
    // using directive: imports everything
    using namespace Logging;
    info("Application started");

    // using declaration: imports one name
    using Logging::warn;
    warn("Low memory");

    // Anonymous namespace variable is accessible in this file
    internal_counter++;
    std::cout << "Counter: " << internal_counter << std::endl;  // 1

    // Inline namespace: Library::version() resolves to v2::version()
    std::cout << "Library version: " << Library::version() << std::endl;     // 2
    std::cout << "v1 version: " << Library::v1::version() << std::endl;      // 1

    return 0;
}
```

| Feature | Syntax | Effect |
|---|---|---|
| Using directive | `using namespace N;` | Imports all names from `N` |
| Using declaration | `using N::x;` | Imports only `x` from `N` |
| Anonymous namespace | `namespace { ... }` | Internal linkage (visible only in this file) |
| Inline namespace | `inline namespace V { ... }` | Members accessible via enclosing namespace |

### Practice

4\. 🌟 **Fix:** the code uses fully qualified names everywhere. Simplify it with the appropriate `using` declarations.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

namespace StringUtils {
    std::string to_upper(const std::string& s) {
        std::string result = s;
        for (char& c : result) {
            if (c >= 'a' && c <= 'z') c -= 32;
        }
        return result;
    }

    std::string to_lower(const std::string& s) {
        std::string result = s;
        for (char& c : result) {
            if (c >= 'A' && c <= 'Z') c += 32;
        }
        return result;
    }
}

int main() {
    // Add a using declaration for StringUtils::to_upper and StringUtils::to_lower
    // so you can call them without the namespace prefix
    __
    __

    // After adding the using declarations, these should compile:
    assert(to_upper("hello") == "HELLO");
    assert(to_lower("WORLD") == "world");
    assert(to_upper("c++") == "C++");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟🌟 **Complete:** use an inline namespace to implement API versioning. The `Api` namespace should have `v1` and `v2` sub-namespaces, with `v2` as the default (inline). Fill in the namespace structure.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

namespace Api {
    // v1 namespace (not inline — must be accessed explicitly)
    __

    // v2 namespace (inline — becomes the default)
    __
}

int main() {
    // Api::greet() should use v2 (the inline namespace)
    assert(Api::greet("World") == "Hello, World!");

    // Explicit v1 access
    assert(Api::v1::greet("World") == "Hi World");

    // Explicit v2 access should match the default
    assert(Api::v2::greet("World") == "Hello, World!");
    assert(Api::greet("C++") == Api::v2::greet("C++"));

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
