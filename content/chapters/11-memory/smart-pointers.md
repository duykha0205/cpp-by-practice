# Smart Pointers

Smart pointers manage heap memory automatically using RAII — the memory is freed when the smart pointer goes out of scope. They eliminate most manual `new`/`delete` bugs: leaks, dangling pointers, and double frees.

## unique_ptr

`std::unique_ptr` has **exclusive ownership**: only one `unique_ptr` can own a given object at a time. It cannot be copied, only **moved**. When it goes out of scope (or is reset), the object is deleted.

### How it works

```cpp
#include <iostream>
#include <memory>

int main() {
    auto p1 = std::make_unique<int>(42);
    std::cout << *p1 << std::endl;  // 42

    // auto p2 = p1;             // ERROR: can't copy unique_ptr
    auto p2 = std::move(p1);     // OK: transfer ownership
    std::cout << *p2 << std::endl;  // 42
    // p1 is now nullptr

    // Memory freed automatically when p2 goes out of scope
    return 0;
}
```

| Operation | Effect |
|-----------|--------|
| `std::make_unique<T>(args)` | Allocates and constructs a `T`, returns `unique_ptr<T>` |
| `auto p2 = std::move(p1)` | Transfers ownership; `p1` becomes `nullptr` |
| Copy (`auto p2 = p1`) | Compile error — exclusive ownership |
| Scope exit | Automatically calls `delete` |

### Practice

1. 🌟🌟 **Fix:** You can't copy a `unique_ptr`. Use `std::move` to transfer ownership.

```cpp,editable
#include <iostream>
#include <memory>
#include <cassert>

int main() {
    auto p1 = std::make_unique<int>(42);

    // BUG: can't copy unique_ptr
    auto p2 = p1;

    assert(*p2 == 42);
    assert(p1 == nullptr);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## unique_ptr with Classes

`unique_ptr` is ideal for owning polymorphic objects. Combined with `make_unique`, it provides exception-safe, leak-free object creation.

### How it works

```cpp
#include <iostream>
#include <memory>
#include <string>

class Widget {
    std::string name_;
public:
    Widget(std::string name) : name_(std::move(name)) {
        std::cout << "Widget " << name_ << " created\n";
    }
    ~Widget() { std::cout << "Widget " << name_ << " destroyed\n"; }
    const std::string& name() const { return name_; }
};

int main() {
    auto w = std::make_unique<Widget>("Alpha");
    std::cout << "Name: " << w->name() << std::endl;
    // Widget automatically destroyed at end of scope
    return 0;
}
```

| Feature | Detail |
|---------|--------|
| `make_unique<T>(args)` | Forwards `args` to `T`'s constructor |
| `w->member()` | Access members through the smart pointer |
| `w.get()` | Returns the raw pointer (doesn't transfer ownership) |
| `w.reset()` | Deletes owned object, sets to `nullptr` |

### Practice

2. 🌟🌟 **Fill:** Create a `unique_ptr<Widget>` using `make_unique`.

```cpp,editable
#include <iostream>
#include <memory>
#include <string>
#include <cassert>

class Widget {
    std::string name_;
public:
    Widget(std::string name) : name_(std::move(name)) {}
    const std::string& name() const { return name_; }
};

int main() {
    auto w = __;

    assert(w != nullptr);
    assert(w->name() == "MyWidget");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## shared_ptr

`std::shared_ptr` uses **reference counting**: multiple `shared_ptr`s can point to the same object. The object is deleted only when the **last** `shared_ptr` owning it is destroyed. Use `use_count()` to inspect the reference count.

### How it works

```cpp
#include <iostream>
#include <memory>

int main() {
    auto p1 = std::make_shared<int>(42);
    std::cout << "Count: " << p1.use_count() << std::endl;  // 1

    {
        auto p2 = p1;  // copy — count goes up
        std::cout << "Count: " << p1.use_count() << std::endl;  // 2
    }
    // p2 destroyed — count goes back down
    std::cout << "Count: " << p1.use_count() << std::endl;  // 1
    return 0;
}
```

| Operation | Effect on `use_count()` |
|-----------|------------------------|
| `make_shared<T>(args)` | Creates with count = 1 |
| Copy (`auto p2 = p1`) | Count increments |
| `shared_ptr` destroyed | Count decrements; object deleted when count reaches 0 |

### Practice

3. 🌟🌟 **Fill:** What is the `use_count()` at each point?

```cpp,editable
#include <iostream>
#include <memory>
#include <cassert>

int main() {
    auto p1 = std::make_shared<int>(42);
    assert(p1.use_count() == __);

    auto p2 = p1;
    assert(p1.use_count() == __);
    assert(p2.use_count() == __);

    p2.reset();
    assert(p1.use_count() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## weak_ptr

`std::weak_ptr` observes a `shared_ptr`-managed object **without** incrementing the reference count. Its main use is breaking **circular references** that would otherwise prevent `shared_ptr` objects from ever reaching count 0.

### How it works

```cpp
#include <iostream>
#include <memory>
#include <string>

struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;  // weak to avoid circular reference

    Node(std::string n) : name(std::move(n)) {}
    ~Node() { std::cout << name << " destroyed\n"; }
};

int main() {
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");

    a->next = b;      // A → B (shared)
    b->prev = a;      // B → A (weak, no ownership cycle)

    return 0;  // Both A and B are properly destroyed
}
```

| Concept | Detail |
|---------|--------|
| `weak_ptr` | Non-owning observer; doesn't affect reference count |
| `wp.lock()` | Returns a `shared_ptr` if the object still exists, else empty |
| Circular reference | Two `shared_ptr`s pointing to each other → neither ever freed |
| Fix | Make one direction a `weak_ptr` |

### Practice

4. 🌟🌟🌟 **Fix:** `A` and `B` both hold `shared_ptr` to each other, creating a circular reference (neither is ever destroyed). Change one link to `weak_ptr`.

```cpp,editable
#include <iostream>
#include <memory>
#include <string>
#include <cassert>

struct Node {
    std::string name;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // BUG: creates circular reference

    Node(std::string n) : name(std::move(n)) {}
    ~Node() { std::cout << name << " destroyed" << std::endl; }
};

int main() {
    auto a = std::make_shared<Node>("A");
    auto b = std::make_shared<Node>("B");

    a->next = b;
    b->prev = a;

    // After this scope, both nodes should be destroyed
    // but with circular shared_ptr they won't be!
    a.reset();
    b.reset();

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
