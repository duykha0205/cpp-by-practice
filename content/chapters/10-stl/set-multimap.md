# Set, Multiset, Multimap & Unordered Variants

Beyond `std::map`, the STL provides several other associative containers. `std::set` stores unique sorted keys, `std::unordered_set` offers hash-based O(1) lookups, and `std::multimap` / `std::multiset` allow duplicate keys. Choosing the right container depends on whether you need ordering, uniqueness, and the performance characteristics you require.

## std::set

`std::set` stores **unique elements** in sorted order, implemented as a balanced binary search tree. It provides O(log n) insertion, lookup, and deletion. Duplicate insertions are silently ignored.

### How it works

```cpp
#include <iostream>
#include <set>

int main() {
    std::set<int> s = {3, 1, 4, 1, 5, 9, 2, 6};

    // Duplicates are ignored — only unique values stored
    std::cout << "Size: " << s.size() << std::endl;  // 7

    // Elements are always in sorted order
    for (int x : s)
        std::cout << x << " ";  // 1 2 3 4 5 6 9
    std::cout << std::endl;

    // Lookup operations
    std::cout << "Count 4: " << s.count(4) << std::endl;  // 1
    std::cout << "Count 7: " << s.count(7) << std::endl;  // 0

    auto it = s.find(5);
    if (it != s.end())
        std::cout << "Found: " << *it << std::endl;  // Found: 5

    // Erase an element
    s.erase(3);
    std::cout << "Size after erase: " << s.size() << std::endl;  // 6
    return 0;
}
```

| Operation | Effect | Complexity |
|-----------|--------|------------|
| `s.insert(val)` | Adds element if not already present; returns `pair<iterator, bool>` | O(log n) |
| `s.find(val)` | Returns iterator to element, or `end()` if not found | O(log n) |
| `s.count(val)` | Returns 0 or 1 | O(log n) |
| `s.erase(val)` | Removes element if present | O(log n) |
| Iteration order | Always sorted (ascending by default) | — |

### Practice

1. 🌟 **Fill:** Insert elements into a set and verify that duplicates are rejected.

```cpp,editable
#include <iostream>
#include <set>
#include <cassert>

int main() {
    std::set<int> s;
    s.insert(5);
    s.insert(3);
    s.insert(5);  // duplicate — ignored
    s.insert(1);
    s.insert(3);  // duplicate — ignored

    assert(s.size() == __);  // how many unique elements?
    assert(s.count(5) == __);  // is 5 in the set?
    assert(s.count(2) == __);  // is 2 in the set?

    // The first element (smallest) should be:
    assert(*s.begin() == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Fill:** Use `find()` and `erase()` to remove a specific element from the set, then verify the remaining elements.

```cpp,editable
#include <iostream>
#include <set>
#include <cassert>

int main() {
    std::set<std::string> fruits = {"apple", "banana", "cherry", "date"};

    assert(fruits.size() == 4);

    // Find and erase "banana"
    auto it = fruits.find(__);  // find "banana"
    if (it != fruits.end()) {
        fruits.erase(__);  // erase using the iterator
    }

    assert(fruits.size() == 3);
    assert(fruits.count("banana") == 0);
    assert(fruits.count("apple") == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::unordered_set

`std::unordered_set` uses a **hash table** instead of a tree. Lookups, insertions, and deletions are O(1) on average. Use it when you do not need sorted order and want maximum lookup speed.

### How it works

```cpp
#include <iostream>
#include <unordered_set>
#include <string>

int main() {
    std::unordered_set<std::string> words;
    words.insert("hello");
    words.insert("world");
    words.insert("hello");  // duplicate — ignored

    std::cout << "Size: " << words.size() << std::endl;  // 2

    if (words.count("hello"))
        std::cout << "Found hello" << std::endl;

    // Iteration order is NOT guaranteed
    for (const auto& w : words)
        std::cout << w << " ";
    std::cout << std::endl;
    return 0;
}
```

| Feature | `std::set` | `std::unordered_set` |
|---------|-----------|---------------------|
| Order | Sorted | No guaranteed order |
| Lookup | O(log n) | O(1) average |
| Underlying | Balanced BST | Hash table |
| Key requirement | `operator<` | `std::hash` + `operator==` |
| Worst case | O(log n) | O(n) — hash collisions |

### Practice

3. 🌟🌟 **Complete:** Use an `unordered_set` to remove duplicates from a vector while counting the unique elements.

```cpp,editable
#include <iostream>
#include <unordered_set>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> numbers = {1, 3, 5, 3, 1, 7, 5, 9, 7};

    std::unordered_set<int> unique_nums;
    for (int n : numbers) {
        __ // insert n into the unordered_set
    }

    assert(unique_nums.size() == 5);  // {1, 3, 5, 7, 9}
    assert(unique_nums.count(3) == __);
    assert(unique_nums.count(4) == __);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::multimap & std::multiset

`std::multiset` allows **duplicate elements**, and `std::multimap` allows **duplicate keys**. The `equal_range()` function returns a pair of iterators marking all entries with the same key, making it easy to iterate over duplicates.

### How it works

```cpp
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    // multiset: allows duplicate elements
    std::multiset<int> ms = {1, 2, 2, 3, 3, 3};
    std::cout << "Count of 3: " << ms.count(3) << std::endl;  // 3

    // multimap: allows duplicate keys
    std::multimap<std::string, int> grades;
    grades.insert({"Alice", 90});
    grades.insert({"Alice", 85});
    grades.insert({"Bob", 92});
    grades.insert({"Alice", 88});

    std::cout << "Alice's grades: ";
    auto [begin, end] = grades.equal_range("Alice");
    for (auto it = begin; it != end; ++it)
        std::cout << it->second << " ";  // 90 85 88
    std::cout << std::endl;

    std::cout << "Total entries: " << grades.size() << std::endl;  // 4
    return 0;
}
```

| Container | Duplicates | Order | Lookup |
|-----------|-----------|-------|--------|
| `std::set` | No | Sorted | O(log n) |
| `std::multiset` | Yes | Sorted | O(log n) |
| `std::map` | No (unique keys) | Sorted by key | O(log n) |
| `std::multimap` | Yes (duplicate keys) | Sorted by key | O(log n) |
| `equal_range(key)` | Returns `pair<iterator, iterator>` marking all matches | — | O(log n) |

### Practice

4. 🌟🌟🌟 **Fill:** Use a `multimap` to store student scores, then use `equal_range` to compute Alice's average.

```cpp,editable
#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <cmath>

int main() {
    std::multimap<std::string, int> scores;
    scores.insert({"Alice", 90});
    scores.insert({"Bob", 80});
    scores.insert({"Alice", 85});
    scores.insert({"Alice", 92});
    scores.insert({"Bob", 88});

    assert(scores.count("Alice") == __);  // how many entries for Alice?

    // Compute Alice's average using equal_range
    auto [begin, end] = scores.equal_range(__);  // find Alice's range
    int sum = 0;
    int count = 0;
    for (auto it = begin; it != end; ++it) {
        sum += it->second;
        count++;
    }
    double avg = static_cast<double>(sum) / count;

    assert(std::abs(avg - 89.0) < 0.01);
    assert(count == 3);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Use a `multiset` to count how many times each element appears, then erase all occurrences of a specific value.

```cpp,editable
#include <iostream>
#include <set>
#include <cassert>

int main() {
    std::multiset<int> ms = {1, 2, 3, 2, 4, 2, 3, 5};

    assert(ms.size() == 8);
    assert(ms.count(2) == __);  // how many 2s?
    assert(ms.count(3) == __);  // how many 3s?

    // Erase ALL occurrences of 2
    ms.erase(__);  // pass the value to erase all matching elements

    assert(ms.count(2) == 0);
    assert(ms.size() == __);  // how many elements remain?

    // Erase only ONE occurrence of 3 using an iterator
    auto it = ms.find(3);
    if (it != ms.end()) {
        ms.erase(it);
    }

    assert(ms.count(3) == 1);  // one 3 left

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
