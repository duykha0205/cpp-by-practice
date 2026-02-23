# Polymorphism

Polymorphism lets you write code that works with a base class but automatically calls the correct derived-class behaviour at runtime. C++ achieves this through virtual functions, abstract classes, and safe downcasting.

## Virtual Functions

Virtual functions enable **runtime polymorphism**: when you call a virtual function through a base-class pointer or reference, the program looks up the actual object's type (via the **vtable**) and calls the correct override.

### How it works

```cpp
#include <iostream>
#include <string>

class Shape {
public:
    virtual std::string name() const { return "Shape"; }
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    std::string name() const override { return "Circle"; }
};

int main() {
    Circle c;
    Shape* ptr = &c;

    // vtable dispatch → calls Circle::name()
    std::cout << ptr->name() << std::endl;  // Circle
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `virtual` keyword | Tells the compiler to use dynamic dispatch (vtable lookup) |
| `override` specifier | Ensures the derived function actually overrides a base virtual |
| vtable | Hidden per-class table of function pointers created by the compiler |
| Without `virtual` | The base version is always called through a base pointer (static dispatch) |

### Practice

1. 🌟🌟 **Fix:** `area_str()` is not virtual, so the base pointer calls the base version. Add the `virtual` keyword so the derived version is called.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Shape {
public:
    std::string area_str() const {  // BUG: not virtual
        return "Shape::area_str";
    }
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    std::string area_str() const {
        return "Circle::area_str";
    }
};

int main() {
    Circle c;
    Shape* ptr = &c;

    assert(ptr->area_str() == "Circle::area_str");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Pure Virtual Functions & Abstract Classes

A **pure virtual function** (`= 0`) has no base implementation. Any class with at least one pure virtual function is **abstract** and cannot be instantiated — it forces every derived class to provide its own implementation.

### How it works

```cpp
#include <iostream>
#include <cmath>

class Shape {
public:
    virtual double area() const = 0;   // pure virtual
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    Circle(double r) : r_(r) {}
    double area() const override { return M_PI * r_ * r_; }
};

int main() {
    // Shape s;          // ERROR: cannot instantiate abstract class
    Circle c(2.0);
    Shape* ptr = &c;
    std::cout << ptr->area() << std::endl;  // 12.5664
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `= 0` | Makes the function pure virtual — no body in the base class |
| Abstract class | Has at least one pure virtual function; cannot be instantiated |
| Concrete class | Provides implementations for **all** inherited pure virtuals |

### Practice

2. 🌟🌟 **Fill:** Make `Shape::area()` pure virtual and implement `Rectangle::area()`.

```cpp,editable
#include <iostream>
#include <cmath>
#include <cassert>

class Shape {
public:
    virtual double area() const __ // Make this pure virtual
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    double area() const override { return M_PI * radius_ * radius_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    __
};

int main() {
    Circle c(1.0);
    Rectangle r(3.0, 4.0);

    assert(std::abs(c.area() - M_PI) < 0.001);
    assert(std::abs(r.area() - 12.0) < 0.001);

    Shape* shapes[] = {&c, &r};
    for (auto* s : shapes) {
        std::cout << "Area: " << s->area() << std::endl;
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Virtual Destructor

When you `delete` a derived object through a **base pointer**, the base destructor must be `virtual`. Otherwise only the base destructor runs, leaking any resources allocated by the derived class.

### How it works

```cpp
#include <iostream>

class Base {
public:
    Base() { std::cout << "Base created\n"; }
    virtual ~Base() { std::cout << "Base destroyed\n"; }
};

class Derived : public Base {
    int* data_;
public:
    Derived() : data_(new int[100]) { std::cout << "Derived created\n"; }
    ~Derived() override {
        delete[] data_;
        std::cout << "Derived destroyed\n";
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // Both Derived and Base destructors run — no leak
    return 0;
}
```

| Scenario | What happens |
|----------|-------------|
| Non-virtual destructor + `delete base_ptr` | Only `~Base()` runs → **memory leak** |
| Virtual destructor + `delete base_ptr` | `~Derived()` then `~Base()` run → correct cleanup |
| **Rule of thumb** | If a class has any virtual function, give it a virtual destructor |

### Practice

3. 🌟🌟🌟 **Fix:** `Base`'s destructor is not virtual, so deleting through a base pointer skips the `Derived` destructor. Add `virtual` to fix the memory leak.

```cpp,editable
#include <iostream>

class Base {
public:
    Base() { std::cout << "Base created" << std::endl; }
    ~Base() { std::cout << "Base destroyed" << std::endl; }  // BUG: not virtual
};

class Derived : public Base {
    int* data_;
public:
    Derived() : data_(new int[100]) {
        std::cout << "Derived created" << std::endl;
    }
    ~Derived() {
        delete[] data_;
        std::cout << "Derived destroyed" << std::endl;
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Dynamic Cast

`dynamic_cast` performs a **safe downcast** on polymorphic types at runtime. If the cast is invalid it returns `nullptr` (for pointers) or throws `std::bad_cast` (for references), rather than causing undefined behaviour.

### How it works

```cpp
#include <iostream>

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const = 0;
};

class Dog : public Animal {
public:
    std::string speak() const override { return "Woof"; }
    void fetch() const { std::cout << "Fetching!\n"; }
};

class Cat : public Animal {
public:
    std::string speak() const override { return "Meow"; }
};

int main() {
    Dog d;
    Animal* a = &d;

    Dog* dog = dynamic_cast<Dog*>(a);  // succeeds
    if (dog) dog->fetch();

    Cat* cat = dynamic_cast<Cat*>(a);  // fails → nullptr
    std::cout << "Cat ptr: " << cat << std::endl;  // 0
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `dynamic_cast<T*>(ptr)` | Returns `T*` on success, `nullptr` on failure |
| `dynamic_cast<T&>(ref)` | Returns `T&` on success, throws `std::bad_cast` on failure |
| Requirement | The base class must have at least one virtual function (polymorphic type) |
| Cost | Small runtime overhead (RTTI lookup) compared to `static_cast` |

### Practice

4. 🌟🌟🌟 **Fill:** Use `dynamic_cast` to safely downcast `animal` to `Dog*`.

```cpp,editable
#include <iostream>
#include <cassert>

class Animal {
public:
    virtual ~Animal() = default;
    virtual std::string speak() const = 0;
};

class Dog : public Animal {
public:
    std::string speak() const override { return "Woof"; }
    void fetch() const { std::cout << "Fetching!" << std::endl; }
};

class Cat : public Animal {
public:
    std::string speak() const override { return "Meow"; }
};

int main() {
    Dog d;
    Animal* animal = &d;

    Dog* dog_ptr = __;

    if (dog_ptr) {
        dog_ptr->fetch();
    }

    Cat* cat_ptr = dynamic_cast<Cat*>(animal);
    assert(cat_ptr == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
