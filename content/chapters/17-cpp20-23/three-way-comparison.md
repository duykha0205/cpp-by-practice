# Three-Way Comparison

The **three-way comparison operator** (`<=>`, nicknamed the "spaceship operator") is a C++20 feature that lets you define a single comparison and have the compiler **auto-generate** all six relational operators (`==`, `!=`, `<`, `>`, `<=`, `>=`). This drastically reduces boilerplate in classes that need ordering.

> **Note:** Requires GCC 10+ or Clang 10+ with `-std=c++20`. MSVC 19.26+ supports it with `/std:c++20`.

## Spaceship Operator

The `<=>` operator compares two values and returns an object that encodes whether the left operand is less than, equal to, or greater than the right operand. When you define `<=>` and `==` for a class (or default them), the compiler synthesizes all the other comparison operators automatically.

### How it works

```cpp
#include <iostream>
#include <compare>

struct Point {
    int x, y;

    // Defaulted <=> does member-wise comparison (x first, then y)
    auto operator<=>(const Point&) const = default;
};

int main() {
    Point a{1, 2}, b{1, 3}, c{1, 2};

    std::cout << std::boolalpha;
    std::cout << (a < b)  << std::endl;   // true  (same x, 2 < 3)
    std::cout << (a == c) << std::endl;   // true
    std::cout << (b > a)  << std::endl;   // true
    std::cout << (a != b) << std::endl;   // true
    std::cout << (a <= c) << std::endl;   // true
    std::cout << (a >= b) << std::endl;   // false
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `<=>` (spaceship) | Returns a comparison category object, not a `bool` |
| `= default` | Compiler generates member-wise comparison in declaration order |
| Auto-generated ops | `==`, `!=`, `<`, `>`, `<=`, `>=` are all synthesized from `<=>` |
| `#include <compare>` | Required header for comparison categories |

### Practice

1. 🌟🌟 **Fill:** Add a defaulted spaceship operator to `Student` so all six comparison operators work. Students are compared by `grade` first, then `name`.

```cpp,editable
#include <iostream>
#include <compare>
#include <string>
#include <cassert>

struct Student {
    int grade;
    std::string name;

    __;  // default the spaceship operator
};

int main() {
    Student alice{90, "Alice"};
    Student bob{85, "Bob"};
    Student charlie{90, "Charlie"};

    assert(bob < alice);         // 85 < 90
    assert(alice < charlie);     // same grade, "Alice" < "Charlie"
    assert(!(alice == charlie)); // different names

    Student alice2{90, "Alice"};
    assert(alice == alice2);     // identical

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Ordering Categories

The spaceship operator returns one of three **ordering category** types, depending on the semantics of the comparison. The category determines what kind of equivalence and ordering your type supports.

### How it works

```cpp
#include <iostream>
#include <compare>

// strong_ordering: equal values are truly indistinguishable
struct Id {
    int value;
    auto operator<=>(const Id&) const = default;
    // Returns std::strong_ordering
};

// weak_ordering: equivalent values may differ in some unobserved way
struct CaseInsensitiveChar {
    char c;
    std::weak_ordering operator<=>(const CaseInsensitiveChar& other) const {
        return std::tolower(c) <=> std::tolower(other.c);
    }
    bool operator==(const CaseInsensitiveChar& other) const {
        return std::tolower(c) == std::tolower(other.c);
    }
};

// partial_ordering: some values may be incomparable (like NaN)
struct MaybeValue {
    double val;
    auto operator<=>(const MaybeValue&) const = default;
    // double gives std::partial_ordering because of NaN
};

int main() {
    std::cout << std::boolalpha;

    Id a{1}, b{2};
    std::cout << (a < b) << std::endl;  // true

    CaseInsensitiveChar x{'A'}, y{'a'};
    std::cout << (x == y) << std::endl;  // true

    MaybeValue m{1.0}, n{0.0/0.0};  // n is NaN
    std::cout << (m < n) << std::endl;   // false
    std::cout << (m > n) << std::endl;   // false
    std::cout << (m == n) << std::endl;  // false (NaN is incomparable)
    return 0;
}
```

| Category | Meaning | Example types |
|----------|---------|---------------|
| `std::strong_ordering` | Equal means indistinguishable; total order | `int`, `char`, `std::string` |
| `std::weak_ordering` | Equivalent but distinguishable; total order | Case-insensitive strings |
| `std::partial_ordering` | Some values incomparable | `double` (NaN), partially ordered sets |
| `strong_ordering::less` | Left < Right | |
| `strong_ordering::equal` | Left == Right | |
| `strong_ordering::greater` | Left > Right | |
| `partial_ordering::unordered` | Neither less, equal, nor greater (e.g., NaN) | |

### Practice

2. 🌟🌟 **Fill:** Return the correct `std::strong_ordering` value by comparing `value` fields.

```cpp,editable
#include <iostream>
#include <compare>
#include <cassert>

struct Priority {
    int value;

    std::strong_ordering operator<=>(const Priority& other) const {
        return __;  // compare this->value with other.value
    }
    bool operator==(const Priority& other) const {
        return value == other.value;
    }
};

int main() {
    Priority low{1}, med{5}, high{10}, also_med{5};

    assert((low <=> high) == std::strong_ordering::less);
    assert((high <=> low) == std::strong_ordering::greater);
    assert((med <=> also_med) == std::strong_ordering::equal);
    assert(low < high);
    assert(high > med);
    assert(med == also_med);
    assert(med <= also_med);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Implement a `CaseInsensitiveString` that uses `std::weak_ordering` so that "Hello" and "hello" are equivalent but "apple" < "Banana".

```cpp,editable
#include <iostream>
#include <compare>
#include <string>
#include <algorithm>
#include <cassert>

struct CaseInsensitiveString {
    std::string str;

    std::weak_ordering operator<=>(const CaseInsensitiveString& other) const {
        __;  // compare str and other.str character by character, ignoring case
             // Hint: convert both to lowercase, then compare
    }

    bool operator==(const CaseInsensitiveString& other) const {
        __;  // return true if strings are equal ignoring case
    }
};

int main() {
    CaseInsensitiveString a{"Hello"}, b{"hello"}, c{"apple"}, d{"Banana"};

    assert(a == b);        // "Hello" == "hello" (case-insensitive)
    assert(!(a != b));
    assert(c < d);         // "apple" < "banana" (case-insensitive)
    assert(d > c);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Defaulted Comparisons

Using `= default` for `<=>` tells the compiler to compare members **in declaration order**. You can also customize by defaulting `<=>` and then providing a custom `==`, or by writing a custom `<=>` that compares only certain members.

### How it works

```cpp
#include <iostream>
#include <compare>
#include <string>

struct Employee {
    std::string name;
    int department;
    int employee_id;

    // Custom <=>: compare by department first, then by name
    // Ignore employee_id for ordering purposes
    auto operator<=>(const Employee& other) const {
        if (auto cmp = department <=> other.department; cmp != 0)
            return cmp;
        return name <=> other.name;
    }

    // Custom ==: two employees are equal only if all fields match
    bool operator==(const Employee& other) const {
        return name == other.name &&
               department == other.department &&
               employee_id == other.employee_id;
    }
};

int main() {
    Employee a{"Alice", 1, 1001};
    Employee b{"Bob", 1, 1002};
    Employee c{"Alice", 2, 1003};

    std::cout << std::boolalpha;
    std::cout << (a < b) << std::endl;   // true (same dept, "Alice" < "Bob")
    std::cout << (a < c) << std::endl;   // true (dept 1 < dept 2)
    std::cout << (a == a) << std::endl;  // true (all fields match)
    return 0;
}
```

| Pattern | When to use |
|---------|-------------|
| `auto operator<=>(const T&) const = default;` | Simple member-wise comparison in declaration order |
| Custom `<=>` + custom `==` | Different fields for ordering vs. equality |
| Custom `<=>` + defaulted `==` | Custom order, but member-wise equality |
| Compare subset of fields | Skip fields like IDs, caches, or metadata |

### Practice

4. 🌟🌟 **Fill:** Default the spaceship operator for `Color` so that it compares `r`, `g`, `b` in declaration order.

```cpp,editable
#include <iostream>
#include <compare>
#include <cassert>

struct Color {
    int r, g, b;

    __;  // default the spaceship operator
};

int main() {
    Color red{255, 0, 0};
    Color green{0, 255, 0};
    Color blue{0, 0, 255};
    Color also_red{255, 0, 0};

    assert(green < red);           // 0 < 255 (comparing r first)
    assert(blue < green);          // same r (0), 0 < 255 (comparing g)
    assert(red == also_red);
    assert(red > green);
    assert(blue <= green);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Write a custom `<=>` for `Version` that compares `major`, then `minor`, then `patch`. Provide a defaulted `==`.

```cpp,editable
#include <iostream>
#include <compare>
#include <cassert>

struct Version {
    int major;
    int minor;
    int patch;

    std::strong_ordering operator<=>(const Version& other) const {
        __;  // compare major, then minor, then patch
             // Hint: check each field; if it's not equal, return the result
    }

    bool operator==(const Version&) const = default;
};

int main() {
    Version v1{1, 0, 0};
    Version v2{1, 2, 0};
    Version v3{1, 2, 3};
    Version v4{2, 0, 0};
    Version v1_copy{1, 0, 0};

    assert(v1 < v2);       // minor: 0 < 2
    assert(v2 < v3);       // patch: 0 < 3
    assert(v3 < v4);       // major: 1 < 2
    assert(v1 == v1_copy);
    assert(v4 > v1);
    assert(v2 >= v1);
    assert(v1 <= v2);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
