# Map

`std::map` is an associative container that stores **sorted key-value pairs** with unique keys. It is implemented as a balanced binary search tree (typically red-black), giving O(log n) lookup, insert, and erase.

## Map Basics

You can create a map with brace initialisation, access values with `[]`, and query the number of entries with `size()`. The `[]` operator inserts a default-constructed value if the key does not exist.

### How it works

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob",   25},
        {"Carol", 28}
    };

    std::cout << "Size: " << ages.size()    << std::endl;  // 3
    std::cout << "Bob:  " << ages["Bob"]    << std::endl;  // 25
    ages["Dave"] = 35;  // insert new entry
    std::cout << "Size: " << ages.size()    << std::endl;  // 4
    return 0;
}
```

| Operation | Effect | Complexity |
|-----------|--------|------------|
| `m[key]` | Returns reference; **inserts default** if key missing | O(log n) |
| `m.at(key)` | Returns reference; **throws** if key missing | O(log n) |
| `m.size()` | Number of key-value pairs | O(1) |
| `m.insert({k, v})` | Inserts only if key doesn't exist | O(log n) |

### Practice

1. 🌟 **Fill:** What is the size and Bob's age?

```cpp,editable
#include <iostream>
#include <map>
#include <string>
#include <cassert>

int main() {
    std::map<std::string, int> ages = {
        {"Alice", 30},
        {"Bob",   25},
        {"Carol", 28}
    };

    assert(ages.size() == __);
    assert(ages["Bob"] == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Checking Key Existence

Using `[]` to check for a key is a common bug — it silently inserts a default entry. Use `find()` or `count()` instead for safe lookups.

### How it works

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};

    // BAD: [] inserts "Charlie" with value 0
    // if (scores["Charlie"] == 0) { ... }

    // GOOD: find returns end() if key not found
    auto it = scores.find("Charlie");
    if (it != scores.end())
        std::cout << "Found: " << it->second << std::endl;
    else
        std::cout << "Not found" << std::endl;

    // Also OK: count returns 0 or 1
    std::cout << "Alice exists: " << scores.count("Alice") << std::endl;  // 1
    return 0;
}
```

| Method | Returns | Side effect |
|--------|---------|-------------|
| `m[key]` | Reference to value | **Creates entry** if key absent |
| `m.find(key)` | Iterator (`end()` if absent) | None |
| `m.count(key)` | `0` or `1` | None |
| `m.contains(key)` (C++20) | `bool` | None |

### Practice

2. 🌟🌟 **Fix:** The code uses `[]` to check if "Charlie" exists, which accidentally creates the entry. Use `find()` instead.

```cpp,editable
#include <iostream>
#include <map>
#include <string>
#include <cassert>

int main() {
    std::map<std::string, int> scores = {{"Alice", 90}, {"Bob", 85}};

    // BUG: this creates a "Charlie" entry with value 0!
    if (scores["Charlie"] == 0) {
        std::cout << "Charlie not found" << std::endl;
    }

    // After the bug, Charlie is now in the map
    assert(scores.size() == 2);  // Should still be 2!

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Iterating

Maps are iterated in **sorted key order**. Each element is a `std::pair<const Key, Value>`. C++17 structured bindings make this clean.

### How it works

```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> ages = {
        {"Alice", 30}, {"Bob", 25}, {"Carol", 28}
    };

    // Structured bindings (C++17)
    for (const auto& [name, age] : ages)
        std::cout << name << ": " << age << std::endl;
    // Alice: 30, Bob: 25, Carol: 28 (sorted by key)

    // Traditional pair access
    for (const auto& pair : ages)
        std::cout << pair.first << " is " << pair.second << std::endl;
    return 0;
}
```

| Access style | Syntax |
|-------------|--------|
| Structured bindings | `for (const auto& [k, v] : m)` |
| Pair access | `pair.first` (key), `pair.second` (value) |
| Iterator | `it->first`, `it->second` |

### Practice

3. 🌟🌟 **Fill:** Iterate the map and sum all the values using `pair.second`.

```cpp,editable
#include <iostream>
#include <map>
#include <string>
#include <cassert>

int main() {
    std::map<std::string, int> scores = {
        {"Alice", 90}, {"Bob", 85}, {"Carol", 92}
    };

    int total = 0;
    for (const auto& __ : scores) {
        total += __;
    }

    assert(total == 267);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Unordered Map & Frequency Counting

`std::unordered_map` uses a **hash table** for O(1) average lookup. A classic use case is counting frequencies: `freq[item]++` increments the count, auto-inserting 0 for new keys.

### How it works

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

int main() {
    std::string text = "hello world";

    std::unordered_map<char, int> freq;
    for (char c : text)
        freq[c]++;

    for (const auto& [ch, count] : freq)
        std::cout << "'" << ch << "': " << count << std::endl;
    return 0;
}
```

| Feature | `std::map` | `std::unordered_map` |
|---------|-----------|---------------------|
| Order | Sorted by key | No guaranteed order |
| Lookup | O(log n) | O(1) average |
| Underlying | Balanced BST | Hash table |
| Key requirement | `operator<` | `std::hash` + `operator==` |

### Practice

4. 🌟🌟 **Fill:** Count the frequency of each character in the string using `freq[c]++`.

```cpp,editable
#include <iostream>
#include <unordered_map>
#include <string>
#include <cassert>

int main() {
    std::string text = "abracadabra";

    std::unordered_map<char, int> freq;
    for (char c : text) {
        __  // increment the count for character c
    }

    assert(freq['a'] == 5);
    assert(freq['b'] == 2);
    assert(freq['r'] == 2);
    assert(freq['c'] == 1);
    assert(freq['d'] == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
