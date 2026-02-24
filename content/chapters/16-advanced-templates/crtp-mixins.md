# CRTP & Mixin Patterns

The Curiously Recurring Template Pattern (CRTP) and mixin classes are advanced template techniques for achieving **static polymorphism** and **composable functionality**. CRTP lets a base class call derived-class methods without virtual dispatch, while mixins add reusable behavior to any class through template inheritance. Together they enable zero-overhead abstractions that rival hand-written code.

## Curiously Recurring Template Pattern

In CRTP, a class `Derived` inherits from `Base<Derived>`. The base class can `static_cast` its `this` pointer to `Derived*` and call methods on it — all resolved at compile time with no vtable overhead.

### How it works

```cpp
#include <iostream>

template<typename Derived>
class Shape {
public:
    double area() const {
        // static_cast to Derived and call its implementation
        return static_cast<const Derived*>(this)->area_impl();
    }

    void describe() const {
        std::cout << "Area: " << area() << std::endl;
    }
};

class Circle : public Shape<Circle> {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    double area_impl() const { return 3.14159265 * radius_ * radius_; }
};

class Square : public Shape<Square> {
    double side_;
public:
    Square(double s) : side_(s) {}
    double area_impl() const { return side_ * side_; }
};

int main() {
    Circle c(5.0);
    c.describe();  // Area: 78.5398

    Square s(4.0);
    s.describe();  // Area: 16
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `Base<Derived>` | Base class is templated on the derived class |
| `static_cast<const Derived*>(this)` | Safe because `this` is always a `Derived` object |
| Compile-time dispatch | No virtual table, no runtime indirection |
| `area_impl()` | Convention: derived provides `_impl` methods |
| Limitation | Cannot use base pointers polymorphically (no common base) |

### Practice

1. 🌟🌟🌟 **Complete:** Implement a CRTP base class `Printable` that provides a `print()` method. It should call `to_string_impl()` on the derived class. Then complete the `Name` and `Number` classes.

```cpp,editable
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

template<typename Derived>
class Printable {
public:
    std::string to_string() const {
        return __;
    }

    void print() const {
        std::cout << to_string() << std::endl;
    }
};

class Name : public Printable<Name> {
    std::string name_;
public:
    Name(const std::string& n) : name_(n) {}
    std::string to_string_impl() const {
        return __;
    }
};

class Number : public Printable<Number> {
    int value_;
public:
    Number(int v) : value_(v) {}
    std::string to_string_impl() const {
        return __;
    }
};

int main() {
    Name n("Alice");
    assert(n.to_string() == "Name: Alice");
    n.print();

    Number num(42);
    assert(num.to_string() == "Number: 42");
    num.print();

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## CRTP vs Virtual Dispatch

CRTP provides static polymorphism — the dispatch happens at compile time, so there is no vtable pointer per object and no indirect call overhead. Virtual dispatch, on the other hand, provides runtime polymorphism — you can store different derived types behind a base pointer. The choice depends on whether you need runtime flexibility or maximum performance.

### How it works

```cpp
#include <iostream>

// --- Virtual dispatch version ---
class AnimalV {
public:
    virtual ~AnimalV() = default;
    virtual std::string speak() const = 0;
    void greet() const {
        std::cout << speak() << "!" << std::endl;  // runtime dispatch
    }
};

class DogV : public AnimalV {
public:
    std::string speak() const override { return "Woof"; }
};

// --- CRTP version ---
template<typename Derived>
class AnimalC {
public:
    std::string speak() const {
        return static_cast<const Derived*>(this)->speak_impl();  // compile-time
    }
    void greet() const {
        std::cout << speak() << "!" << std::endl;
    }
};

class DogC : public AnimalC<DogC> {
public:
    std::string speak_impl() const { return "Woof"; }
};

int main() {
    DogV dv;
    dv.greet();  // Woof! (virtual dispatch)

    DogC dc;
    dc.greet();  // Woof! (static dispatch, no vtable)
    return 0;
}
```

| Aspect | Virtual Dispatch | CRTP |
|--------|-----------------|------|
| Dispatch time | Runtime | Compile time |
| Overhead | vtable pointer per object + indirect call | Zero |
| Heterogeneous collections | Yes (`std::vector<Base*>`) | No (each `Base<D>` is a different type) |
| Adding new types | Easy (just inherit and override) | Easy (just inherit and implement `_impl`) |
| Use when | Types determined at runtime | Types known at compile time, performance critical |

### Practice

2. 🌟🌟🌟 **Complete:** Convert the virtual `Comparator` class to use CRTP. Implement `less_impl` in both `IntAscending` and `IntDescending`, and have the CRTP base provide `less()` and `greater()` methods.

```cpp,editable
#include <iostream>
#include <cassert>

template<typename Derived>
class Comparator {
public:
    bool less(int a, int b) const {
        return __;
    }

    bool greater(int a, int b) const {
        return __;
    }

    bool equal(int a, int b) const {
        return !less(a, b) && !greater(a, b);
    }
};

class IntAscending : public Comparator<IntAscending> {
public:
    // Natural order: a < b
    bool less_impl(int a, int b) const {
        return __;
    }
};

class IntDescending : public Comparator<IntDescending> {
public:
    // Reverse order: a < b means a is actually greater
    bool less_impl(int a, int b) const {
        return __;
    }
};

int main() {
    IntAscending asc;
    assert(asc.less(1, 2) == true);
    assert(asc.less(2, 1) == false);
    assert(asc.greater(5, 3) == true);
    assert(asc.equal(4, 4) == true);

    IntDescending desc;
    assert(desc.less(2, 1) == true);   // 2 comes before 1 in descending
    assert(desc.less(1, 2) == false);
    assert(desc.greater(1, 5) == true); // 1 comes after 5 in descending
    assert(desc.equal(3, 3) == true);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Mixin Classes

Mixins add reusable functionality to a class through template inheritance. A mixin is a template class that inherits from its template parameter, layering additional behavior on top. Multiple mixins can be composed together, each adding its own feature without modifying the base class.

### How it works

```cpp
#include <iostream>
#include <string>

// Base class
class Widget {
    std::string name_;
public:
    Widget(const std::string& name) : name_(name) {}
    std::string name() const { return name_; }
};

// Mixin: adds logging
template<typename Base>
class WithLogging : public Base {
public:
    using Base::Base;  // inherit constructors
    void log(const std::string& msg) const {
        std::cout << "[" << this->name() << "] " << msg << std::endl;
    }
};

// Mixin: adds serialization
template<typename Base>
class WithSerialize : public Base {
public:
    using Base::Base;
    std::string serialize() const {
        return "{\"name\": \"" + this->name() + "\"}";
    }
};

// Compose mixins
using LoggedWidget     = WithLogging<Widget>;
using FullWidget       = WithSerialize<WithLogging<Widget>>;

int main() {
    LoggedWidget lw("Button");
    lw.log("clicked");  // [Button] clicked

    FullWidget fw("Panel");
    fw.log("created");  // [Panel] created
    std::cout << fw.serialize() << std::endl;  // {"name": "Panel"}
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<typename Base> class Mixin : public Base` | Mixin inherits from its parameter |
| `using Base::Base` | Inherits all constructors from the base |
| Composition | `WithB<WithA<Core>>` layers A then B on Core |
| Order matters | Inner mixin is constructed first |
| No diamond problem | Each mixin is a linear chain of inheritance |

### Practice

3. 🌟🌟🌟 **Complete:** Create two mixins: `WithCounter` that tracks how many times `increment()` is called, and `WithUpperCase` that provides an `upper_name()` method returning the name in uppercase. Compose them on a `Person` base class.

```cpp,editable
#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

class Person {
    std::string name_;
public:
    Person(const std::string& name) : name_(name) {}
    std::string name() const { return name_; }
};

// Mixin: adds a counter that tracks calls to increment()
template<typename Base>
class WithCounter : public Base {
    int count_ = 0;
public:
    using Base::Base;
    void increment() { __; }
    int count() const { return __; }
};

// Mixin: adds upper_name() that returns name in uppercase
template<typename Base>
class WithUpperCase : public Base {
public:
    using Base::Base;
    std::string upper_name() const {
        std::string result = this->name();
        __;
        return result;
    }
};

// Compose both mixins on Person
using EnhancedPerson = __;

int main() {
    EnhancedPerson p("Alice");

    assert(p.name() == "Alice");
    assert(p.upper_name() == "ALICE");

    assert(p.count() == 0);
    p.increment();
    p.increment();
    p.increment();
    assert(p.count() == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟🌟 **Complete:** Combine CRTP with mixins. Create a CRTP base `Drawable` that calls `draw_impl()`, and a mixin `WithBorder` that wraps the draw output. The final `Circle` class should inherit from the border mixin layered on the CRTP base.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

// CRTP base: provides draw() that delegates to Derived::draw_impl()
template<typename Derived>
class Drawable {
public:
    std::string draw() const {
        return __;
    }
};

// Mixin: wraps draw() output with a border
template<typename Base>
class WithBorder : public Base {
public:
    using Base::Base;
    std::string draw_with_border() const {
        return "[" + __ + "]";
    }
};

// Circle uses CRTP for drawing and mixin for border
class Circle : public WithBorder<Drawable<Circle>> {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    std::string draw_impl() const {
        return __;
    }
};

int main() {
    Circle c(5.0);

    assert(c.draw() == "Circle(5)");
    assert(c.draw_with_border() == "[Circle(5)]");

    c.draw_with_border();

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
