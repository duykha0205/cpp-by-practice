# Inheritance

Inheritance allows a class to reuse and extend the functionality of another class. The derived class (child) inherits members from the base class (parent) and can add new members or override existing behavior. This is one of the core pillars of object-oriented programming.

## Basic Inheritance

### How it works

```cpp
#include <iostream>
#include <string>

class Animal {
protected:
    std::string name_;

public:
    Animal(const std::string& name) : name_(name) {}

    std::string getName() const { return name_; }
};

class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}

    std::string speak() const { return "Woof!"; }
};

int main() {
    Dog d("Rex");
    std::cout << d.getName() << " says " << d.speak() << std::endl;  // Rex says Woof!

    return 0;
}
```

- `: public Base` means `Dog` publicly inherits from `Animal`
- The derived class gets all public and protected members of the base
- Use `public` inheritance in most cases — it models an "is-a" relationship

### Practice

1. 🌟 Fill in the blank so that `Dog` inherits from `Animal`.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Animal {
protected:
    std::string name_;

public:
    Animal(const std::string& name) : name_(name) {}

    std::string getName() const { return name_; }
};

class Dog __ {
public:
    Dog(const std::string& name) : Animal(name) {}

    std::string speak() const { return "Woof!"; }
};

int main() {
    Dog d("Rex");
    assert(d.getName() == "Rex");
    assert(d.speak() == "Woof!");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Access Control in Inheritance

### How it works

```cpp
#include <iostream>

class Base {
private:
    int secret_ = 42;      // only accessible inside Base

protected:
    int shared_ = 10;      // accessible in Base and derived classes

public:
    int visible_ = 1;      // accessible everywhere
    int getSecret() const { return secret_; }
};

class Derived : public Base {
public:
    void show() const {
        // std::cout << secret_;   // ERROR: private members are inaccessible
        std::cout << "shared = " << shared_ << std::endl;                // OK: protected
        std::cout << "visible = " << visible_ << std::endl;              // OK: public
        std::cout << "secret via getter = " << getSecret() << std::endl; // OK: public method
    }
};

int main() {
    Derived d;
    d.show();

    return 0;
}
```

| Base member access | Accessible in Derived? | Accessible from outside? |
|--------------------|:---:|:---:|
| `private` | No | No |
| `protected` | Yes | No |
| `public` | Yes | Yes |

### Practice

2. 🌟🌟 Fix the error: the derived class tries to access a `private` base member directly. Remove the broken method or change the access level in `Base`.

```cpp,editable
#include <iostream>
#include <cassert>

class Base {
private:
    int secret_ = 42;
protected:
    int shared_ = 10;
public:
    int visible_ = 1;
};

class Derived : public Base {
public:
    int getSecret() const { return secret_; }    // ERROR: can't access private
    int getShared() const { return shared_; }     // OK
    int getVisible() const { return visible_; }   // OK
};

int main() {
    Derived d;
    assert(d.getShared() == 10);
    assert(d.getVisible() == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Constructor Chain

### How it works

```cpp
#include <iostream>
#include <string>

class Shape {
    std::string type_;

public:
    Shape(const std::string& type) : type_(type) {
        std::cout << "Shape(" << type_ << ") constructed" << std::endl;
    }

    std::string getType() const { return type_; }
};

class Circle : public Shape {
    double radius_;

public:
    Circle(double r) : Shape("Circle"), radius_(r) {
        std::cout << "Circle(r=" << radius_ << ") constructed" << std::endl;
    }

    double area() const { return 3.14159 * radius_ * radius_; }
};

int main() {
    Circle c(5.0);
    // Output:
    //   Shape(Circle) constructed
    //   Circle(r=5) constructed

    std::cout << c.getType() << " with area " << c.area() << std::endl;

    return 0;
}
```

- When constructing a derived object, the **base constructor runs first**
- If the base class has no default constructor, the derived class **must** explicitly call a base constructor in its initializer list
- Destruction happens in reverse: derived destructor first, then base destructor

### Practice

3. 🌟🌟 Fix the constructor: `Circle` must call `Shape("Circle")` in its initializer list.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Shape {
    std::string type_;

public:
    Shape(const std::string& type) : type_(type) {}
    std::string getType() const { return type_; }
};

class Circle : public Shape {
    double radius_;

public:
    // Fix: must call Shape constructor
    Circle(double r) : radius_(r) {}

    double area() const { return 3.14159 * radius_ * radius_; }
};

int main() {
    Circle c(5.0);
    assert(c.getType() == "Circle");
    assert(c.area() > 78.0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Method Overriding

### How it works

```cpp
#include <iostream>
#include <string>

class Vehicle {
public:
    virtual std::string describe() const {
        return "I am a vehicle";
    }
    virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
    std::string describe() const override {
        return "I am a car";
    }
};

class Bike : public Vehicle {
public:
    std::string describe() const override {
        return "I am a bike";
    }
};

int main() {
    Car c;
    Bike b;
    Vehicle* v = &c;  // base pointer holding derived object

    std::cout << c.describe() << std::endl;   // I am a car
    std::cout << b.describe() << std::endl;   // I am a bike
    std::cout << v->describe() << std::endl;  // I am a car (polymorphism!)

    return 0;
}
```

- Mark base class methods as `virtual` to allow overriding
- Use `override` in the derived class to indicate you are overriding a virtual method
- `override` catches signature mismatches at compile time — always use it
- Always declare a `virtual` destructor in base classes used polymorphically

### Practice

4. 🌟🌟 Fill in the blanks: override `describe()` in both `Car` and `Bike`.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Vehicle {
public:
    virtual std::string describe() const {
        return "I am a vehicle";
    }
    virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
    // Override describe to return "I am a car"
    __
};

class Bike : public Vehicle {
public:
    // Override describe to return "I am a bike"
    __
};

int main() {
    Car c;
    Bike b;
    assert(c.describe() == "I am a car");
    assert(b.describe() == "I am a bike");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
