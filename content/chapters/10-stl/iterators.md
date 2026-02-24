# Iterators

Iterators are the glue between containers and algorithms in the STL. They provide a uniform way to traverse elements regardless of the underlying container. Understanding iterator categories, utilities, and reverse iterators is essential for writing generic, efficient C++ code.

## Iterator Basics

An iterator is a generalised pointer. You get one from `begin()` (first element) and compare against `end()` (one past the last). You dereference with `*it` to access the element and advance with `++it`. A range-based for loop is syntactic sugar for this pattern.

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};

    // Manual iterator loop
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";  // 10 20 30 40 50
    }
    std::cout << std::endl;

    // Equivalent range-based for loop
    for (int val : v) {
        std::cout << val << " ";  // 10 20 30 40 50
    }
    std::cout << std::endl;

    // Modify through iterator
    for (auto it = v.begin(); it != v.end(); ++it) {
        *it *= 2;
    }
    // v is now {20, 40, 60, 80, 100}
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `v.begin()` | Iterator to the first element |
| `v.end()` | Iterator **past** the last element (sentinel) |
| `*it` | Dereference — access the element the iterator points to |
| `++it` | Advance to the next element |
| Range-for equivalence | `for (auto x : v)` is equivalent to `for (auto it = v.begin(); it != v.end(); ++it)` |

### Practice

1. 🌟 **Fill:** Use an iterator loop to sum all elements in the vector.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};
    int sum = 0;

    for (auto it = v.__; it != v.__; __) {
        sum += *it;
    }

    assert(sum == 150);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Iterator Categories

The STL defines five iterator categories, each with increasing capabilities. Algorithms require a minimum category — for example, `std::sort` needs random access iterators, which is why you cannot sort a `std::list` with `std::sort`.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

int main() {
    // Random access: vector — supports +, -, [], <
    std::vector<int> vec = {3, 1, 2};
    std::sort(vec.begin(), vec.end());  // OK: random access
    auto it = vec.begin();
    it += 2;  // jump forward by 2
    std::cout << *it << std::endl;  // 3

    // Bidirectional: list — supports ++ and --
    std::list<int> lst = {3, 1, 2};
    // std::sort(lst.begin(), lst.end());  // ERROR: not random access
    lst.sort();  // list has its own sort member function
    auto lit = lst.end();
    --lit;  // OK: bidirectional
    std::cout << *lit << std::endl;  // 3

    // Forward: forward_list — supports ++ only
    std::forward_list<int> fl = {3, 1, 2};
    fl.sort();
    auto fit = fl.begin();
    ++fit;  // OK: forward
    // --fit;  // ERROR: cannot go backwards
    std::cout << *fit << std::endl;  // 2
    return 0;
}
```

| Category | Operations | Example containers |
|----------|-----------|-------------------|
| Input | `*it` (read), `++it` | `std::istream_iterator` |
| Output | `*it` (write), `++it` | `std::ostream_iterator` |
| Forward | Input + multi-pass | `std::forward_list`, `std::unordered_set` |
| Bidirectional | Forward + `--it` | `std::list`, `std::set`, `std::map` |
| Random Access | Bidirectional + `it + n`, `it[n]`, `it1 - it2` | `std::vector`, `std::deque`, `std::array` |

### Practice

2. 🌟🌟 **Fill:** Demonstrate random access vs. bidirectional iterators. Use `+` with the vector iterator and `--` with the list iterator.

```cpp,editable
#include <iostream>
#include <vector>
#include <list>
#include <cassert>

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50};
    std::list<int> lst = {10, 20, 30, 40, 50};

    // Random access: jump directly to the 3rd element (index 2)
    auto vit = vec.begin() __ __;  // use + to advance by 2
    assert(*vit == 30);

    // Bidirectional: go to end, then back up by one to reach last element
    auto lit = lst.end();
    __  // decrement once to reach the last element
    assert(*lit == 50);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Iterator Utilities

The `<iterator>` header provides utility functions that work with all iterator categories: `std::advance` moves an iterator, `std::distance` counts elements between two iterators, `std::next` and `std::prev` return a new iterator without modifying the original.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <iterator>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};

    auto it = v.begin();
    std::advance(it, 3);  // moves it forward by 3
    std::cout << *it << std::endl;  // 40

    auto it2 = std::next(v.begin(), 2);  // returns iterator at position 2
    std::cout << *it2 << std::endl;  // 30

    auto it3 = std::prev(v.end());  // one before end = last element
    std::cout << *it3 << std::endl;  // 50

    auto dist = std::distance(v.begin(), v.end());
    std::cout << "Distance: " << dist << std::endl;  // 5

    // Works with bidirectional iterators too
    std::list<int> lst = {1, 2, 3, 4, 5};
    auto lit = lst.begin();
    std::advance(lit, 2);  // moves lit forward by 2
    std::cout << *lit << std::endl;  // 3
    return 0;
}
```

| Utility | Effect | Works with |
|---------|--------|-----------|
| `std::advance(it, n)` | Moves `it` by `n` positions (modifies `it`) | All iterator categories |
| `std::next(it, n)` | Returns new iterator `n` positions ahead (does not modify `it`) | Forward+ |
| `std::prev(it, n)` | Returns new iterator `n` positions back (does not modify `it`) | Bidirectional+ |
| `std::distance(first, last)` | Returns number of steps from `first` to `last` | All iterator categories |

### Practice

3. 🌟🌟 **Fill:** Use `std::advance`, `std::next`, `std::prev`, and `std::distance` to navigate a list.

```cpp,editable
#include <iostream>
#include <list>
#include <iterator>
#include <cassert>

int main() {
    std::list<int> lst = {10, 20, 30, 40, 50};

    // Use std::distance to find the size
    auto size = std::distance(lst.__, lst.__);
    assert(size == 5);

    // Use std::next to get the 3rd element (index 2) without modifying begin
    auto it = std::next(lst.begin(), __);
    assert(*it == 30);

    // Use std::prev to get the last element
    auto last = std::prev(lst.__);
    assert(*last == 50);

    // Use std::advance to move it forward by 2 (from 30 to 50)
    std::advance(it, __);
    assert(*it == 50);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Reverse Iterators

`rbegin()` / `rend()` provide **reverse iterators** that traverse a container from back to front. `cbegin()` / `cend()` (and `crbegin()` / `crend()`) return **const iterators** that prevent modification of elements.

### How it works

```cpp
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Reverse iteration
    std::cout << "Reverse: ";
    for (auto rit = v.rbegin(); rit != v.rend(); ++rit) {
        std::cout << *rit << " ";  // 5 4 3 2 1
    }
    std::cout << std::endl;

    // Const iterator — prevents modification
    for (auto cit = v.cbegin(); cit != v.cend(); ++cit) {
        std::cout << *cit << " ";
        // *cit = 10;  // ERROR: cannot modify through const iterator
    }
    std::cout << std::endl;

    // Const reverse iterator
    for (auto crit = v.crbegin(); crit != v.crend(); ++crit) {
        std::cout << *crit << " ";  // 5 4 3 2 1
    }
    std::cout << std::endl;
    return 0;
}
```

| Iterator type | Begin | End | Direction | Modifiable |
|--------------|-------|-----|-----------|-----------|
| Normal | `begin()` | `end()` | Forward | Yes |
| Const | `cbegin()` | `cend()` | Forward | No |
| Reverse | `rbegin()` | `rend()` | Backward | Yes |
| Const reverse | `crbegin()` | `crend()` | Backward | No |

### Practice

4. 🌟🌟 **Fill:** Use reverse iterators to fill a vector with elements in reverse order from another vector.

```cpp,editable
#include <iostream>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> src = {1, 2, 3, 4, 5};
    std::vector<int> dst;

    // Use reverse iterators to push elements in reverse order
    for (auto rit = src.__; rit != src.__; ++rit) {
        dst.push_back(*rit);
    }

    assert(dst.size() == 5);
    assert(dst[0] == 5);
    assert(dst[1] == 4);
    assert(dst[2] == 3);
    assert(dst[3] == 2);
    assert(dst[4] == 1);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Use `cbegin()` / `cend()` to find the maximum element without modifying the container, and use `rbegin()` / `rend()` to find the position of the last even number.

```cpp,editable
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

int main() {
    std::vector<int> v = {3, 7, 2, 8, 4, 1, 6};

    // Find the max using const iterators (element cannot be modified)
    auto max_it = std::max_element(v.__, v.__);
    assert(*max_it == 8);

    // Find the last even number using reverse iterators
    auto rit = std::find_if(v.__, v.__, [](int x) {
        return __ ;  // check if x is even
    });
    assert(*rit == 6);

    // Convert reverse iterator to forward distance from begin
    // std::distance from rbegin to rit gives position from the back
    auto pos_from_end = std::distance(v.rbegin(), rit);
    assert(pos_from_end == 0);  // 6 is the last element

    // Find the second-to-last even number
    ++rit;  // advance the reverse iterator
    rit = std::find_if(rit, v.rend(), [](int x) { return x % 2 == 0; });
    assert(*rit == __);  // which even number comes before 6 from the back?

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
