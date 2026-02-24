# Template Metaprogramming

Template metaprogramming (TMP) uses the C++ template system to perform **computations at compile time**. The compiler instantiates templates recursively, effectively executing a program during compilation. TMP enables zero-overhead abstractions, compile-time validation, and powerful generic libraries.

> **Note:** All exercises compile with `-std=c++17` or later. GCC 10+, Clang 10+, and MSVC 19.26+ are recommended.

## Compile-Time Computation

The classic TMP technique uses **recursive template instantiation** to compute values at compile time. A primary template handles the recursive case, and a **template specialization** provides the base case to stop recursion.

### How it works

```cpp
#include <iostream>

// Recursive case: Factorial<N> = N * Factorial<N-1>
template<int N>
struct Factorial {
    static constexpr long long value = N * Factorial<N - 1>::value;
};

// Base case: Factorial<0> = 1
template<>
struct Factorial<0> {
    static constexpr long long value = 1;
};

// Fibonacci using the same pattern
template<int N>
struct Fibonacci {
    static constexpr long long value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template<>
struct Fibonacci<0> {
    static constexpr long long value = 0;
};

template<>
struct Fibonacci<1> {
    static constexpr long long value = 1;
};

int main() {
    static_assert(Factorial<5>::value == 120);
    static_assert(Factorial<10>::value == 3628800);

    static_assert(Fibonacci<0>::value == 0);
    static_assert(Fibonacci<1>::value == 1);
    static_assert(Fibonacci<10>::value == 55);

    std::cout << "Factorial<12> = " << Factorial<12>::value << std::endl;  // 479001600
    std::cout << "Fibonacci<15> = " << Fibonacci<15>::value << std::endl;  // 610
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| Primary template | The recursive case; depends on `N-1` (or similar) |
| Full specialization | `template<> struct Foo<0>` — the base case that stops recursion |
| `static constexpr` member | Stores the computed value; available at compile time |
| `static_assert` | Proves computation happens at compile time |
| Instantiation depth | Compilers limit recursion depth (typically 256-1024); keep `N` reasonable |

### Practice

1. 🌟🌟🌟 **Fill:** Complete the `Power` template that computes `Base^Exp` at compile time using recursive template instantiation.

```cpp,editable
#include <iostream>
#include <cassert>

// Recursive case: Power<Base, Exp> = Base * Power<Base, Exp-1>
template<int Base, int Exp>
struct Power {
    static constexpr long long value = __;
};

// Base case: Power<Base, 0> = 1
template<int Base>
struct Power<Base, 0> {
    static constexpr long long value = __;
};

int main() {
    static_assert(Power<2, 0>::value == 1);
    static_assert(Power<2, 10>::value == 1024);
    static_assert(Power<3, 5>::value == 243);
    static_assert(Power<5, 3>::value == 125);

    std::cout << "2^20 = " << Power<2, 20>::value << std::endl;
    assert(Power<2, 20>::value == 1048576);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟🌟 **Complete:** Write a template `GCD` that computes the greatest common divisor of two integers at compile time using the Euclidean algorithm. `GCD<A, B>` = `GCD<B, A % B>` with base case `GCD<A, 0>` = `A`.

```cpp,editable
#include <iostream>
#include <cassert>

// Recursive case: GCD<A, B> = GCD<B, A % B>
template<int A, int B>
struct GCD {
    __;
};

// Base case: GCD<A, 0> = A
template<int A>
struct GCD<A, 0> {
    __;
};

int main() {
    static_assert(GCD<12, 8>::value == 4);
    static_assert(GCD<100, 75>::value == 25);
    static_assert(GCD<17, 13>::value == 1);  // coprime
    static_assert(GCD<48, 18>::value == 6);
    static_assert(GCD<7, 0>::value == 7);

    std::cout << "GCD<252, 105> = " << GCD<252, 105>::value << std::endl;
    assert(GCD<252, 105>::value == 21);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::integral_constant & Type Lists

`std::integral_constant<T, v>` wraps a compile-time value `v` of type `T` into a type. This is the foundation of type-level computation — it lets you treat values as types and manipulate them with template machinery. `std::bool_constant<B>` (alias for `std::integral_constant<bool, B>`) is used extensively in type traits.

### How it works

```cpp
#include <iostream>
#include <type_traits>

// integral_constant wraps a value as a type
using Five = std::integral_constant<int, 5>;
using Ten = std::integral_constant<int, 10>;

// bool_constant for compile-time boolean logic
using True = std::bool_constant<true>;
using False = std::bool_constant<false>;

// A type list: a variadic template that holds types
template<typename... Ts>
struct TypeList {};

// Count the number of types in a TypeList
template<typename List>
struct Length;

template<typename... Ts>
struct Length<TypeList<Ts...>> {
    static constexpr int value = sizeof...(Ts);
};

// Check if a TypeList is empty
template<typename List>
struct IsEmpty : std::bool_constant<Length<List>::value == 0> {};

int main() {
    static_assert(Five::value == 5);
    static_assert(Ten::value == 10);
    static_assert(True::value == true);
    static_assert(False::value == false);

    using MyList = TypeList<int, double, char>;
    static_assert(Length<MyList>::value == 3);
    static_assert(!IsEmpty<MyList>::value);
    static_assert(IsEmpty<TypeList<>>::value);

    std::cout << "Length of TypeList<int, double, char> = "
              << Length<MyList>::value << std::endl;  // 3
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::integral_constant<T, v>` | A type wrapping compile-time value `v` of type `T` |
| `::value` | Access the wrapped value |
| `std::bool_constant<B>` | Alias for `std::integral_constant<bool, B>` |
| `std::true_type` / `std::false_type` | Aliases for `bool_constant<true>` / `bool_constant<false>` |
| `TypeList<Ts...>` | A list of types stored as template parameters |
| `sizeof...(Ts)` | Returns the number of types in a parameter pack |

### Practice

3. 🌟🌟🌟 **Complete:** Implement `Contains` that checks at compile time whether a `TypeList` contains a specific type. Use recursive specialization: peel off the first type and compare, recurse on the rest.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

template<typename... Ts>
struct TypeList {};

// Primary template: Contains<TypeList, T> — is T in the list?
// Base case: empty list — T is not found
template<typename List, typename T>
struct Contains : std::false_type {};

// Recursive case: first type matches T
template<typename T, typename... Rest>
struct Contains<TypeList<T, Rest...>, T> : ___ {};

// Recursive case: first type does NOT match T — check the rest
template<typename First, typename... Rest, typename T>
struct Contains<TypeList<First, Rest...>, T> : ___ {};

int main() {
    using MyList = TypeList<int, double, char, std::string>;

    static_assert(Contains<MyList, int>::value == true);
    static_assert(Contains<MyList, double>::value == true);
    static_assert(Contains<MyList, char>::value == true);
    static_assert(Contains<MyList, float>::value == false);
    static_assert(Contains<TypeList<>, int>::value == false);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Template Template Parameters

A **template template parameter** is a template that takes another template as its parameter. This enables "higher-order" generic programming — you can write a template that is parameterized not just on types or values, but on other templates themselves. This is the basis of policy-based design.

### How it works

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <string>

// A template template parameter: Container is itself a template
template<typename T, template<typename, typename> class Container = std::vector>
class DataStore {
    Container<T, std::allocator<T>> data_;
public:
    void add(const T& item) { data_.push_back(item); }
    size_t size() const { return data_.size(); }
    const T& get(size_t i) const { return data_[i]; }
    void print() const {
        for (const auto& item : data_)
            std::cout << item << " ";
        std::cout << std::endl;
    }
};

int main() {
    // Uses vector by default
    DataStore<int> vec_store;
    vec_store.add(1);
    vec_store.add(2);
    vec_store.add(3);
    vec_store.print();  // 1 2 3

    // Explicitly use deque
    DataStore<std::string, std::deque> deque_store;
    deque_store.add("hello");
    deque_store.add("world");
    deque_store.print();  // hello world

    std::cout << "Sizes: " << vec_store.size() << ", "
              << deque_store.size() << std::endl;  // 3, 2
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `template<typename, typename> class C` | A template template parameter accepting two type arguments |
| Default template template arg | `= std::vector` provides a default container |
| Policy-based design | Parameterize behavior by passing different template "policies" |
| `Container<T, Alloc>` | Instantiate the template template parameter with concrete types |

### Practice

4. 🌟🌟🌟 **Fill:** Complete the `Stack` class that uses a template template parameter for its underlying container. It should work with `std::vector` and `std::deque`.

```cpp,editable
#include <iostream>
#include <vector>
#include <deque>
#include <cassert>

template<typename T, template<typename, typename> class Container = std::vector>
class Stack {
    __ data_;  // instantiate Container with T and std::allocator<T>
public:
    void push(const T& val) { data_.push_back(val); }

    T pop() {
        T val = data_.back();
        data_.pop_back();
        return val;
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }
};

int main() {
    // Stack using vector (default)
    Stack<int> vec_stack;
    vec_stack.push(10);
    vec_stack.push(20);
    vec_stack.push(30);
    assert(vec_stack.size() == 3);
    assert(vec_stack.pop() == 30);
    assert(vec_stack.pop() == 20);

    // Stack using deque
    Stack<std::string, std::deque> deque_stack;
    deque_stack.push("hello");
    deque_stack.push("world");
    assert(deque_stack.size() == 2);
    assert(deque_stack.pop() == "world");
    assert(deque_stack.pop() == "hello");
    assert(deque_stack.empty());

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Implement a compile-time `Map` template that applies a metafunction (a template template parameter) to every type in a `TypeList`, producing a new `TypeList` of transformed types.

```cpp,editable
#include <iostream>
#include <type_traits>
#include <cassert>

template<typename... Ts>
struct TypeList {};

// Count the number of types in a TypeList
template<typename List>
struct Length;

template<typename... Ts>
struct Length<TypeList<Ts...>> {
    static constexpr int value = sizeof...(Ts);
};

// Map: apply a metafunction F to each type in a TypeList
// Map<TypeList<A, B, C>, F> = TypeList<F<A>::type, F<B>::type, F<C>::type>
template<typename List, template<typename> class F>
struct Map;

template<template<typename> class F, typename... Ts>
struct Map<TypeList<Ts...>, F> {
    using type = TypeList<__>;  // apply F to each T and extract ::type
};

// Helper alias
template<typename List, template<typename> class F>
using Map_t = typename Map<List, F>::type;

int main() {
    using Input = TypeList<int, char, float>;

    // std::add_pointer: int -> int*, char -> char*, float -> float*
    using Pointers = Map_t<Input, std::add_pointer>;
    static_assert(std::is_same_v<Pointers, TypeList<int*, char*, float*>>);

    // std::add_const: int -> const int, char -> const char, float -> const float
    using Consts = Map_t<Input, std::add_const>;
    static_assert(std::is_same_v<Consts, TypeList<const int, const char, const float>>);

    // Length is preserved
    static_assert(Length<Input>::value == Length<Pointers>::value);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
