# Coroutines

Coroutines are functions that can **suspend** and **resume** execution. Unlike regular functions that run from start to finish, a coroutine can pause midway, return a value to the caller, and later pick up exactly where it left off. C++20 introduces coroutines with three new keywords: `co_return`, `co_yield`, and `co_await`.

> **Note:** Coroutines require GCC 11+ or Clang 14+ with `-std=c++20` and `-fcoroutines` (GCC). MSVC 19.28+ supports them natively with `/std:c++20`.

## Coroutine Basics

A function becomes a coroutine the moment its body contains `co_return`, `co_yield`, or `co_await`. Coroutines require a **promise type** that controls their behavior — the compiler looks for it inside the return type.

### How it works

```cpp
#include <iostream>
#include <coroutine>
#include <optional>

// A minimal Generator<T> class for producing values one at a time.
// Students can reuse this class in all coroutine exercises.
template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        bool finished = false;

        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() { finished = true; }
        void unhandled_exception() { std::terminate(); }
    };

    using handle_type = std::coroutine_handle<promise_type>;

    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }

    // Move only
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }
    Generator& operator=(Generator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    // Advance the coroutine and check if a value is available
    bool next() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    T value() const { return handle_.promise().current_value; }

private:
    handle_type handle_;
};

// A coroutine that yields 1, 2, 3
Generator<int> count_up_to_three() {
    co_yield 1;
    co_yield 2;
    co_yield 3;
}

int main() {
    auto gen = count_up_to_three();

    while (gen.next()) {
        std::cout << gen.value() << " ";  // 1 2 3
    }
    std::cout << std::endl;
    return 0;
}
```

| Keyword | Purpose |
|---------|---------|
| `co_yield expr` | Suspend and produce a value to the caller |
| `co_return` | End the coroutine (optionally with a value) |
| `co_await expr` | Suspend until the awaited operation completes |
| `promise_type` | Nested type inside the return type that controls coroutine behavior |
| `std::coroutine_handle` | Low-level handle to resume or destroy a coroutine |
| `std::suspend_always` | An awaitable that always suspends |
| `std::suspend_never` | An awaitable that never suspends |

### Practice

1. 🌟🌟 **Fill:** Complete the coroutine so it yields the values 10, 20, and 30 using `co_yield`.

```cpp,editable
#include <iostream>
#include <coroutine>
#include <cassert>

// Reusable Generator<T> class
template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

Generator<int> three_values() {
    __;  // yield 10
    __;  // yield 20
    __;  // yield 30
}

int main() {
    auto gen = three_values();

    assert(gen.next());
    assert(gen.value() == 10);
    assert(gen.next());
    assert(gen.value() == 20);
    assert(gen.next());
    assert(gen.value() == 30);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Generator Pattern

The generator pattern uses `co_yield` to produce a sequence of values **lazily** — values are computed one at a time, only when the caller asks for the next one. This is ideal for sequences that are expensive to compute or potentially infinite.

### How it works

```cpp
#include <iostream>
#include <coroutine>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

// Infinite sequence of natural numbers
Generator<int> naturals(int start = 1) {
    int n = start;
    while (true) {
        co_yield n++;
    }
}

int main() {
    auto gen = naturals();
    for (int i = 0; i < 5; ++i) {
        gen.next();
        std::cout << gen.value() << " ";  // 1 2 3 4 5
    }
    std::cout << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Lazy evaluation | Values are computed only when `next()` is called |
| Infinite sequences | The coroutine can loop forever; caller controls how many values to consume |
| Memory efficient | Only one value exists in memory at a time |
| `co_yield n++` | Yields the current value of `n`, then increments it |

### Practice

2. 🌟🌟 **Fill:** Complete the coroutine that generates the first N even numbers starting from 2.

```cpp,editable
#include <iostream>
#include <coroutine>
#include <cassert>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

Generator<int> even_numbers(int count) {
    int value = 2;
    for (int i = 0; i < count; ++i) {
        __;   // yield the current even value
        __;   // advance to the next even number
    }
}

int main() {
    auto gen = even_numbers(4);

    assert(gen.next());
    assert(gen.value() == 2);
    assert(gen.next());
    assert(gen.value() == 4);
    assert(gen.next());
    assert(gen.value() == 6);
    assert(gen.next());
    assert(gen.value() == 8);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Write a coroutine `fibonacci()` that yields an infinite sequence of Fibonacci numbers (0, 1, 1, 2, 3, 5, 8, ...).

```cpp,editable
#include <iostream>
#include <coroutine>
#include <cassert>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

Generator<long long> fibonacci() {
    __;  // initialize two variables for the Fibonacci state
    while (true) {
        __;  // yield the current Fibonacci number
        __;  // advance to the next Fibonacci pair
    }
}

int main() {
    auto fib = fibonacci();

    // Expected: 0, 1, 1, 2, 3, 5, 8
    long long expected[] = {0, 1, 1, 2, 3, 5, 8};
    for (int i = 0; i < 7; ++i) {
        assert(fib.next());
        assert(fib.value() == expected[i]);
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Lazy Evaluation

Coroutines enable **lazy evaluation** — computing values only when they are needed. This is useful for expensive computations, streaming data, or pipelining transformations without materializing entire collections in memory.

### How it works

```cpp
#include <iostream>
#include <coroutine>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

// Lazily squares numbers from a source generator
Generator<int> lazy_square(Generator<int> source) {
    while (source.next()) {
        co_yield source.value() * source.value();
    }
}

// Produces numbers 1 through n
Generator<int> iota(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;
    }
}

int main() {
    // Pipeline: iota(5) -> lazy_square -> consume
    auto squares = lazy_square(std::move(iota(5)));

    while (squares.next()) {
        std::cout << squares.value() << " ";  // 1 4 9 16 25
    }
    std::cout << std::endl;
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Lazy pipeline | Each stage pulls values on demand from the previous stage |
| No intermediate storage | Values flow one at a time through the pipeline |
| Composable | Generators can be chained: `iota -> filter -> transform -> consume` |
| Early termination | The consumer can stop pulling at any time; unused values are never computed |

### Practice

4. 🌟🌟🌟 **Complete:** Write a coroutine `lazy_filter` that takes a generator and yields only values that satisfy a predicate.

```cpp,editable
#include <iostream>
#include <coroutine>
#include <functional>
#include <cassert>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

Generator<int> iota(int n) {
    for (int i = 1; i <= n; ++i) {
        co_yield i;
    }
}

Generator<int> lazy_filter(Generator<int> source, std::function<bool(int)> pred) {
    __;  // iterate through source, yielding only values where pred returns true
}

int main() {
    // Filter: keep only even numbers from 1..10
    auto evens = lazy_filter(std::move(iota(10)), [](int x) { return x % 2 == 0; });

    int expected[] = {2, 4, 6, 8, 10};
    for (int i = 0; i < 5; ++i) {
        assert(evens.next());
        assert(evens.value() == expected[i]);
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Write a coroutine `take` that yields at most N values from a source generator, demonstrating early termination.

```cpp,editable
#include <iostream>
#include <coroutine>
#include <cassert>

template<typename T>
class Generator {
public:
    struct promise_type {
        T current_value;
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
    using handle_type = std::coroutine_handle<promise_type>;
    explicit Generator(handle_type h) : handle_(h) {}
    ~Generator() { if (handle_) handle_.destroy(); }
    Generator(Generator&& o) noexcept : handle_(o.handle_) { o.handle_ = nullptr; }
    bool next() { if (!handle_ || handle_.done()) return false; handle_.resume(); return !handle_.done(); }
    T value() const { return handle_.promise().current_value; }
private:
    handle_type handle_;
};

// Infinite generator: 1, 2, 3, ...
Generator<int> naturals() {
    int n = 1;
    while (true) {
        co_yield n++;
    }
}

Generator<int> take(Generator<int> source, int n) {
    __;  // yield at most n values from source, then stop
}

int main() {
    // Take first 5 values from an infinite sequence
    auto first_five = take(std::move(naturals()), 5);

    int expected[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) {
        assert(first_five.next());
        assert(first_five.value() == expected[i]);
    }
    // No more values
    assert(!first_five.next());

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
