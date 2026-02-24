# Atomics & Condition Variables

Atomic operations provide **lock-free** thread safety for simple values, while condition variables let threads **wait** for a specific condition to become true. Together with mutexes, these are the building blocks for efficient concurrent patterns like producer-consumer queues.

> **Note:** Compile all exercises in this file with the `-pthread` flag (e.g., `g++ -std=c++17 -pthread main.cpp`).

## std::atomic

`std::atomic<T>` provides thread-safe operations on a single value without needing a mutex. Operations like `load()`, `store()`, `fetch_add()`, and `compare_exchange_strong()` are guaranteed to be indivisible — no other thread can see a half-written state.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter{0};

void increment(int times) {
    for (int i = 0; i < times; ++i)
        counter.fetch_add(1);  // atomic increment
}

int main() {
    std::thread t1(increment, 1000);
    std::thread t2(increment, 1000);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter.load() << std::endl;  // always 2000
    return 0;
}
```

| Operation | Detail |
|-----------|--------|
| `a.load()` | Atomically read the current value |
| `a.store(val)` | Atomically write a new value |
| `a.fetch_add(n)` | Atomically add `n` and return the old value |
| `a.compare_exchange_strong(expected, desired)` | If value == expected, set to desired and return true; else update expected and return false |
| `a++`, `a += n` | Shorthand for `fetch_add(1)` and `fetch_add(n)` |

### Practice

1. 🌟🌟 **Fill:** Use `std::atomic` operations to safely increment a counter from two threads. Use `store()` to initialize, `fetch_add()` to increment, and `load()` to read the final value.

```cpp,editable
#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>

int main() {
    std::atomic<int> counter{0};

    auto increment = [&counter]() {
        for (int i = 0; i < 1000; ++i)
            counter.__(1);
    };

    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    assert(counter.__() == 2000);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟🌟 **Complete:** Use `compare_exchange_strong` to implement a lock-free update: double the counter's value exactly once from each thread. Each thread reads the current value, computes double, and atomically swaps if unchanged.

```cpp,editable
#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>

std::atomic<int> value{1};

void double_value() {
    int expected = value.load();
    int desired = expected * 2;
    // Keep trying until we succeed
    while (!value.__(expected, desired)) {
        // expected is updated automatically on failure
        desired = expected * 2;
    }
}

int main() {
    std::thread t1(double_value);
    std::thread t2(double_value);

    t1.join();
    t2.join();

    // Started at 1, doubled twice: 1 -> 2 -> 4
    assert(value.load() == 4);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::condition_variable

A condition variable lets a thread **sleep** until another thread **notifies** it that something has changed. Always use it with a `std::mutex` and a **predicate** (a condition to check), because **spurious wakeups** can happen — the thread may wake up even when no notification was sent.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    // Wait until ready is true (handles spurious wakeups)
    cv.wait(lock, [] { return ready; });
    std::cout << "Worker proceeding!" << std::endl;
}

int main() {
    std::thread t(worker);

    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // wake up the worker

    t.join();
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `cv.wait(lock, pred)` | Releases lock and sleeps; re-acquires lock when notified and `pred()` is true |
| `cv.notify_one()` | Wakes one waiting thread |
| `cv.notify_all()` | Wakes all waiting threads |
| Spurious wakeup | Thread may wake without notification; the predicate re-checks the condition |
| Requires `unique_lock` | `condition_variable` needs `unique_lock` (not `lock_guard`) for its wait operations |

### Practice

3. 🌟🌟 **Fill:** Use a condition variable so the main thread signals the worker to proceed. Fill in the `wait` call with the correct predicate and the `notify` call.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <atomic>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;
std::atomic<bool> worker_done{false};

void worker() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, __);
    worker_done = true;
}

int main() {
    std::thread t(worker);

    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.__();

    t.join();

    assert(worker_done == true);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Producer-Consumer Pattern

The **producer-consumer** pattern is a classic concurrency design: one thread produces data and pushes it into a shared queue, while another thread consumes data from the queue. A mutex protects the queue, and a condition variable signals when new data is available.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv;
bool finished = false;

void producer() {
    for (int i = 1; i <= 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(i);
        }
        cv.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_one();
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !buffer.empty() || finished; });

        while (!buffer.empty()) {
            std::cout << "Consumed: " << buffer.front() << std::endl;
            buffer.pop();
        }

        if (finished && buffer.empty())
            break;
    }
}

int main() {
    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();
    return 0;
}
```

| Component | Role |
|-----------|------|
| `std::queue` | Shared buffer between producer and consumer |
| `std::mutex` | Protects access to the queue and `finished` flag |
| `std::condition_variable` | Consumer sleeps until data is available or producer is done |
| `finished` flag | Tells the consumer to exit after processing remaining items |

### Practice

4. 🌟🌟🌟 **Fill:** Complete the producer-consumer pattern. The producer pushes values 1-5, then sets `finished`. The consumer collects all values into a vector.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <cassert>
#include <algorithm>
#include <numeric>

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv;
bool finished = false;

void producer() {
    for (int i = 1; i <= 5; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(i);
        }
        cv.__();
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.__();
}

void consumer(std::vector<int>& results) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return __ || __; });

        while (!buffer.empty()) {
            results.push_back(buffer.front());
            buffer.pop();
        }

        if (finished && buffer.empty())
            break;
    }
}

int main() {
    std::vector<int> results;

    std::thread prod(producer);
    std::thread cons(consumer, std::ref(results));

    prod.join();
    cons.join();

    std::sort(results.begin(), results.end());
    assert(results.size() == 5);

    int sum = std::accumulate(results.begin(), results.end(), 0);
    assert(sum == 15);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Build a producer-consumer where **two producers** each push 3 values and **one consumer** collects them all. Use `std::atomic<int>` to track how many producers have finished.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <cassert>
#include <atomic>
#include <algorithm>
#include <numeric>

std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv;
__ producers_done{0};
const int NUM_PRODUCERS = 2;

void producer(int start) {
    for (int i = start; i < start + 3; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(i);
        }
        cv.notify_one();
    }
    producers_done++;
    cv.notify_one();
}

void consumer(std::vector<int>& results) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {
            return !buffer.empty() || producers_done == NUM_PRODUCERS;
        });

        while (!buffer.empty()) {
            results.push_back(buffer.front());
            buffer.pop();
        }

        if (producers_done == NUM_PRODUCERS && buffer.empty())
            break;
    }
}

int main() {
    std::vector<int> results;

    std::thread p1(producer, 1);
    std::thread p2(producer, 4);
    std::thread cons(consumer, std::ref(results));

    p1.join();
    p2.join();
    cons.join();

    std::sort(results.begin(), results.end());
    assert(results.size() == 6);

    int sum = std::accumulate(results.begin(), results.end(), 0);
    // 1+2+3+4+5+6 = 21
    assert(sum == 21);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
