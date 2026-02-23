# Flow Control

Flow control determines the order in which statements execute. Branching (`if`, `switch`) lets your program choose different paths, and loops (`for`, `while`, `do-while`) let it repeat work. Combined with `break` and `continue`, these tools give you full control over program flow.

## If / Else

### How it works

`if` tests a condition. If it is true the body runs; otherwise the optional `else if` / `else` branches are checked in order.

```cpp
#include <iostream>

int main() {
    int n = 0;

    if (n > 0) {
        std::cout << "positive" << std::endl;
    } else if (n < 0) {
        std::cout << "negative" << std::endl;
    } else {
        std::cout << "zero" << std::endl;
    }
    return 0;
}
```

| Keyword | Purpose |
|---|---|
| `if (cond)` | First condition to test |
| `else if (cond)` | Additional condition tested only when previous ones failed |
| `else` | Catch-all when no condition was true |

### Practice

1\. 🌟 Fill the blanks with the correct keywords so the program compiles and runs.

```cpp,editable
#include <iostream>

int main() {
    int n = 5;

    if (n < 0) {
        std::cout << n << " is negative" << std::endl;
    } __ (n > 0) {
        std::cout << n << " is positive" << std::endl;
    } __ {
        std::cout << n << " is zero" << std::endl;
    }

    return 0;
}
```

## Ternary Operator

### How it works

The ternary operator `condition ? value_if_true : value_if_false` is a compact one-line alternative to a simple `if / else`.

```cpp
#include <iostream>
#include <string>

int main() {
    int age = 20;
    std::string status = (age >= 18) ? "adult" : "minor";
    std::cout << status << std::endl;  // adult
    return 0;
}
```

| Part | Meaning |
|---|---|
| `condition` | Boolean expression evaluated first |
| `? value_if_true` | Result when the condition is `true` |
| `: value_if_false` | Result when the condition is `false` |

### Practice

2\. 🌟🌟 Rewrite the `if/else` block using a single ternary expression, then remove the `if/else`.

```cpp,editable
#include <iostream>
#include <cassert>
#include <string>

int main() {
    int n = 10;
    std::string result;

    if (n % 2 == 0) {
        result = "even";
    } else {
        result = "odd";
    }

    // Rewrite the above using ternary: result = __ ? __ : __;
    // Then remove the if/else block

    assert(result == "even");
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Switch

### How it works

`switch` compares a single value against multiple `case` labels. Each case needs a `break`; without it execution **falls through** to the next case.

```cpp
#include <iostream>
#include <string>

int main() {
    int day = 3;
    std::string name;

    switch (day) {
        case 1: name = "Monday";    break;
        case 2: name = "Tuesday";   break;
        case 3: name = "Wednesday"; break;
        case 4: name = "Thursday";  break;
        case 5: name = "Friday";    break;
        default: name = "Weekend";  break;
    }

    std::cout << name << std::endl;  // Wednesday
    return 0;
}
```

| Concept | Detail |
|---|---|
| `case value:` | Execution jumps here when the switch expression equals `value` |
| `break;` | Exits the switch; without it code falls into the next case |
| `default:` | Runs when no case matches |
| Fall-through | Intentional omission of `break` — rare and should be commented |

### Practice

3\. 🌟🌟 Fix: the missing `break` statements cause fall-through so `name` ends up as `"Weekend"`.

```cpp,editable
#include <iostream>
#include <cassert>
#include <string>

int main() {
    int day = 3;
    std::string name;

    switch (day) {
        case 1: name = "Monday";
        case 2: name = "Tuesday";
        case 3: name = "Wednesday";
        case 4: name = "Thursday";
        case 5: name = "Friday";
        default: name = "Weekend";
    }

    assert(name == "Wednesday");
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## For Loop

### How it works

The classic `for` loop has three parts: initialization, condition, and increment. C++11 added the **range-based for** loop which iterates directly over elements of a container.

```cpp
#include <iostream>

int main() {
    // Classic for
    for (int i = 1; i <= 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;  // 1 2 3 4 5

    // Range-based for
    int arr[] = {10, 20, 30};
    for (int val : arr) {
        std::cout << val << " ";
    }
    std::cout << std::endl;  // 10 20 30
    return 0;
}
```

| Part | Classic `for` | Range-based `for` |
|---|---|---|
| Syntax | `for (init; cond; step)` | `for (type var : container)` |
| Use when | You need the index or custom step | You just need each element |
| C++ version | All | C++11 and later |

### Practice

4\. 🌟 Fix: the loop prints `0 1 2 3 4` instead of `1 2 3 4 5`. Adjust it.

```cpp,editable
#include <iostream>

int main() {
    for (int i = 0; i < 5; i++) {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    // Expected output: 1 2 3 4 5
    return 0;
}
```

5\. 🌟🌟 Fill the blank to iterate over the array with a range-based for loop.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int arr[] = {10, 20, 30, 40, 50};
    int sum = 0;

    for (__ : arr) {
        sum += val;
    }

    assert(sum == 150);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## While Loop

### How it works

A `while` loop checks its condition **before** each iteration. If the condition is false from the start the body never executes.

```cpp
#include <iostream>

int main() {
    int n = 5;
    while (n > 0) {
        std::cout << n << " ";
        n--;
    }
    std::cout << std::endl;  // 5 4 3 2 1
    return 0;
}
```

| Aspect | Detail |
|---|---|
| Condition checked | Before each iteration |
| Zero iterations? | Yes — if the condition is initially false |
| Infinite loop | `while (true) { … }` — use `break` to exit |

### Practice

6\. 🌟🌟 Fill the blanks to make FizzBuzz work from 1 to 15 using a `while` loop.

```cpp,editable
#include <iostream>

int main() {
    int n = 1;

    while (n __ 16) {
        if (n % 15 == 0) {
            std::cout << "FizzBuzz";
        } else if (n % 3 == 0) {
            std::cout << "Fizz";
        } else if (n % 5 == 0) {
            std::cout << "Buzz";
        } else {
            std::cout << n;
        }
        std::cout << " ";
        __;
    }
    std::cout << std::endl;
    return 0;
}
```

## Do-While

### How it works

`do-while` is like `while` but the body runs **at least once** because the condition is checked **after** the body.

```cpp
#include <iostream>

int main() {
    int n = 0;
    do {
        std::cout << n << " ";
        n++;
    } while (n < 3);
    std::cout << std::endl;  // 0 1 2
    return 0;
}
```

| Aspect | `while` | `do-while` |
|---|---|---|
| Condition check | Before body | After body |
| Minimum runs | 0 | 1 |

### Practice

7\. 🌟 Fill the blank so the loop runs exactly 3 times.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int count = 0;

    do {
        count++;
    } while (count __ 3);

    assert(count == 3);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Break and Continue

### How it works

`break` exits the nearest enclosing loop immediately. `continue` skips the rest of the current iteration and jumps to the next one.

```cpp
#include <iostream>

int main() {
    // break — stop at 3
    for (int i = 1; i <= 5; i++) {
        if (i == 4) break;
        std::cout << i << " ";
    }
    std::cout << std::endl;  // 1 2 3

    // continue — skip odd numbers
    for (int i = 1; i <= 6; i++) {
        if (i % 2 != 0) continue;
        std::cout << i << " ";
    }
    std::cout << std::endl;  // 2 4 6
    return 0;
}
```

| Statement | Effect |
|---|---|
| `break` | Exits the loop entirely |
| `continue` | Skips to the next iteration |

### Practice

8\. 🌟 Fill the blank to stop the loop when `n` reaches 66.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int n = 0;
    for (int i = 0; i <= 100; i++) {
        if (n == 66) {
            __
        }
        n++;
    }

    assert(n == 66);
    std::cout << "Success!" << std::endl;
    return 0;
}
```

9\. 🌟🌟 Fill the blank so only even numbers from 1 to 10 are printed.

```cpp,editable
#include <iostream>

int main() {
    for (int i = 1; i <= 10; i++) {
        if (i % 2 != 0) {
            __;
        }
        std::cout << i << " ";
    }
    std::cout << std::endl;
    // Expected: 2 4 6 8 10
    return 0;
}
```

## Nested Loops

### How it works

Loops inside loops are common for working with grids or searching pairs. A plain `break` only exits the **inner** loop. To break out of both, use a flag variable checked in the outer loop's condition.

```cpp
#include <iostream>

int main() {
    bool done = false;
    for (int i = 1; i <= 5 && !done; i++) {
        for (int j = 1; j <= 5; j++) {
            if (i * j == 12) {
                std::cout << i << " * " << j << " = 12" << std::endl;
                done = true;
                break;
            }
        }
    }
    return 0;
}
```

| Technique | How it works |
|---|---|
| Flag variable | Set `done = true` inside inner loop; check `!done` in outer loop condition |
| `break` | Only exits the immediately enclosing loop |
| `goto` (alternative) | Jumps to a label — works but generally discouraged |

### Practice

10\. 🌟🌟🌟 Fill the blank: set the flag so both loops exit when `i * j == 42`.

```cpp,editable
#include <iostream>
#include <cassert>

int main() {
    int found_i = 0, found_j = 0;
    bool done = false;

    for (int i = 1; i <= 10 && !done; i++) {
        for (int j = 1; j <= 10; j++) {
            if (i * j == 42) {
                found_i = i;
                found_j = j;
                __;
                break;
            }
        }
    }

    assert(found_i * found_j == 42);
    std::cout << found_i << " * " << found_j << " = 42" << std::endl;
    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
