# Enums, Type Aliases & Typedef

Enumerations give meaningful names to sets of related integer constants, and type aliases let you create shorter or more descriptive names for existing types. Modern C++ introduces **scoped enums** (`enum class`) for type safety and the `using` keyword as a cleaner alternative to `typedef`.

## C-style Enums

### How it works

A traditional (unscoped) `enum` defines a set of named integer constants. The enumerators are implicitly convertible to `int` and are injected into the enclosing scope, which can cause name collisions.

```cpp
#include <iostream>
#include <cassert>

enum Direction {
    North,   // 0
    East,    // 1
    South,   // 2
    West     // 3
};

enum Status {
    OK = 200,
    NotFound = 404,
    ServerError = 500
};

int main() {
    Direction d = North;
    int value = d;  // implicit conversion to int — allowed

    assert(value == 0);
    assert(East == 1);
    assert(South == 2);

    Status s = NotFound;
    assert(s == 404);  // compares as int

    std::cout << "Direction: " << d << " Status: " << s << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | Detail |
|---------|--------|
| Default values | Start at 0 and increment by 1 |
| Custom values | Assign with `= N`; subsequent values increment from there |
| Implicit int conversion | Enumerators convert to `int` without a cast |
| Scope | Names are injected into the enclosing scope (can collide) |

### Practice

1\. 🌟 **Fill:** Assign custom values to the HTTP status codes so the assertions pass.

```cpp,editable
#include <iostream>
#include <cassert>

enum HttpStatus {
    OK = __,
    Created = __,
    BadRequest = __,
    NotFound = __
};

int main() {
    assert(OK == 200);
    assert(Created == 201);
    assert(BadRequest == 400);
    assert(NotFound == 404);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2\. 🌟 **Fill:** Complete the enum and use it to set the variable.

```cpp,editable
#include <iostream>
#include <cassert>

enum Season {
    Spring,  // 0
    Summer,  // 1
    __,      // 2
    __       // 3
};

int main() {
    Season current = Winter;
    int val = current;

    assert(val == 3);
    assert(Summer == 1);

    std::cout << "Current season value: " << val << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Scoped Enums (enum class)

### How it works

C++11 `enum class` (scoped enum) fixes the problems of C-style enums: the enumerators are scoped under the enum name and do **not** implicitly convert to `int`. You can also specify an underlying type.

```cpp
#include <iostream>
#include <cassert>

enum class Color : int {
    Red = 1,
    Green = 2,
    Blue = 3
};

enum class Permission : unsigned {
    Read    = 1,
    Write   = 2,
    Execute = 4
};

int main() {
    Color c = Color::Blue;

    // int x = c;  // ERROR: no implicit conversion
    int x = static_cast<int>(c);  // explicit cast required
    assert(x == 3);

    // Scoped: no name collision even if another enum has "Red"
    Permission p = Permission::Read;
    assert(static_cast<unsigned>(p) == 1);

    std::cout << "Color value: " << x << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | `enum` | `enum class` |
|---------|--------|--------------|
| Scope | Enclosing (pollutes namespace) | Scoped (`Color::Red`) |
| Implicit int conversion | Yes | No (need `static_cast`) |
| Underlying type | Implementation-defined | Can be specified (`: int`, `: char`, etc.) |
| Type safety | Weak | Strong |

### Practice

3\. 🌟🌟 **Fix:** The code uses unscoped access and implicit conversion. Change the enum to `enum class` and fix the usage.

```cpp,editable
#include <iostream>
#include <cassert>

// Change this to enum class
enum TrafficLight {
    Red,
    Yellow,
    Green
};

int main() {
    // Fix: use scoped access (TrafficLight::Green)
    TrafficLight light = Green;

    // Fix: enum class requires static_cast to convert to int
    int value = light;

    assert(value == 2);

    std::cout << "Light value: " << value << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Type Aliases

### How it works

`typedef` and `using` both create alternative names for types. The modern `using` syntax is more readable, especially with templates — `using` supports **alias templates** while `typedef` does not.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// typedef syntax
typedef unsigned int uint;
typedef std::vector<std::string> StringList;

// using syntax (preferred in modern C++)
using Score = int;
using Names = std::vector<std::string>;

// Alias template — only possible with 'using'
template <typename T>
using Vec = std::vector<T>;

int main() {
    uint age = 25;
    Score points = 100;

    StringList old_names = {"Alice", "Bob"};
    Names new_names = {"Charlie", "Dave"};

    Vec<int> numbers = {1, 2, 3};
    Vec<double> values = {1.1, 2.2};

    assert(age == 25);
    assert(points == 100);
    assert(old_names.size() == 2);
    assert(numbers.size() == 3);

    std::cout << "age=" << age << " points=" << points << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

| Feature | `typedef` | `using` |
|---------|-----------|---------|
| Syntax | `typedef old_type new_name;` | `using new_name = old_type;` |
| Readability | Type comes first (reversed) | Name comes first (natural) |
| Alias templates | Not supported | `template<typename T> using Vec = ...;` |
| Recommendation | Legacy code | Preferred in modern C++ |

### Practice

4\. 🌟🌟 **Fill:** Create type aliases using the `using` keyword.

```cpp,editable
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Create a type alias 'Scores' for std::vector<int> using 'using'
__

// Create a type alias 'Name' for std::string using 'using'
__

int main() {
    Scores s = {90, 85, 92};
    Name n = "Alice";

    assert(s.size() == 3);
    assert(s[0] == 90);
    assert(n == "Alice");

    std::cout << n << " has " << s.size() << " scores" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

5\. 🌟🌟 **Complete:** Create an alias template `Vec<T>` and use it to declare vectors of different types.

```cpp,editable
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

// Create an alias template: Vec<T> should be std::vector<T>
template <typename T>
__

int main() {
    Vec<int> numbers = {1, 2, 3, 4, 5};
    Vec<std::string> words = {"hello", "world"};

    assert(numbers.size() == 5);
    assert(numbers[0] == 1);
    assert(words.size() == 2);
    assert(words[1] == "world");

    std::cout << "numbers size: " << numbers.size() << std::endl;
    std::cout << "words size: " << words.size() << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
