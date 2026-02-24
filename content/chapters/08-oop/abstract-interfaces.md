# Abstract Classes & Interfaces

Abstract classes and interfaces define contracts that derived classes must fulfill. A **pure interface** contains only pure virtual functions and no data, while an abstract class may provide partial implementations. C++ supports **multiple inheritance**, which enables powerful interface composition but introduces challenges like the **diamond problem**.

## Pure Interfaces

A pure interface is a class that contains **only** pure virtual functions and no data members. It defines a contract — any class that inherits from it must implement every function. This is the C++ equivalent of Java's `interface` keyword.

### How it works

```cpp
#include <iostream>
#include <string>

class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    virtual ~ISerializable() = default;
};

class Config : public ISerializable {
    std::string name_;
    int value_;
public:
    Config(const std::string& n, int v) : name_(n), value_(v) {}

    std::string serialize() const override {
        return name_ + "=" + std::to_string(value_);
    }
    void deserialize(const std::string& data) override {
        auto pos = data.find('=');
        name_ = data.substr(0, pos);
        value_ = std::stoi(data.substr(pos + 1));
    }

    std::string name() const { return name_; }
    int value() const { return value_; }
};

int main() {
    Config cfg("timeout", 30);
    ISerializable* ptr = &cfg;
    std::cout << ptr->serialize() << std::endl;  // timeout=30
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Pure interface | Class with only pure virtual functions (`= 0`) and no data members |
| Contract | All inheriting classes **must** implement every pure virtual function |
| Virtual destructor | Always provide `virtual ~Interface() = default` for safe polymorphic deletion |
| Naming convention | Common to prefix interface names with `I` (e.g., `ISerializable`) |

### Practice

1. 🌟🌟 **Fill:** Complete the `IShape` pure interface and implement `area()` and `name()` in the `Circle` class.

```cpp,editable
#include <iostream>
#include <string>
#include <cmath>
#include <cassert>

class IShape {
public:
    virtual double area() const = 0;
    virtual std::string name() const = 0;
    virtual ~IShape() = default;
};

class Circle : public IShape {
    double radius_;
public:
    Circle(double r) : radius_(r) {}
    __ // implement area(): return M_PI * radius_ * radius_
    __ // implement name(): return "Circle"
};

int main() {
    Circle c(2.0);
    IShape* shape = &c;

    assert(std::abs(shape->area() - 4.0 * M_PI) < 0.001);
    assert(shape->name() == "Circle");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Fill:** Create a `Printer` class that implements the `IPrintable` interface by returning a formatted string from `to_string()`.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class IPrintable {
public:
    virtual std::string to_string() const = 0;
    virtual ~IPrintable() = default;
};

class Printer : public IPrintable {
    std::string model_;
    int pages_per_min_;
public:
    Printer(const std::string& model, int ppm)
        : model_(model), pages_per_min_(ppm) {}
    __ // implement to_string(): return model_ + " (" + std::to_string(pages_per_min_) + " ppm)"
};

int main() {
    Printer p("LaserJet", 30);
    IPrintable* ptr = &p;

    assert(ptr->to_string() == "LaserJet (30 ppm)");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Interface Composition

C++ allows a class to inherit from **multiple** interfaces. This is the safe form of multiple inheritance — since pure interfaces carry no data, there is no ambiguity about which base member to use.

### How it works

```cpp
#include <iostream>
#include <string>

class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class IResizable {
public:
    virtual void resize(double factor) = 0;
    virtual ~IResizable() = default;
};

class Widget : public IDrawable, public IResizable {
    double size_ = 1.0;
public:
    void draw() const override {
        std::cout << "Drawing widget (size " << size_ << ")" << std::endl;
    }
    void resize(double factor) override {
        size_ *= factor;
    }
    double size() const { return size_; }
};

int main() {
    Widget w;
    IDrawable* d = &w;
    IResizable* r = &w;

    d->draw();      // Drawing widget (size 1)
    r->resize(2.0);
    d->draw();      // Drawing widget (size 2)
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Multiple inheritance | A class can inherit from several base classes with `: public A, public B` |
| Interface composition | Combining multiple pure interfaces is safe — no data conflicts |
| Polymorphic access | A single object can be accessed through different interface pointers |
| All must be implemented | The concrete class must implement every pure virtual from all interfaces |

### Practice

3. 🌟🌟🌟 **Complete:** Implement the `Robot` class that inherits from both `IMovable` and `ISpeakable`, tracking position and returning a greeting.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class IMovable {
public:
    virtual void move(int dx, int dy) = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual ~IMovable() = default;
};

class ISpeakable {
public:
    virtual std::string speak() const = 0;
    virtual ~ISpeakable() = default;
};

class Robot : public IMovable, public ISpeakable {
    int x_ = 0, y_ = 0;
    std::string name_;
public:
    Robot(const std::string& name) : name_(name) {}
    __ // implement move(dx, dy): add dx to x_, add dy to y_
    __ // implement x(): return x_
    __ // implement y(): return y_
    __ // implement speak(): return "Hi, I am " + name_
};

int main() {
    Robot r("Robo");
    IMovable* mover = &r;
    ISpeakable* talker = &r;

    assert(talker->speak() == "Hi, I am Robo");
    assert(mover->x() == 0 && mover->y() == 0);

    mover->move(3, 4);
    assert(mover->x() == 3);
    assert(mover->y() == 4);

    mover->move(-1, 2);
    assert(mover->x() == 2);
    assert(mover->y() == 6);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Diamond Problem & Virtual Inheritance

When two classes inherit from the same base, and a fourth class inherits from both, the base class appears **twice** in memory. This is the **diamond problem**. The fix is `virtual` inheritance, which ensures only one shared copy of the base.

### How it works

```cpp
#include <iostream>
#include <string>

class Animal {
public:
    std::string name;
    Animal(const std::string& n) : name(n) {}
    virtual ~Animal() = default;
};

// virtual inheritance: only one Animal sub-object in the final class
class Mammal : virtual public Animal {
public:
    Mammal(const std::string& n) : Animal(n) {}
};

class WingedAnimal : virtual public Animal {
public:
    WingedAnimal(const std::string& n) : Animal(n) {}
};

class Bat : public Mammal, public WingedAnimal {
public:
    // The most-derived class must initialise the virtual base directly
    Bat(const std::string& n)
        : Animal(n), Mammal(n), WingedAnimal(n) {}
};

int main() {
    Bat b("Batty");
    // Only one copy of Animal::name — no ambiguity
    std::cout << b.name << std::endl;  // Batty
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Diamond problem | Base class duplicated when two intermediate classes inherit from it |
| `virtual public Base` | Ensures only one shared copy of `Base` in the most-derived class |
| Most-derived initialises | The final class must call the virtual base constructor directly |
| Without `virtual` | Accessing `name` is ambiguous — compiler error |

### Practice

4. 🌟🌟🌟 **Fix:** The code has a diamond problem — `Amphibian` inherits two copies of `Animal`. Add `virtual` to the inheritance of `LandAnimal` and `WaterAnimal` to fix the ambiguity.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Animal {
public:
    std::string species;
    Animal(const std::string& s) : species(s) {}
    virtual ~Animal() = default;
};

class LandAnimal : __ Animal {  // FIX: add virtual
public:
    LandAnimal(const std::string& s) : Animal(s) {}
    std::string terrain() const { return "land"; }
};

class WaterAnimal : __ Animal {  // FIX: add virtual
public:
    WaterAnimal(const std::string& s) : Animal(s) {}
    std::string terrain() const { return "water"; }
};

class Amphibian : public LandAnimal, public WaterAnimal {
public:
    Amphibian(const std::string& s)
        : Animal(s), LandAnimal(s), WaterAnimal(s) {}
};

int main() {
    Amphibian frog("Frog");

    assert(frog.species == "Frog");
    assert(frog.LandAnimal::terrain() == "land");
    assert(frog.WaterAnimal::terrain() == "water");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Build a diamond hierarchy with virtual inheritance where `SmartPhone` inherits from both `Phone` and `Camera`, which both virtually inherit from `Device`. The most-derived class must initialise `Device` directly.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Device {
public:
    std::string brand;
    Device(const std::string& b) : brand(b) {}
    virtual ~Device() = default;
};

class Phone : virtual public Device {
public:
    Phone(const std::string& b) : Device(b) {}
    std::string call(const std::string& number) const {
        return "Calling " + number;
    }
};

class Camera : virtual public Device {
public:
    Camera(const std::string& b) : Device(b) {}
    std::string snap() const {
        return "Photo taken";
    }
};

class SmartPhone : __ {  // inherit from Phone and Camera
public:
    __ // constructor: initialise Device, Phone, and Camera with brand
};

int main() {
    SmartPhone sp("Pixel");

    assert(sp.brand == "Pixel");
    assert(sp.call("12345") == "Calling 12345");
    assert(sp.snap() == "Photo taken");

    Device* dev = &sp;
    assert(dev->brand == "Pixel");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
