# RAII & Rule of Five

C++ gives you direct control over object lifetimes. **RAII** ties resource management to object scope, and the **Rule of Five** ensures classes that manage resources handle copying, moving, and destruction correctly. Together with `= default`, `= delete`, and the copy-and-swap idiom, these patterns form the backbone of safe, leak-free C++ code.

## RAII Pattern

**Resource Acquisition Is Initialization** means you acquire a resource (memory, file handle, lock) in the constructor and release it in the destructor. Because C++ guarantees destructors run when objects go out of scope, resources are always cleaned up — even when exceptions are thrown.

### How it works

```cpp
#include <iostream>
#include <cstring>

class StringBuffer {
    char* data_;
    size_t size_;
public:
    StringBuffer(const char* str)
        : size_(std::strlen(str))
        , data_(new char[std::strlen(str) + 1])
    {
        std::strcpy(data_, str);
        std::cout << "Acquired: " << data_ << std::endl;
    }

    ~StringBuffer() {
        std::cout << "Released: " << data_ << std::endl;
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    size_t size() const { return size_; }
};

int main() {
    {
        StringBuffer buf("hello");
        std::cout << "Using: " << buf.c_str() << std::endl;
    }  // destructor runs here — memory freed automatically
    std::cout << "Out of scope" << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Constructor acquires | `new`, `fopen`, `lock` — resource obtained when object is created |
| Destructor releases | `delete`, `fclose`, `unlock` — resource freed when object is destroyed |
| Scope-based lifetime | Destructor is called automatically at end of scope or on exception |
| No manual cleanup | Eliminates `delete` / `free` calls scattered through code |

### Practice

1. 🌟🌟 **Fill:** Complete the RAII class `IntArray` so that the constructor allocates an array of `n` ints (all zero-initialised) and the destructor frees it.

```cpp,editable
#include <iostream>
#include <cassert>

class IntArray {
    int* data_;
    int size_;
public:
    IntArray(int n) : size_(n) {
        __ // allocate an array of n ints, zero-initialised: new int[n]()
    }

    ~IntArray() {
        __ // free the array
    }

    int& operator[](int i) { return data_[i]; }
    int size() const { return size_; }
};

int main() {
    IntArray arr(5);
    assert(arr.size() == 5);
    assert(arr[0] == 0);  // zero-initialised

    arr[0] = 42;
    arr[4] = 99;
    assert(arr[0] == 42);
    assert(arr[4] == 99);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Rule of Three / Rule of Five

If your class manages a resource, the compiler-generated copy/move operations will do a **shallow copy** — two objects pointing to the same resource. You need to define: (1) copy constructor, (2) copy assignment, (3) destructor (Rule of Three), plus (4) move constructor and (5) move assignment (Rule of Five) for efficiency.

### How it works

```cpp
#include <iostream>
#include <cstring>
#include <utility>

class Buffer {
    char* data_;
    size_t size_;
public:
    // Constructor
    Buffer(const char* str)
        : size_(std::strlen(str))
        , data_(new char[size_ + 1])
    {
        std::strcpy(data_, str);
    }

    // 1. Copy constructor — deep copy
    Buffer(const Buffer& other)
        : size_(other.size_)
        , data_(new char[size_ + 1])
    {
        std::strcpy(data_, other.data_);
    }

    // 2. Copy assignment
    Buffer& operator=(const Buffer& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            data_ = new char[size_ + 1];
            std::strcpy(data_, other.data_);
        }
        return *this;
    }

    // 3. Destructor
    ~Buffer() { delete[] data_; }

    // 4. Move constructor
    Buffer(Buffer&& other) noexcept
        : data_(other.data_), size_(other.size_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 5. Move assignment
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    const char* c_str() const { return data_; }
    size_t size() const { return size_; }
};

int main() {
    Buffer a("hello");
    Buffer b = a;           // copy constructor
    Buffer c("world");
    c = a;                  // copy assignment
    Buffer d = std::move(a); // move constructor
    std::cout << b.c_str() << " " << c.c_str() << " " << d.c_str() << std::endl;
    return 0;
}
```

| Special member | Signature | Purpose |
|----------------|-----------|---------|
| Copy constructor | `T(const T&)` | Deep copy from another object |
| Copy assignment | `T& operator=(const T&)` | Deep copy into existing object |
| Destructor | `~T()` | Release owned resources |
| Move constructor | `T(T&&) noexcept` | Steal resources from a temporary |
| Move assignment | `T& operator=(T&&) noexcept` | Steal resources into existing object |

### Practice

2. 🌟🌟🌟 **Fill:** Complete the move constructor for `UniqueInt`. It should steal the pointer from `other` and leave `other` in a null state.

```cpp,editable
#include <iostream>
#include <utility>
#include <cassert>

class UniqueInt {
    int* ptr_;
public:
    UniqueInt(int val) : ptr_(new int(val)) {}
    ~UniqueInt() { delete ptr_; }

    // Move constructor
    UniqueInt(UniqueInt&& other) noexcept {
        __ // steal other.ptr_ into this->ptr_
        __ // set other.ptr_ to nullptr
    }

    // Delete copy operations
    UniqueInt(const UniqueInt&) = delete;
    UniqueInt& operator=(const UniqueInt&) = delete;

    int value() const { return *ptr_; }
    bool is_valid() const { return ptr_ != nullptr; }
};

int main() {
    UniqueInt a(42);
    assert(a.value() == 42);

    UniqueInt b(std::move(a));
    assert(b.value() == 42);
    assert(a.is_valid() == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Implement the copy constructor and copy assignment operator for `IntVec` that perform a deep copy of the internal array.

```cpp,editable
#include <iostream>
#include <cassert>

class IntVec {
    int* data_;
    int size_;
public:
    IntVec(int n) : size_(n), data_(new int[n]()) {}

    ~IntVec() { delete[] data_; }

    // Copy constructor — deep copy
    IntVec(const IntVec& other) : size_(other.size_) {
        __ // allocate new array of size_ ints
        for (int i = 0; i < size_; ++i)
            __ // copy each element from other.data_
    }

    // Copy assignment — deep copy
    IntVec& operator=(const IntVec& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            __ // allocate new array of size_ ints
            for (int i = 0; i < size_; ++i)
                __ // copy each element from other.data_
        }
        return *this;
    }

    int& operator[](int i) { return data_[i]; }
    const int& operator[](int i) const { return data_[i]; }
    int size() const { return size_; }
};

int main() {
    IntVec a(3);
    a[0] = 10; a[1] = 20; a[2] = 30;

    IntVec b = a;  // copy constructor
    assert(b[0] == 10 && b[1] == 20 && b[2] == 30);

    b[0] = 99;
    assert(a[0] == 10);  // original unchanged — deep copy!

    IntVec c(1);
    c = a;  // copy assignment
    assert(c[0] == 10 && c[1] == 20 && c[2] == 30);
    assert(c.size() == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Default & Delete

C++ lets you explicitly request the compiler-generated version of a special member with `= default`, or prevent an operation entirely with `= delete`. This makes intent crystal clear and avoids subtle bugs.

### How it works

```cpp
#include <iostream>

class Movable {
public:
    Movable() = default;
    ~Movable() = default;

    // Allow moves
    Movable(Movable&&) = default;
    Movable& operator=(Movable&&) = default;

    // Forbid copies
    Movable(const Movable&) = delete;
    Movable& operator=(const Movable&) = delete;
};

class Singleton {
    static Singleton* instance_;
    Singleton() = default;
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static Singleton& get() {
        static Singleton s;
        return s;
    }
};

int main() {
    Movable a;
    Movable b = std::move(a);  // OK — move allowed
    // Movable c = b;           // ERROR — copy deleted

    Singleton& s = Singleton::get();  // OK
    // Singleton s2 = s;              // ERROR — copy deleted
    std::cout << "Done" << std::endl;
    return 0;
}
```

| Keyword | Effect |
|---------|--------|
| `= default` | Compiler generates the default implementation |
| `= delete` | Any call to this function is a compile error |
| Deleted copy + default move | Makes a type move-only (like `std::unique_ptr`) |
| Deleted default constructor | Forces callers to use a parameterised constructor |

### Practice

4. 🌟🌟 **Fix:** The `NonCopyable` class should prevent copying but allow moving. Add `= delete` and `= default` to the correct special members.

```cpp,editable
#include <iostream>
#include <utility>
#include <cassert>

class NonCopyable {
    int value_;
public:
    NonCopyable(int v) : value_(v) {}
    ~NonCopyable() = default;

    // FIX: delete copy operations
    NonCopyable(const NonCopyable&) __
    NonCopyable& operator=(const NonCopyable&) __

    // FIX: default move operations
    NonCopyable(NonCopyable&&) __
    NonCopyable& operator=(NonCopyable&&) __

    int value() const { return value_; }
};

int main() {
    NonCopyable a(42);
    // NonCopyable b = a;  // should not compile (copy deleted)

    NonCopyable c = std::move(a);  // move constructor
    assert(c.value() == 42);

    NonCopyable d(10);
    d = std::move(c);  // move assignment
    assert(d.value() == 42);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Copy-and-Swap Idiom

The copy-and-swap idiom implements the assignment operator by making a copy of the parameter, then swapping internals with the copy. This provides **strong exception safety** (if the copy throws, `this` is unchanged) and handles self-assignment automatically.

### How it works

```cpp
#include <iostream>
#include <algorithm>
#include <cstring>
#include <utility>

class String {
    char* data_;
    size_t size_;

    friend void swap(String& a, String& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
        swap(a.size_, b.size_);
    }

public:
    String(const char* str = "")
        : size_(std::strlen(str))
        , data_(new char[size_ + 1])
    {
        std::strcpy(data_, str);
    }

    // Copy constructor
    String(const String& other)
        : size_(other.size_)
        , data_(new char[size_ + 1])
    {
        std::strcpy(data_, other.data_);
    }

    // Unified assignment using copy-and-swap
    String& operator=(String other) {  // note: pass by VALUE
        swap(*this, other);            // swap internals
        return *this;                  // old data destroyed with 'other'
    }

    ~String() { delete[] data_; }

    const char* c_str() const { return data_; }
};

int main() {
    String a("hello");
    String b("world");
    b = a;  // copy-and-swap: safe even for self-assignment
    std::cout << b.c_str() << std::endl;  // hello
    return 0;
}
```

| Step | What happens |
|------|-------------|
| Pass by value | The parameter `other` is a fresh copy (copy constructor runs) |
| `swap(*this, other)` | This object gets the new data; `other` gets the old data |
| `other` destructor | Cleans up the old data automatically at end of function |
| Self-assignment | `a = a` — swap with a copy of yourself is harmless |
| Exception safety | If the copy (step 1) throws, `this` is never modified |

### Practice

5. 🌟🌟🌟 **Fill:** Complete the `swap` friend function and the copy-and-swap assignment operator for `DynArray`.

```cpp,editable
#include <iostream>
#include <algorithm>
#include <utility>
#include <cassert>

class DynArray {
    int* data_;
    int size_;

    friend void swap(DynArray& a, DynArray& b) noexcept {
        using std::swap;
        __ // swap a.data_ and b.data_
        __ // swap a.size_ and b.size_
    }

public:
    DynArray(int n) : size_(n), data_(new int[n]()) {}

    // Copy constructor
    DynArray(const DynArray& other)
        : size_(other.size_), data_(new int[other.size_])
    {
        for (int i = 0; i < size_; ++i)
            data_[i] = other.data_[i];
    }

    // Copy-and-swap assignment
    DynArray& operator=(DynArray other) {  // pass by value
        __ // swap *this with other
        return *this;
    }

    ~DynArray() { delete[] data_; }

    int& operator[](int i) { return data_[i]; }
    const int& operator[](int i) const { return data_[i]; }
    int size() const { return size_; }
};

int main() {
    DynArray a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    DynArray b(1);
    b = a;  // copy-and-swap assignment

    assert(b.size() == 3);
    assert(b[0] == 1 && b[1] == 2 && b[2] == 3);

    b[0] = 99;
    assert(a[0] == 1);  // deep copy — original unchanged

    a = a;  // self-assignment — should not crash
    assert(a[0] == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
