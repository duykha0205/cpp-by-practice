# Threads

Threads let you run multiple paths of execution **concurrently** within a single program. C++11 introduced `std::thread` as a portable threading abstraction. Every thread you create must be either **joined** (waited on) or **detached** before it goes out of scope — otherwise the program calls `std::terminate`.

> **Note:** Compile all exercises in this file with the `-pthread` flag (e.g., `g++ -std=c++17 -pthread main.cpp`).

## Creating Threads

`std::thread` takes any **callable** (function, lambda, functor) and runs it in a new thread. You must call `join()` on the thread to wait for it to finish; failing to do so before the `std::thread` object is destroyed causes `std::terminate`.

### How it works

```cpp
#include <iostream>
#include <thread>

void hello() {
    std::cout << "Hello from thread!" << std::endl;
}

int main() {
    std::thread t(hello);   // launch thread running hello()
    t.join();               // wait for thread to finish
    // Without join(), the program would call std::terminate
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::thread t(func)` | Creates a new thread that immediately begins executing `func` |
| `t.join()` | Blocks the calling thread until `t` finishes |
| Callable | Can be a function pointer, lambda, or functor |
| Forgetting `join()` | The `std::thread` destructor calls `std::terminate` if the thread is joinable |

### Practice

1. 🌟 **Fill:** Create a thread that runs the `increment` function and join it. After the thread completes, `counter` should be `1`.

```cpp,editable
#include <iostream>
#include <thread>
#include <cassert>

int counter = 0;

void increment() {
    counter = 1;
}

int main() {
    std::thread t(__);
    __;

    assert(counter == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Fill:** Create a thread using a **lambda** that sets `result` to `42`, then join it.

```cpp,editable
#include <iostream>
#include <thread>
#include <cassert>

int main() {
    int result = 0;

    std::thread t(__);
    t.join();

    assert(result == 42);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Passing Arguments to Threads

Arguments are **copied** into the thread by default. To pass by reference, wrap the argument with `std::ref()`. You can also **move** objects into the thread for efficient transfer of ownership.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <string>

void greet(std::string msg) {
    std::cout << msg << std::endl;
}

void add_to(int& value, int amount) {
    value += amount;
}

int main() {
    // Pass by value (string is copied into the thread)
    std::thread t1(greet, "Hello");
    t1.join();

    // Pass by reference using std::ref
    int x = 10;
    std::thread t2(add_to, std::ref(x), 5);
    t2.join();
    std::cout << x << std::endl;  // 15
    return 0;
}
```

| Method | Syntax | When to use |
|--------|--------|-------------|
| By value | `std::thread t(func, arg)` | Default; safe, each thread has its own copy |
| By reference | `std::thread t(func, std::ref(arg))` | When the thread needs to modify the caller's variable |
| By move | `std::thread t(func, std::move(arg))` | Transfer ownership of a resource (e.g., `unique_ptr`) |

### Practice

3. 🌟🌟 **Fix:** The thread should modify `value` in the caller, but without `std::ref` the argument is copied. Fix it so `value` becomes `15`.

```cpp,editable
#include <iostream>
#include <thread>
#include <cassert>

void add_to(int& val, int amount) {
    val += amount;
}

int main() {
    int value = 10;

    // BUG: value is copied, not passed by reference
    std::thread t(add_to, value, 5);
    t.join();

    assert(value == 15);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Detach & Thread Lifecycle

A **detached** thread runs independently — the caller doesn't wait for it. Once detached, you cannot join the thread. Use `joinable()` to check whether a thread can still be joined. `thread_local` storage gives each thread its own copy of a variable.

### How it works

```cpp
#include <iostream>
#include <thread>

thread_local int tls_counter = 0;

void count_up(int id) {
    for (int i = 0; i < 3; ++i)
        ++tls_counter;
    // Each thread has its own tls_counter, so this always prints 3
    std::cout << "Thread " << id << " counter: " << tls_counter << std::endl;
}

int main() {
    std::thread t1(count_up, 1);
    std::thread t2(count_up, 2);

    // joinable() returns true if the thread hasn't been joined or detached
    std::cout << "t1 joinable: " << t1.joinable() << std::endl;  // true

    t1.join();
    t2.join();

    // After join, no longer joinable
    std::cout << "t1 joinable: " << t1.joinable() << std::endl;  // false

    // Main thread has its own tls_counter, still 0
    std::cout << "Main counter: " << tls_counter << std::endl;    // 0
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `t.detach()` | Thread runs independently; no way to join later |
| `t.joinable()` | Returns `true` if the thread can be joined (not yet joined/detached) |
| `thread_local` | Each thread gets its own independent copy of the variable |
| Detached thread caution | Must not access local variables from the launching scope (they may be destroyed) |

### Practice

4. 🌟🌟 **Fill:** Check if threads are `joinable()` at the right times, and join them.

```cpp,editable
#include <iostream>
#include <thread>
#include <cassert>
#include <atomic>

std::atomic<int> total{0};

void work() {
    total += 1;
}

int main() {
    std::thread t1(work);
    std::thread t2(work);

    // Before joining, threads should be joinable
    assert(t1.__() == true);
    assert(t2.__() == true);

    t1.join();
    t2.join();

    // After joining, threads should NOT be joinable
    assert(t1.__() == false);
    assert(t2.__() == false);

    assert(total == 2);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Use `thread_local` storage so that each thread has its own counter. Each thread increments its local counter 3 times. Use an atomic variable to collect the final values from each thread.

```cpp,editable
#include <iostream>
#include <thread>
#include <cassert>
#include <atomic>

__ int local_counter = 0;

std::atomic<int> global_sum{0};

void count_and_report() {
    for (int i = 0; i < 3; ++i)
        ++local_counter;
    // Each thread's local_counter is 3
    global_sum += local_counter;
}

int main() {
    std::thread t1(count_and_report);
    std::thread t2(count_and_report);

    t1.join();
    t2.join();

    // Two threads, each counted to 3
    assert(global_sum == 6);

    // Main thread's local_counter is untouched
    assert(local_counter == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
