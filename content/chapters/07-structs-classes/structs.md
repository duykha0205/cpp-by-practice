# Structs

A `struct` in C++ groups related data fields into a single type. Unlike classes, struct members are `public` by default, making them ideal for simple data containers. Structs can also have member functions and operator overloads, giving them the same capabilities as classes.

## Struct Basics

### How it works

```cpp
#include <iostream>

struct Point {
    double x;
    double y;
};

int main() {
    Point p;
    p.x = 3.0;
    p.y = 4.0;

    std::cout << "Point: (" << p.x << ", " << p.y << ")" << std::endl;  // (3, 4)

    Point q = {1.0, 2.0};
    std::cout << "Point: (" << q.x << ", " << q.y << ")" << std::endl;  // (1, 2)

    return 0;
}
```

- `struct` defines a new type that bundles multiple variables together
- Members are accessed with the dot operator (`.`)
- Members are `public` by default (unlike `class`, where they are `private`)

### Practice

1. 🌟 Fill in the blanks with the correct values after setting the struct members.

```cpp,editable
#include <iostream>
#include <cassert>

struct Point {
    double x;
    double y;
};

int main() {
    Point p;
    p.x = 3.0;
    p.y = 4.0;

    assert(p.x == __);
    assert(p.y == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Aggregate Initialization

### How it works

```cpp
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
};

int main() {
    // Aggregate initialization (ordered by declaration)
    Person p1 = {"Alice", 30};
    std::cout << p1.name << " is " << p1.age << std::endl;  // Alice is 30

    // Designated initializers (C++20) — explicit field names
    Person p2 = {.name = "Bob", .age = 25};
    std::cout << p2.name << " is " << p2.age << std::endl;  // Bob is 25

    return 0;
}
```

- **Aggregate initialization**: values are assigned to members in declaration order
- **Designated initializers** (C++20): use `.field = value` for clarity — order must match declaration order
- Both forms require matching types and count

### Practice

2. 🌟🌟 Fill in the blanks to initialize a `Person` using aggregate initialization.

```cpp,editable
#include <iostream>
#include <cassert>
#include <string>

struct Person {
    std::string name;
    int age;
};

int main() {
    Person p = {__, __};

    assert(p.name == "Alice");
    assert(p.age == 30);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Member Functions

### How it works

```cpp
#include <iostream>
#include <cmath>

struct Point {
    double x;
    double y;

    double distance_to_origin() const {
        return std::sqrt(x * x + y * y);
    }

    void print() const {
        std::cout << "(" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    Point p = {3.0, 4.0};
    p.print();                                                      // (3, 4)
    std::cout << "Distance: " << p.distance_to_origin() << std::endl;  // 5

    return 0;
}
```

- Structs can have **member functions** (methods) just like classes
- Use `const` after the parameter list if the method does not modify the struct
- Inside a member function, you can access all members directly (no dot needed)

### Practice

3. 🌟🌟 Add a `distance_to_origin()` method that returns `sqrt(x*x + y*y)`.

```cpp,editable
#include <iostream>
#include <cassert>
#include <cmath>

struct Point {
    double x;
    double y;

    // Add distance_to_origin() method here
    __
};

int main() {
    Point p = {3.0, 4.0};

    assert(std::abs(p.distance_to_origin() - 5.0) < 0.001);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Operator Overloading

### How it works

```cpp
#include <iostream>

struct Vec2 {
    int x, y;

    bool operator==(const Vec2& other) const {
        return x == other.x && y == other.y;
    }

    Vec2 operator+(const Vec2& other) const {
        return {x + other.x, y + other.y};
    }
};

int main() {
    Vec2 a = {1, 2};
    Vec2 b = {3, 4};
    Vec2 c = a + b;

    std::cout << "c = (" << c.x << ", " << c.y << ")" << std::endl;  // (4, 6)
    std::cout << "a == b? " << (a == b) << std::endl;                  // 0 (false)

    return 0;
}
```

- C++ does **not** auto-generate `operator==` for structs (prior to C++20's `= default`)
- You can overload operators like `==`, `+`, `<`, `<<` to give your types natural syntax
- Operator overloads are member functions with special names

### Practice

4. 🌟🌟 Add `operator==` to the `Vec2` struct so that two vectors can be compared.

```cpp,editable
#include <iostream>
#include <cassert>

struct Vec2 {
    int x, y;

    // Add operator== here
    __
};

int main() {
    Vec2 a = {1, 2};
    Vec2 b = {1, 2};
    Vec2 c = {3, 4};

    assert(a == b);
    assert(!(a == c));

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
