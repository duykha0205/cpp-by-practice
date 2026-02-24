# Static Members & Friends

Static members belong to the class itself rather than to any particular instance, making them ideal for shared counters, configuration, and factory methods. Friend functions and classes break encapsulation in a controlled way, granting specific external code access to private members when the design requires it.

## Static Member Variables

### How it works

A `static` member variable is shared by all instances of the class. There is only one copy in memory, regardless of how many objects are created. It must be **defined** (allocated) outside the class, typically in a source file.

```cpp
#include <iostream>

class Connection {
    static int active_count_;

public:
    Connection() { active_count_++; }
    ~Connection() { active_count_--; }

    static int getActiveCount() { return active_count_; }
};

// Definition outside the class — required!
int Connection::active_count_ = 0;

int main() {
    std::cout << Connection::getActiveCount() << std::endl;  // 0

    Connection a;
    Connection b;
    std::cout << Connection::getActiveCount() << std::endl;  // 2

    {
        Connection c;
        std::cout << Connection::getActiveCount() << std::endl;  // 3
    }  // c destroyed

    std::cout << Connection::getActiveCount() << std::endl;  // 2
    return 0;
}
```

| Aspect | Details |
|---|---|
| Declaration | `static int count_;` inside the class |
| Definition | `int ClassName::count_ = 0;` outside the class |
| Access | `ClassName::count_` or `obj.count_` |
| Lifetime | Exists for the entire program, not tied to any object |
| Shared | All instances read/write the same variable |

### Practice

1\. 🌟🌟 **Complete:** add a static member variable and its definition to track the total number of `Product` objects ever created (including those already destroyed).

```cpp,editable
#include <iostream>
#include <cassert>

class Product {
    __               // Declare a static int total_created_

public:
    Product() { __; }    // Increment total_created_

    static int getTotalCreated() { return __; }
};

// Define the static member
__

int main() {
    assert(Product::getTotalCreated() == 0);

    Product a;
    Product b;
    assert(Product::getTotalCreated() == 2);

    {
        Product c;
        Product d;
        assert(Product::getTotalCreated() == 4);
    }
    // c and d are destroyed, but total_created_ still counts them
    assert(Product::getTotalCreated() == 4);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Static Member Functions

### How it works

A `static` member function does not have a `this` pointer, so it cannot access non-static members. It can only work with static member variables and its own parameters. Call it using `ClassName::function()`.

```cpp
#include <iostream>
#include <string>

class IdGenerator {
    static int next_id_;

public:
    // Static function — no this pointer, accesses only static members
    static int generate() {
        return next_id_++;
    }

    static void reset() {
        next_id_ = 1;
    }
};

int IdGenerator::next_id_ = 1;

int main() {
    std::cout << IdGenerator::generate() << std::endl;  // 1
    std::cout << IdGenerator::generate() << std::endl;  // 2
    std::cout << IdGenerator::generate() << std::endl;  // 3

    IdGenerator::reset();
    std::cout << IdGenerator::generate() << std::endl;  // 1
    return 0;
}
```

| Aspect | Details |
|---|---|
| No `this` pointer | Cannot access instance members |
| Access | Only static member variables and parameters |
| Calling | `ClassName::function()` — no object needed |
| Use cases | Factory methods, utility functions, managing shared state |

### Practice

2\. 🌟🌟 **Complete:** implement a `Config` class with static members to store and retrieve an application-wide setting. Fill in the static variable, its definition, and the two static functions.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Config {
    __                // Declare a static std::string app_name_

public:
    // Static setter
    __

    // Static getter
    __
};

// Define the static member with default value "Untitled"
__

int main() {
    assert(Config::getAppName() == "Untitled");

    Config::setAppName("MyApp");
    assert(Config::getAppName() == "MyApp");

    Config::setAppName("Production");
    assert(Config::getAppName() == "Production");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Friend Functions and Classes

### How it works

A `friend` declaration inside a class grants an external function or another class access to its private and protected members. Friendship is not inherited, not transitive, and must be explicitly granted.

```cpp
#include <iostream>

class Wallet {
    double balance_;

public:
    Wallet(double b) : balance_(b) {}

    // Grant a free function access to private members
    friend void transfer(Wallet& from, Wallet& to, double amount);

    // Grant another class access
    friend class Auditor;

    double getBalance() const { return balance_; }
};

void transfer(Wallet& from, Wallet& to, double amount) {
    from.balance_ -= amount;  // can access private member
    to.balance_ += amount;
}

class Auditor {
public:
    static void inspect(const Wallet& w) {
        // Can access private members because Auditor is a friend
        std::cout << "Balance: " << w.balance_ << std::endl;
    }
};

int main() {
    Wallet alice(100.0);
    Wallet bob(50.0);

    transfer(alice, bob, 30.0);
    std::cout << "Alice: " << alice.getBalance() << std::endl;  // 70
    std::cout << "Bob: " << bob.getBalance() << std::endl;      // 80

    Auditor::inspect(alice);  // Balance: 70
    return 0;
}
```

| Aspect | Details |
|---|---|
| `friend` function | A non-member function granted access to private members |
| `friend` class | An entire class granted access to private members |
| Declared where? | Inside the class granting access |
| Symmetry | Friendship is **not** mutual unless both declare it |
| Transitivity | Friendship is **not** transitive (friend of a friend is not a friend) |

### Practice

3\. 🌟🌟🌟 **Complete:** implement a `friend` function `distance` that computes the Euclidean distance between two `Point` objects by accessing their private coordinates.

```cpp,editable
#include <iostream>
#include <cmath>
#include <cassert>

class Point {
    double x_, y_;

public:
    Point(double x, double y) : x_(x), y_(y) {}

    // Declare distance as a friend function
    __
};

// Implement the friend function: returns sqrt((x1-x2)^2 + (y1-y2)^2)
__

int main() {
    Point a(0.0, 0.0);
    Point b(3.0, 4.0);

    double d = distance(a, b);
    assert(d > 4.99 && d < 5.01);  // distance should be 5.0

    Point c(1.0, 1.0);
    Point e(1.0, 1.0);
    double d2 = distance(c, e);
    assert(d2 < 0.01);  // same point, distance is 0

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4\. 🌟🌟🌟 **Complete:** declare `Serializer` as a friend class of `SecureData` so it can access the private fields for serialization. Fill in the friend declaration and the serialize method.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

class Serializer;  // Forward declaration

class SecureData {
    std::string username_;
    int access_level_;

public:
    SecureData(const std::string& user, int level)
        : username_(user), access_level_(level) {}

    // Declare Serializer as a friend class
    __
};

class Serializer {
public:
    // Access private members of SecureData to create a formatted string
    // Format: "user:level" (e.g., "admin:5")
    static std::string serialize(const SecureData& data) {
        return __ + ":" + std::to_string(__);
    }
};

int main() {
    SecureData admin("admin", 5);
    SecureData guest("guest", 1);

    assert(Serializer::serialize(admin) == "admin:5");
    assert(Serializer::serialize(guest) == "guest:1");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
