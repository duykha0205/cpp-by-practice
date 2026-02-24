# Mutexes & Locks

When multiple threads access shared data, you need **mutual exclusion** to prevent race conditions. A mutex ensures that only one thread can access a critical section at a time. C++ provides several lock wrappers (lock_guard, unique_lock, scoped_lock) that follow RAII to guarantee the mutex is always released.

> **Note:** Compile all exercises in this file with the `-pthread` flag (e.g., `g++ -std=c++17 -pthread main.cpp`).

## std::mutex

`std::mutex` provides basic mutual exclusion. Call `lock()` before accessing shared data and `unlock()` after. If you forget to unlock (e.g., due to an exception), other threads will block forever — that's why RAII wrappers are preferred.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <mutex>

int counter = 0;
std::mutex mtx;

void increment(int times) {
    for (int i = 0; i < times; ++i) {
        mtx.lock();
        ++counter;       // only one thread at a time
        mtx.unlock();
    }
}

int main() {
    std::thread t1(increment, 1000);
    std::thread t2(increment, 1000);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;  // always 2000
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::mutex` | Provides exclusive ownership — only one thread can hold it |
| `mtx.lock()` | Blocks until the mutex is acquired |
| `mtx.unlock()` | Releases the mutex so other threads can acquire it |
| Race condition | Without the mutex, two threads incrementing the same variable may lose updates |

### Practice

1. 🌟🌟 **Fix:** The code has a race condition — two threads increment `counter` without protection. Add `mutex.lock()` and `mutex.unlock()` around the increment.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>

int counter = 0;
std::mutex mtx;

void increment(int times) {
    for (int i = 0; i < times; ++i) {
        // BUG: no mutex protection — race condition
        ++counter;
    }
}

int main() {
    std::thread t1(increment, 1000);
    std::thread t2(increment, 1000);

    t1.join();
    t2.join();

    assert(counter == 2000);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## RAII Lock Guards

`std::lock_guard` locks the mutex on construction and unlocks it on destruction — no matter how the scope exits (normal return or exception). `std::unique_lock` is more flexible: it supports deferred locking, manual unlock/re-lock, and is required by condition variables.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
std::vector<int> shared_data;

void add_values(int start, int count) {
    std::lock_guard<std::mutex> lock(mtx);
    // Mutex is held for the entire scope
    for (int i = start; i < start + count; ++i)
        shared_data.push_back(i);
    // Mutex automatically released here
}

void add_with_unique_lock(int value) {
    std::unique_lock<std::mutex> lock(mtx);
    shared_data.push_back(value);
    lock.unlock();  // can manually unlock early
    // ... do other non-critical work ...
}

int main() {
    std::thread t1(add_values, 0, 3);
    std::thread t2(add_values, 3, 3);

    t1.join();
    t2.join();

    std::cout << "Size: " << shared_data.size() << std::endl;  // 6
    return 0;
}
```

| Lock type | Feature |
|-----------|---------|
| `std::lock_guard<std::mutex>` | Simple RAII lock; locks on construction, unlocks on destruction |
| `std::unique_lock<std::mutex>` | Flexible: supports deferred locking, manual unlock, condition variables |
| Both | Exception-safe — mutex always released when scope exits |

### Practice

2. 🌟🌟 **Fix:** Replace raw `lock()`/`unlock()` calls with `std::lock_guard` for exception safety.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>

int counter = 0;
std::mutex mtx;

void increment(int times) {
    for (int i = 0; i < times; ++i) {
        // BUG: raw lock/unlock is not exception-safe
        // Replace with std::lock_guard
        mtx.lock();
        ++counter;
        mtx.unlock();
    }
}

int main() {
    std::thread t1(increment, 1000);
    std::thread t2(increment, 1000);

    t1.join();
    t2.join();

    assert(counter == 2000);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

3. 🌟🌟🌟 **Complete:** Use `std::unique_lock` to protect pushing to a shared vector. Unlock early after the critical section so the remaining work runs without holding the mutex.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>
#include <atomic>

std::mutex mtx;
std::vector<int> shared_vec;
std::atomic<int> extra_work_done{0};

void add_value(int value) {
    __ lock(mtx);
    shared_vec.push_back(value);
    __;  // unlock early

    // Simulate non-critical work after releasing the mutex
    extra_work_done += 1;
}

int main() {
    std::thread t1(add_value, 10);
    std::thread t2(add_value, 20);

    t1.join();
    t2.join();

    assert(shared_vec.size() == 2);
    assert(extra_work_done == 2);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::scoped_lock

C++17 introduced `std::scoped_lock` which can lock **multiple mutexes** simultaneously using a deadlock-avoidance algorithm. This replaces the older pattern of `std::lock()` followed by `std::lock_guard` with `std::adopt_lock`.

### How it works

```cpp
#include <iostream>
#include <thread>
#include <mutex>

struct Account {
    std::mutex mtx;
    int balance;
};

void transfer(Account& from, Account& to, int amount) {
    // Locks both mutexes without risk of deadlock
    std::scoped_lock lock(from.mtx, to.mtx);
    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account a{.balance = 100};
    Account b{.balance = 50};

    std::thread t1(transfer, std::ref(a), std::ref(b), 30);
    std::thread t2(transfer, std::ref(b), std::ref(a), 10);

    t1.join();
    t2.join();

    std::cout << "A: " << a.balance << std::endl;  // 80
    std::cout << "B: " << b.balance << std::endl;  // 70
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::scoped_lock lock(m1, m2)` | Locks multiple mutexes atomically; no deadlock |
| Deadlock risk | Two threads locking m1 then m2, and m2 then m1, can deadlock |
| `scoped_lock` fix | Uses `std::lock` internally to acquire all mutexes safely |
| Single mutex | `std::scoped_lock lock(mtx)` works like `lock_guard` |

### Practice

4. 🌟🌟🌟 **Fix:** The `transfer` function locks two mutexes in different orders depending on call order, risking deadlock. Replace with `std::scoped_lock` to lock both safely.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>

struct Account {
    std::mutex mtx;
    int balance;
};

void transfer(Account& from, Account& to, int amount) {
    // BUG: locking in fixed order can deadlock if another thread
    // locks them in reverse order. Use std::scoped_lock instead.
    from.mtx.lock();
    to.mtx.lock();
    from.balance -= amount;
    to.balance += amount;
    to.mtx.unlock();
    from.mtx.unlock();
}

int main() {
    Account a{.balance = 100};
    Account b{.balance = 50};

    std::thread t1(transfer, std::ref(a), std::ref(b), 30);
    std::thread t2(transfer, std::ref(b), std::ref(a), 10);

    t1.join();
    t2.join();

    assert(a.balance == 80);
    assert(b.balance == 70);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟 **Fill:** Use `std::scoped_lock` to protect a shared counter incremented by two threads.

```cpp,editable
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>

int counter = 0;
std::mutex mtx;

void increment(int times) {
    for (int i = 0; i < times; ++i) {
        __ lock(__);
        ++counter;
    }
}

int main() {
    std::thread t1(increment, 1000);
    std::thread t2(increment, 1000);

    t1.join();
    t2.join();

    assert(counter == 2000);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
