# Constructors & Destructors

Constructors and destructors are special member functions that control how objects are created and cleaned up. Constructors initialize an object's state, while destructors release resources when the object goes out of scope — a pattern known as RAII (Resource Acquisition Is Initialization).

## Constructor Basics

### How it works

```cpp
#include <iostream>

class Rectangle {
    double width_;
    double height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double area() const { return width_ * height_; }

    void print() const {
        std::cout << width_ << " x " << height_ << " = " << area() << std::endl;
    }
};

int main() {
    Rectangle r(5.0, 3.0);
    r.print();  // 5 x 3 = 15

    return 0;
}
```

- A **constructor** is called automatically when an object is created
- It has the same name as the class and no return type
- Use it to initialize member variables to a valid state

### Practice

1. 🌟 Add a constructor that takes `width` and `height` parameters.

```cpp,editable
#include <iostream>
#include <cassert>

class Rectangle {
    double width_;
    double height_;

public:
    // Add constructor here
    __

    double area() const { return width_ * height_; }
};

int main() {
    Rectangle r(5.0, 3.0);
    assert(r.area() == 15.0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Member Initializer List

### How it works

```cpp
#include <iostream>
#include <string>

class Student {
    const std::string name_;   // const — must be initialized, not assigned
    const int id_;

public:
    // Member initializer list: initializes members before the body runs
    Student(const std::string& name, int id) : name_(name), id_(id) {
        std::cout << "Created student: " << name_ << " (#" << id_ << ")" << std::endl;
    }

    std::string getName() const { return name_; }
    int getId() const { return id_; }
};

int main() {
    Student s("Alice", 1001);
    std::cout << s.getName() << ", ID: " << s.getId() << std::endl;  // Alice, ID: 1001

    return 0;
}
```

- The **initializer list** appears between `:` and `{` in the constructor
- `const` and reference members **must** be initialized in the initializer list — they cannot be assigned in the body
- Initializer lists are more efficient than assignment in the body (avoids default construction followed by reassignment)

### Practice

2. 🌟🌟 Fix the error: `const` members cannot be assigned in the constructor body. Use an initializer list instead.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Student {
    const std::string name_;
    const int id_;

public:
    Student(const std::string& name, int id) {
        name_ = name;  // ERROR: can't assign to const
        id_ = id;      // ERROR: can't assign to const
    }

    std::string getName() const { return name_; }
    int getId() const { return id_; }
};

int main() {
    Student s("Alice", 1001);
    assert(s.getName() == "Alice");
    assert(s.getId() == 1001);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Destructors

### How it works

```cpp
#include <iostream>
#include <string>

class Logger {
    std::string name_;

public:
    Logger(const std::string& name) : name_(name) {
        std::cout << name_ << " created" << std::endl;
    }

    ~Logger() {
        std::cout << name_ << " destroyed" << std::endl;
    }
};

int main() {
    Logger a("A");           // A created
    {
        Logger b("B");       // B created
        Logger c("C");       // C created
    }                        // C destroyed, then B destroyed (reverse order!)

    std::cout << "End of main" << std::endl;
    return 0;
}                            // A destroyed
```

- The **destructor** (`~ClassName()`) is called automatically when an object goes out of scope
- Objects are destroyed in **reverse order** of their construction (LIFO — last in, first out)
- RAII: acquire resources in the constructor, release them in the destructor

### Practice

3. 🌟🌟 Fill in the expected destruction order. Destructors run in reverse order of construction.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

std::string destruction_order;

class Logger {
    std::string name_;

public:
    Logger(const std::string& name) : name_(name) {
        std::cout << name_ << " created" << std::endl;
    }

    ~Logger() {
        destruction_order += name_;
        std::cout << name_ << " destroyed" << std::endl;
    }
};

int main() {
    Logger a("A");
    {
        Logger b("B");
        Logger c("C");
    }
    // B and C have been destroyed. What order?
    // Hint: destructors run in reverse order of construction.
    assert(destruction_order == "__");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Copy Constructor

### How it works

```cpp
#include <iostream>
#include <cstring>

class MyString {
    char* data_;
    size_t len_;

public:
    MyString(const char* str) {
        len_ = std::strlen(str);
        data_ = new char[len_ + 1];
        std::strcpy(data_, str);
    }

    // Deep copy constructor — allocates new memory and copies content
    MyString(const MyString& other) {
        len_ = other.len_;
        data_ = new char[len_ + 1];
        std::strcpy(data_, other.data_);
    }

    ~MyString() {
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    size_t length() const { return len_; }
};

int main() {
    MyString a("Hello");
    MyString b = a;    // calls copy constructor

    std::cout << "a: " << a.c_str() << std::endl;  // Hello
    std::cout << "b: " << b.c_str() << std::endl;  // Hello
    std::cout << "Same memory? " << (a.c_str() == b.c_str() ? "yes" : "no") << std::endl;  // no

    return 0;
}
```

- Without a custom copy constructor, C++ performs a **shallow copy** (copies pointer values, not the data they point to)
- Shallow copy leads to **double free**: two objects try to `delete[]` the same memory
- The **Rule of Three**: if you define a destructor, copy constructor, or copy assignment operator, you should define all three

### Practice

4. 🌟🌟🌟 Add a proper copy constructor that performs a **deep copy** — allocating new memory and copying the character data.

```cpp,editable
#include <iostream>
#include <cassert>
#include <cstring>

class MyString {
    char* data_;
    size_t len_;

public:
    MyString(const char* str) {
        len_ = std::strlen(str);
        data_ = new char[len_ + 1];
        std::strcpy(data_, str);
    }

    // Add a deep copy constructor here
    __

    ~MyString() {
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    size_t length() const { return len_; }
};

int main() {
    MyString a("Hello");
    MyString b = a;  // Copy!

    assert(std::strcmp(a.c_str(), b.c_str()) == 0);
    assert(a.c_str() != b.c_str());  // Different memory!

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
