# Classes

Classes in C++ provide **encapsulation** — bundling data and the functions that operate on it, while controlling access. Unlike structs, class members are `private` by default, encouraging you to expose only what is necessary through a well-defined public interface.

## Class Basics

### How it works

```cpp
#include <iostream>

class Counter {
    int count_;      // private by default in a class

public:
    Counter() : count_(0) {}

    void increment() { count_++; }
    void decrement() { count_--; }
    int get() const { return count_; }
};

int main() {
    Counter c;
    c.increment();
    c.increment();
    c.increment();
    std::cout << "Count: " << c.get() << std::endl;  // 3

    // c.count_ = 10;  // ERROR: count_ is private

    return 0;
}
```

| Access Specifier | From class itself? | From derived class? | From outside? |
|------------------|:---:|:---:|:---:|
| `private` | Yes | No | No |
| `protected` | Yes | Yes | No |
| `public` | Yes | Yes | Yes |

### Practice

1. 🌟 Fix the error: `count` is a private member and cannot be accessed directly. Use the public interface instead.

```cpp,editable
#include <iostream>
#include <cassert>

class Counter {
    int count;  // private by default in class

public:
    Counter() : count(0) {}
    void increment() { count++; }
    int get() const { return count; }
};

int main() {
    Counter c;
    c.count = 10;  // ERROR: count is private

    assert(c.get() == 10);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Getters and Setters

### How it works

```cpp
#include <iostream>
#include <string>

class Person {
private:
    std::string name_;
    int age_;

public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}

    // Getter — returns value, marked const
    std::string getName() const { return name_; }
    int getAge() const { return age_; }

    // Setter — modifies value, can include validation
    void setName(const std::string& name) { name_ = name; }
    void setAge(int age) {
        if (age >= 0) age_ = age;
    }
};

int main() {
    Person p("Alice", 25);
    std::cout << p.getName() << ", " << p.getAge() << std::endl;  // Alice, 25

    p.setName("Bob");
    p.setAge(30);
    std::cout << p.getName() << ", " << p.getAge() << std::endl;  // Bob, 30

    return 0;
}
```

- **Getters** provide read access to private data — mark them `const`
- **Setters** provide write access — can include validation logic
- This pattern lets you change internal representation without breaking the public API

### Practice

2. 🌟🌟 Add `getName()` and `setName()` methods to the `Person` class.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Person {
private:
    std::string name_;
    int age_;

public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}

    // Add getter and setter for name_
    __
    __

    int getAge() const { return age_; }
};

int main() {
    Person p("Alice", 25);

    assert(p.getName() == "Alice");

    p.setName("Bob");
    assert(p.getName() == "Bob");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Static Members

### How it works

```cpp
#include <iostream>

class Widget {
    static int count_;

public:
    Widget()  { count_++; }
    ~Widget() { count_--; }

    static int getCount() { return count_; }
};

int Widget::count_ = 0;   // must be defined outside the class

int main() {
    std::cout << "Widgets: " << Widget::getCount() << std::endl;  // 0

    Widget a;
    std::cout << "Widgets: " << Widget::getCount() << std::endl;  // 1

    {
        Widget b, c;
        std::cout << "Widgets: " << Widget::getCount() << std::endl;  // 3
    }

    std::cout << "Widgets: " << Widget::getCount() << std::endl;  // 1

    return 0;
}
```

- `static` members belong to the **class**, not to any particular instance
- All instances share the same `static` variable
- `static` member variables must be **defined** outside the class (typically in a `.cpp` file)
- `static` member functions can be called without an object: `Widget::getCount()`

### Practice

3. 🌟🌟 Fill in the constructor and destructor to track the number of live `Widget` instances.

```cpp,editable
#include <iostream>
#include <cassert>

class Widget {
private:
    static int count_;

public:
    Widget() { __; }
    ~Widget() { __; }

    static int getCount() { return count_; }
};

int Widget::count_ = 0;

int main() {
    assert(Widget::getCount() == 0);

    Widget a;
    assert(Widget::getCount() == 1);

    {
        Widget b;
        Widget c;
        assert(Widget::getCount() == 3);
    }

    assert(Widget::getCount() == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Const Methods

### How it works

```cpp
#include <iostream>

class Circle {
    double radius_;

public:
    Circle(double r) : radius_(r) {}

    // const method: promises not to modify the object
    double area() const {
        return 3.14159 * radius_ * radius_;
    }

    // non-const method: allowed to modify the object
    void setRadius(double r) {
        radius_ = r;
    }
};

int main() {
    const Circle c(5.0);
    std::cout << "Area: " << c.area() << std::endl;  // OK: area() is const

    // c.setRadius(10.0);  // ERROR: can't call non-const method on const object

    return 0;
}
```

- A `const` method has `const` after its parameter list: `double area() const`
- It promises not to modify any member variables
- Only `const` methods can be called on a `const` object
- Mark all methods that don't modify state as `const` — it's good practice

### Practice

4. 🌟🌟 Fix the error: `area()` is called on a `const` object, but `area()` is not marked `const`.

```cpp,editable
#include <iostream>
#include <cassert>

class Circle {
    double radius_;

public:
    Circle(double r) : radius_(r) {}

    double area() {  // Missing const qualifier
        return 3.14159 * radius_ * radius_;
    }
};

int main() {
    const Circle c(5.0);
    double a = c.area();  // ERROR: calling non-const method on const object

    assert(a > 78.0);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
