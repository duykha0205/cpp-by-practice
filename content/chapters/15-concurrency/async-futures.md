# Async & Futures

`std::async`, `std::future`, and `std::promise` provide a higher-level concurrency model than raw threads. Instead of managing threads directly, you launch tasks and retrieve their results through **futures** — a clean way to pass values (or exceptions) between threads.

> **Note:** Compile all exercises in this file with the `-pthread` flag (e.g., `g++ -std=c++17 -pthread main.cpp`).

## std::async

`std::async` launches a callable and returns a `std::future` holding the eventual result. You can specify a **launch policy**: `std::launch::async` forces a new thread, while `std::launch::deferred` delays execution until `get()` is called (runs in the calling thread).

### How it works

```cpp
#include <iostream>
#include <future>

int compute(int a, int b) {
    return a + b;
}

int main() {
    // Launch asynchronously (may run in a new thread)
    auto fut1 = std::async(std::launch::async, compute, 10, 20);

    // Deferred: runs only when get() is called
    auto fut2 = std::async(std::launch::deferred, compute, 30, 40);

    std::cout << fut1.get() << std::endl;  // 30
    std::cout << fut2.get() << std::endl;  // 70
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::async(func, args...)` | Launches `func` and returns a `std::future` |
| `std::launch::async` | Forces execution in a new thread |
| `std::launch::deferred` | Lazy evaluation — runs when `get()` or `wait()` is called |
| Default policy | Implementation-defined (may be async or deferred) |

### Practice

1. 🌟🌟 **Fill:** Launch an async task that computes the square of a number, then retrieve the result with `get()`.

```cpp,editable
#include <iostream>
#include <future>
#include <cassert>

int square(int x) {
    return x * x;
}

int main() {
    auto fut = std::async(__, __, 7);

    int result = fut.__();

    assert(result == 49);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Fill:** Launch two async tasks, one with `std::launch::async` and one with `std::launch::deferred`. Retrieve both results.

```cpp,editable
#include <iostream>
#include <future>
#include <cassert>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    auto fut_add = std::async(__, add, 3, 4);
    auto fut_mul = std::async(__, multiply, 3, 4);

    int sum = fut_add.get();
    int product = fut_mul.get();

    assert(sum == 7);
    assert(product == 12);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::future

A `std::future` represents a value that will become available at some point. Calling `get()` blocks until the value is ready (and can only be called **once**). `wait()` blocks without consuming the value. `valid()` checks if the future has an associated shared state. For multiple consumers, use `std::shared_future`.

### How it works

```cpp
#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int slow_compute() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return 42;
}

int main() {
    auto fut = std::async(std::launch::async, slow_compute);

    std::cout << "valid: " << fut.valid() << std::endl;  // true

    int result = fut.get();  // blocks until ready
    std::cout << "Result: " << result << std::endl;       // 42
    std::cout << "valid: " << fut.valid() << std::endl;   // false (consumed)

    // shared_future can be copied and get() called multiple times
    auto fut2 = std::async(std::launch::async, slow_compute);
    std::shared_future<int> sfut = fut2.share();
    std::cout << sfut.get() << std::endl;  // 42
    std::cout << sfut.get() << std::endl;  // 42 (can call again)
    return 0;
}
```

| Operation | Detail |
|-----------|--------|
| `fut.get()` | Blocks until result is ready; can only call once on `std::future` |
| `fut.wait()` | Blocks until result is ready; does not consume the value |
| `fut.valid()` | Returns `true` if the future has an associated shared state |
| `fut.share()` | Converts to `shared_future`; `get()` can be called multiple times |

### Practice

3. 🌟🌟🌟 **Fill:** Create a `shared_future` from an async task so that two threads can both read the result.

```cpp,editable
#include <iostream>
#include <future>
#include <thread>
#include <cassert>
#include <atomic>

int compute() {
    return 42;
}

int main() {
    auto fut = std::async(std::launch::async, compute);

    // Convert to shared_future so multiple consumers can call get()
    std::shared_future<int> sfut = fut.__();

    std::atomic<int> sum{0};

    std::thread t1([&sfut, &sum]() {
        sum += sfut.get();
    });
    std::thread t2([&sfut, &sum]() {
        sum += sfut.get();
    });

    t1.join();
    t2.join();

    // Both threads got 42
    assert(sum == 84);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::promise

A `std::promise` is the **write end** of a future. One thread sets a value (or exception) on the promise, and another thread reads it from the corresponding `std::future`. This is a direct thread-to-thread communication channel.

### How it works

```cpp
#include <iostream>
#include <future>
#include <thread>

void producer(std::promise<int> prom) {
    int result = 6 * 7;
    prom.set_value(result);  // send result to the consumer
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(producer, std::move(prom));

    int value = fut.get();  // blocks until producer sets the value
    std::cout << "Got: " << value << std::endl;  // 42

    t.join();
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::promise<T>` | Write end — call `set_value()` or `set_exception()` |
| `prom.get_future()` | Returns the `std::future<T>` connected to this promise |
| `std::move(prom)` | Promises can't be copied, only moved |
| `set_exception()` | Stores an exception that `get()` will rethrow |

### Practice

4. 🌟🌟 **Fill:** Use a `std::promise` and `std::future` to send a value from a worker thread to the main thread.

```cpp,editable
#include <iostream>
#include <future>
#include <thread>
#include <cassert>

void worker(std::promise<int> prom) {
    int result = 10 + 32;
    prom.__(__);
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.__();

    std::thread t(worker, std::move(prom));

    int value = fut.get();

    assert(value == 42);

    t.join();

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Use a promise to send an exception from a worker thread. The main thread should catch it from the future.

```cpp,editable
#include <iostream>
#include <future>
#include <thread>
#include <stdexcept>
#include <cassert>
#include <string>

void failing_worker(std::promise<int> prom) {
    try {
        throw std::runtime_error("something went wrong");
    } catch (...) {
        // Send the current exception to the future
        prom.__(std::current_exception());
    }
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(failing_worker, std::move(prom));

    bool caught = false;
    try {
        fut.get();  // should rethrow the exception
    } catch (const std::runtime_error& e) {
        caught = true;
        assert(std::string(e.what()) == "something went wrong");
    }

    assert(caught == true);

    t.join();

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
