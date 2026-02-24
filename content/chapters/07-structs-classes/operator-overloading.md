# Operator Overloading

Operator overloading lets you define custom behavior for built-in operators (`+`, `-`, `==`, `<<`, `[]`, etc.) when they are applied to your own types. This makes user-defined types as natural to use as built-in types — you can add two `Vector2D` objects with `+`, compare `Date` objects with `==`, or print a `Matrix` with `<<`.

## Arithmetic Operators

### How it works

Arithmetic operators like `+` and `-` are typically overloaded as member functions or free functions that return a new object **by value**. The original operands are not modified.

```cpp
#include <iostream>

class Vector2D {
    double x_, y_;

public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    // Overload + as a member function
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x_ + other.x_, y_ + other.y_);
    }

    // Overload - as a member function
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x_ - other.x_, y_ - other.y_);
    }

    double getX() const { return x_; }
    double getY() const { return y_; }
};

int main() {
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);

    Vector2D sum = a + b;   // calls a.operator+(b)
    Vector2D diff = a - b;  // calls a.operator-(b)

    std::cout << "sum: (" << sum.getX() << ", " << sum.getY() << ")" << std::endl;   // (4, 6)
    std::cout << "diff: (" << diff.getX() << ", " << diff.getY() << ")" << std::endl; // (-2, -2)
    return 0;
}
```

| Aspect | Details |
|---|---|
| Signature | `Type operator+(const Type& other) const` |
| Return | By value (a new object) |
| `const` qualifier | The left operand is not modified |
| Parameter | The right operand is passed by `const` reference |

### Practice

1\. 🌟 **Complete:** overload the `+` operator for the `Money` class so that amounts can be added together.

```cpp,editable
#include <iostream>
#include <cassert>

class Money {
    int cents_;

public:
    Money(int cents) : cents_(cents) {}

    // Overload + to add two Money objects
    __

    int getCents() const { return cents_; }
};

int main() {
    Money a(150);   // $1.50
    Money b(275);   // $2.75
    Money c = a + b;

    assert(c.getCents() == 425);

    Money d = c + Money(75);
    assert(d.getCents() == 500);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Comparison Operators

### How it works

Comparison operators (`==`, `!=`, `<`, `>`, etc.) return `bool`. They can be implemented as member functions or as `friend` free functions. Using `friend` allows the function to access private members while remaining a non-member.

```cpp
#include <iostream>

class Date {
    int year_, month_, day_;

public:
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {}

    // Overload == as a member function
    bool operator==(const Date& other) const {
        return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
    }

    // Overload < as a friend function
    friend bool operator<(const Date& lhs, const Date& rhs) {
        if (lhs.year_ != rhs.year_) return lhs.year_ < rhs.year_;
        if (lhs.month_ != rhs.month_) return lhs.month_ < rhs.month_;
        return lhs.day_ < rhs.day_;
    }
};

int main() {
    Date d1(2024, 1, 15);
    Date d2(2024, 1, 15);
    Date d3(2024, 6, 1);

    std::cout << (d1 == d2) << std::endl;  // 1 (true)
    std::cout << (d1 < d3) << std::endl;   // 1 (true)
    std::cout << (d3 < d1) << std::endl;   // 0 (false)
    return 0;
}
```

| Approach | Syntax | Access to private members |
|---|---|---|
| Member function | `bool operator==(const Type& other) const` | Yes (implicit `this`) |
| Friend function | `friend bool operator<(const Type& a, const Type& b)` | Yes (explicitly granted) |
| Free function | `bool operator<(const Type& a, const Type& b)` | Only public members |

### Practice

2\. 🌟🌟 **Complete:** overload `==` and `<` for the `Fraction` class. Two fractions are equal when their cross products match. Fraction `a/b < c/d` when `a*d < c*b` (assuming positive denominators).

```cpp,editable
#include <iostream>
#include <cassert>

class Fraction {
    int num_, den_;

public:
    Fraction(int n, int d) : num_(n), den_(d) {}

    // Overload == : two fractions a/b and c/d are equal if a*d == c*b
    __

    // Overload < as a friend: a/b < c/d if a*d < c*b
    __

    int getNum() const { return num_; }
    int getDen() const { return den_; }
};

int main() {
    Fraction a(1, 2);
    Fraction b(2, 4);  // same as 1/2
    Fraction c(3, 4);

    assert(a == b);
    assert(!(a == c));
    assert(a < c);
    assert(!(c < a));

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Stream Output Operator

### How it works

The `<<` operator for `std::ostream` must be a free function (not a member of your class) because the left operand is `std::ostream`, not your type. Declare it as a `friend` inside the class to grant access to private members.

```cpp
#include <iostream>
#include <string>

class Point {
    double x_, y_;

public:
    Point(double x, double y) : x_(x), y_(y) {}

    // Declared as friend inside the class
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x_ << ", " << p.y_ << ")";
        return os;
    }
};

int main() {
    Point p(3.0, 4.0);
    std::cout << "Point: " << p << std::endl;  // Point: (3, 4)
    return 0;
}
```

| Aspect | Details |
|---|---|
| Signature | `friend std::ostream& operator<<(std::ostream& os, const Type& obj)` |
| Left operand | `std::ostream&` (cout, file stream, etc.) |
| Right operand | Your type by `const` reference |
| Return | `std::ostream&` to allow chaining (`cout << a << b`) |

### Practice

3\. 🌟🌟 **Complete:** overload `operator<<` so `Color` objects print as `"rgb(R, G, B)"`.

```cpp,editable
#include <iostream>
#include <sstream>
#include <cassert>

class Color {
    int r_, g_, b_;

public:
    Color(int r, int g, int b) : r_(r), g_(g), b_(b) {}

    // Overload << to print as "rgb(R, G, B)"
    __
};

int main() {
    Color red(255, 0, 0);
    Color white(255, 255, 255);

    // Use a stringstream to capture the output
    std::ostringstream oss1;
    oss1 << red;
    assert(oss1.str() == "rgb(255, 0, 0)");

    std::ostringstream oss2;
    oss2 << white;
    assert(oss2.str() == "rgb(255, 255, 255)");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Subscript Operator

### How it works

The subscript operator `[]` lets your class behave like an array or container. You should provide both a `const` and a non-`const` version: the non-const version allows modification, while the const version is used on const objects.

```cpp
#include <iostream>
#include <cassert>

class IntArray {
    int data_[5];
    int size_;

public:
    IntArray() : data_{0, 0, 0, 0, 0}, size_(5) {}

    // Non-const version: allows modification
    int& operator[](int index) {
        return data_[index];
    }

    // Const version: read-only access
    const int& operator[](int index) const {
        return data_[index];
    }

    int size() const { return size_; }
};

int main() {
    IntArray arr;
    arr[0] = 10;
    arr[1] = 20;

    std::cout << arr[0] << std::endl;  // 10
    std::cout << arr[1] << std::endl;  // 20

    const IntArray& ref = arr;
    std::cout << ref[0] << std::endl;  // 10 (uses const version)
    // ref[0] = 99;                    // ERROR: const version returns const ref

    return 0;
}
```

| Version | Signature | When called |
|---|---|---|
| Non-const | `T& operator[](int index)` | On non-const objects; allows read/write |
| Const | `const T& operator[](int index) const` | On const objects; read-only |

### Practice

4\. 🌟🌟🌟 **Complete:** add both const and non-const `operator[]` to the `SafeArray` class. The operator should check bounds and return a reference to a static "error" element for out-of-range access.

```cpp,editable
#include <iostream>
#include <cassert>

class SafeArray {
    int data_[4];
    int size_;
    static int error_element_;

public:
    SafeArray() : data_{0, 0, 0, 0}, size_(4) {}

    // Non-const operator[]: return reference to data_[index],
    // or reference to error_element_ if index is out of range
    __

    // Const operator[]: return const reference to data_[index],
    // or const reference to error_element_ if index is out of range
    __

    int size() const { return size_; }
};

int SafeArray::error_element_ = -1;

int main() {
    SafeArray arr;
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;

    assert(arr[0] == 10);
    assert(arr[3] == 40);
    assert(arr[99] == -1);    // out of bounds returns error element

    const SafeArray& cref = arr;
    assert(cref[1] == 20);
    assert(cref[100] == -1);  // const version also checks bounds

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟🌟 **Complete:** overload `+`, `==`, and `<<` for a `Matrix2x2` class. Addition is element-wise, equality checks all four elements, and `<<` prints in the format `"[a b; c d]"`.

```cpp,editable
#include <iostream>
#include <sstream>
#include <cassert>

class Matrix2x2 {
    int data_[2][2];

public:
    Matrix2x2(int a, int b, int c, int d) {
        data_[0][0] = a; data_[0][1] = b;
        data_[1][0] = c; data_[1][1] = d;
    }

    // Overload + for element-wise addition
    __

    // Overload == to compare all four elements
    __

    // Overload << to print as "[a b; c d]"
    __
};

int main() {
    Matrix2x2 a(1, 2, 3, 4);
    Matrix2x2 b(5, 6, 7, 8);

    Matrix2x2 c = a + b;
    Matrix2x2 expected(6, 8, 10, 12);
    assert(c == expected);

    Matrix2x2 d(1, 2, 3, 4);
    assert(a == d);
    assert(!(a == b));

    std::ostringstream oss;
    oss << a;
    assert(oss.str() == "[1 2; 3 4]");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
