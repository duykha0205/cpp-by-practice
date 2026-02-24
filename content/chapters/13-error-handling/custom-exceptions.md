# Custom Exceptions & Exception Safety

While the standard exception hierarchy covers many common cases, real applications often need **custom exception classes** that carry domain-specific error information. Equally important is understanding **exception safety guarantees** — the promises your code makes about its state when an exception is thrown — and how **RAII** keeps resources safe during stack unwinding.

## Custom Exception Classes

You create custom exceptions by inheriting from `std::exception` (or one of its subclasses like `std::runtime_error`). Override the `what()` method to return a descriptive message, and add any extra data members your error handling needs.

### How it works

```cpp
#include <iostream>
#include <stdexcept>
#include <string>

class FileError : public std::runtime_error {
    std::string filename_;
    int line_;
public:
    FileError(const std::string& filename, int line, const std::string& msg)
        : std::runtime_error(msg), filename_(filename), line_(line) {}

    const std::string& filename() const { return filename_; }
    int line() const { return line_; }
};

void read_config(const std::string& filename) {
    // Simulate a parse error at line 42
    throw FileError(filename, 42, "unexpected token");
}

int main() {
    try {
        read_config("app.conf");
    } catch (const FileError& e) {
        std::cout << e.filename() << ":" << e.line()
                  << ": " << e.what() << std::endl;
        // Output: app.conf:42: unexpected token
    }
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Inherit from `std::runtime_error` | Gets `what()` implementation for free via constructor |
| Inherit from `std::exception` | Must override `what()` yourself (returns `const char*`) |
| Extra data members | Carry error context (filename, error code, etc.) |
| Catch by `const` reference | Always; avoids slicing |

### Practice

1. :star::star: **Fill:** Complete the custom exception class that stores an HTTP status code and message. It should inherit from `std::runtime_error` and provide an accessor for the status code.

```cpp,editable
#include <iostream>
#include <stdexcept>
#include <string>
#include <cassert>

class HttpError : public __ {
    int status_code_;
public:
    HttpError(int code, const std::string& msg)
        : __(msg), status_code_(code) {}

    int status_code() const { return __; }
};

void fetch_url(const std::string& url) {
    throw HttpError(404, "Not Found");
}

int main() {
    bool caught = false;
    try {
        fetch_url("https://example.com/missing");
    } catch (const HttpError& e) {
        caught = true;
        assert(e.status_code() == 404);
        assert(std::string(e.what()) == "Not Found");
    }
    assert(caught);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. :star::star::star: **Complete:** Create a custom exception class `ValidationError` that inherits directly from `std::exception`. It must store a field name and error message, and override `what()` to return a formatted string like `"field_name: error message"`.

```cpp,editable
#include <iostream>
#include <exception>
#include <string>
#include <cassert>
#include <cstring>

class ValidationError : public std::exception {
    std::string field_;
    std::string message_;
    std::string what_msg_;
public:
    // Constructor: store field, message, and build the what() string
    __

    const char* what() const noexcept override {
        __
    }

    const std::string& field() const { return field_; }
    const std::string& message() const { return message_; }
};

void validate_age(int age) {
    if (age < 0)
        throw ValidationError("age", "must be non-negative");
    if (age > 150)
        throw ValidationError("age", "unrealistic value");
}

int main() {
    bool caught = false;
    try {
        validate_age(-5);
    } catch (const ValidationError& e) {
        caught = true;
        assert(e.field() == "age");
        assert(e.message() == "must be non-negative");
        assert(std::string(e.what()) == "age: must be non-negative");
    }
    assert(caught);

    // Also works when caught as std::exception
    bool caught_base = false;
    try {
        validate_age(200);
    } catch (const std::exception& e) {
        caught_base = true;
        assert(std::string(e.what()) == "age: unrealistic value");
    }
    assert(caught_base);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Exception Safety Guarantees

When a function throws, the question is: what state are your objects in? C++ defines three levels of exception safety. Understanding them helps you write robust code — especially in containers and resource managers.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

// Strong guarantee: if push fails, vec is unchanged
template<typename T>
void safe_push(std::vector<T>& vec, const T& value) {
    std::vector<T> copy = vec;  // work on a copy
    copy.push_back(value);      // might throw (e.g., bad_alloc)
    std::swap(vec, copy);       // swap is noexcept — commit the change
}

// Basic guarantee: invariants hold, but state may change
void basic_insert(std::vector<int>& vec, int pos, int value) {
    if (pos < 0 || pos > static_cast<int>(vec.size()))
        throw std::out_of_range("bad position");
    vec.insert(vec.begin() + pos, value);
}

// No-throw guarantee: never throws
void safe_swap(std::vector<int>& a, std::vector<int>& b) noexcept {
    a.swap(b);  // std::vector::swap is noexcept
}

int main() {
    std::vector<int> v = {1, 2, 3};
    safe_push(v, 4);
    std::cout << v.size() << std::endl;  // 4
    return 0;
}
```

| Guarantee | Promise | Example |
|-----------|---------|---------|
| **No-throw** | Function never throws; marked `noexcept` | Destructors, `swap`, move operations |
| **Strong** | If an exception occurs, state is rolled back (as if the call never happened) | Copy-and-swap idiom |
| **Basic** | If an exception occurs, no resources leak and invariants hold, but state may have changed | Most standard library operations |

### Practice

3. :star::star: **Fix:** The `safe_assign` function is supposed to provide the **strong guarantee** using copy-and-swap, but it modifies the original data directly. Fix it so that if the copy throws, the original vector is untouched.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

// Should provide the STRONG guarantee:
// if anything throws, 'dest' must be unchanged.
void safe_assign(std::vector<int>& dest, const std::vector<int>& src) {
    // BUG: modifies dest directly — if an exception happens
    // midway, dest is left in a partial state
    dest.clear();
    for (const auto& val : src) {
        dest.push_back(val);
    }
}

int main() {
    std::vector<int> original = {1, 2, 3};
    std::vector<int> source = {10, 20, 30, 40};

    safe_assign(original, source);

    assert(original.size() == 4);
    assert(original[0] == 10);
    assert(original[3] == 40);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Stack Unwinding & RAII

When an exception is thrown, the runtime **unwinds the stack**: it destroys all local objects in each frame as it searches for a matching `catch` block. This is why **RAII** (Resource Acquisition Is Initialization) is critical — if you hold resources in RAII objects, their destructors run automatically during unwinding, preventing leaks.

### How it works

```cpp
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <memory>

class FileHandle {
    std::string name_;
public:
    FileHandle(const std::string& name) : name_(name) {
        std::cout << "Opening " << name_ << std::endl;
    }
    ~FileHandle() {
        std::cout << "Closing " << name_ << std::endl;  // always runs
    }
};

void process() {
    FileHandle f("data.txt");         // RAII: destructor closes file
    auto ptr = std::make_unique<int>(42);  // RAII: unique_ptr frees memory
    throw std::runtime_error("oops");
    // ~unique_ptr and ~FileHandle run during stack unwinding
}

int main() {
    try {
        process();
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << std::endl;
    }
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Stack unwinding | Destructors of all local objects are called as the stack unwinds |
| RAII | Tie resource lifetime to object lifetime — acquire in constructor, release in destructor |
| Raw `new` without RAII | Memory leaks if an exception occurs before `delete` |
| `std::unique_ptr` | RAII wrapper for heap memory — safe with exceptions |

### Practice

4. :star::star: **Fix:** The function uses raw `new` and `delete`. If an exception is thrown between them, the memory leaks. Convert to RAII using `std::unique_ptr`.

```cpp,editable
#include <iostream>
#include <memory>
#include <stdexcept>
#include <cassert>

class Sensor {
    int id_;
public:
    Sensor(int id) : id_(id) {}
    int id() const { return id_; }
};

std::string read_sensor(int id) {
    // BUG: raw new — if an exception is thrown, this memory leaks
    Sensor* s = new Sensor(id);

    if (id < 0)
        throw std::invalid_argument("negative sensor id");

    int sensor_id = s->id();
    delete s;

    return "sensor_" + std::to_string(sensor_id);
}

int main() {
    assert(read_sensor(5) == "sensor_5");

    bool caught = false;
    try {
        read_sensor(-1);
    } catch (const std::invalid_argument& e) {
        caught = true;
    }
    assert(caught);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. :star::star::star: **Complete:** Implement a `Transaction` RAII class that tracks whether a transaction was committed. If the transaction is not committed before destruction (e.g., due to an exception), it should automatically roll back. This pattern is called **scope guard**.

```cpp,editable
#include <iostream>
#include <string>
#include <cassert>

std::string log_output;

class Transaction {
    std::string name_;
    bool committed_;
public:
    // Constructor: store name, set committed_ to false, log "begin <name>"
    __

    // Destructor: if not committed, log "rollback <name>"
    __

    // commit(): set committed_ to true, log "commit <name>"
    __
};

void successful_operation() {
    Transaction t("transfer");
    // ... do work ...
    t.commit();  // explicitly commit
}  // destructor runs, but no rollback since committed

void failing_operation() {
    Transaction t("payment");
    // ... do work ...
    throw std::runtime_error("insufficient funds");
    // destructor runs during stack unwinding -> rollback
}

int main() {
    log_output.clear();
    successful_operation();
    assert(log_output == "begin transfer\ncommit transfer\n");

    log_output.clear();
    try {
        failing_operation();
    } catch (...) {}
    assert(log_output == "begin payment\nrollback payment\n");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
