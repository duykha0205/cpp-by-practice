# String Streams

String streams let you use the familiar `<<` and `>>` stream operators to build and parse strings in memory. The `<sstream>` header provides `std::ostringstream` for composing strings from mixed types and `std::istringstream` for extracting typed values from a string — no files or console needed.

## std::ostringstream

`std::ostringstream` works like `std::cout`, but writes to an internal string buffer instead of the console. Call `str()` to retrieve the composed string. This is the cleanest way to build a string from mixed types (ints, doubles, strings) without manual concatenation.

### How it works

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::ostringstream oss;
    oss << "Name: " << "Alice" << ", Age: " << 30 << ", GPA: " << 3.85;

    std::string result = oss.str();
    std::cout << result << std::endl;
    // Name: Alice, Age: 30, GPA: 3.85

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::ostringstream oss` | Creates an output string stream |
| `oss << value` | Appends the value to the internal buffer |
| `oss.str()` | Returns the composed `std::string` |
| `oss.str("")` | Clears the internal buffer |
| Mixed types | Ints, doubles, strings — all work with `<<` |

### Practice

1. 🌟 **Fill:** Use `ostringstream` to build a formatted string from mixed types.

```cpp,editable
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

int main() {
    std::string name = "Bob";
    int age = 25;
    double height = 5.11;

    std::ostringstream oss;
    oss << __ << __ << __ << __ << __ << __;

    std::string result = oss.str();
    assert(result == "Bob is 25 years old and 5.11 feet tall");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

2. 🌟🌟 **Complete:** Build a comma-separated string from a vector of integers using `ostringstream`.

```cpp,editable
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>

int main() {
    std::vector<int> nums = {10, 20, 30, 40, 50};

    std::ostringstream oss;
    for (size_t i = 0; i < nums.size(); ++i) {
        if (i > 0) {
            __;
        }
        __;
    }

    std::string result = oss.str();
    assert(result == "10, 20, 30, 40, 50");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## std::istringstream

`std::istringstream` works like `std::cin`, but reads from a string instead of the keyboard. You can extract typed values with `>>` or read line-by-line with `std::getline`. It is ideal for parsing structured text data.

### How it works

```cpp
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string data = "Alice 90 3.85";
    std::istringstream iss(data);

    std::string name;
    int score;
    double gpa;
    iss >> name >> score >> gpa;

    std::cout << name << " scored " << score
              << " with GPA " << gpa << std::endl;
    // Alice scored 90 with GPA 3.85

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::istringstream iss(str)` | Creates an input stream that reads from `str` |
| `iss >> var` | Extracts the next whitespace-delimited token into `var` |
| `std::getline(iss, line)` | Reads one full line from the stream |
| `while (iss >> var)` | Loop that reads all tokens until the stream is exhausted |
| Type conversion | Automatically converts to `int`, `double`, `std::string`, etc. |

### Practice

3. 🌟🌟 **Fill:** Parse a string containing a name and two scores using `istringstream`.

```cpp,editable
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

int main() {
    std::string data = "Carol 88 95";

    __ iss(__);

    std::string name;
    int score1, score2;
    iss >> __ >> __ >> __;

    assert(name == "Carol");
    assert(score1 == 88);
    assert(score2 == 95);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

4. 🌟🌟 **Complete:** Count the number of words in a sentence using `istringstream`.

```cpp,editable
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

int main() {
    std::string sentence = "the quick brown fox jumps";

    std::istringstream iss(sentence);
    std::string word;
    int count = 0;

    while (__) {
        __;
    }

    assert(count == 5);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Stream Manipulators

Stream manipulators from `<iomanip>` control formatting: field width, precision, fill characters, alignment, and number base. They work with any output stream — `std::cout`, `std::ofstream`, or `std::ostringstream`.

### How it works

```cpp
#include <iostream>
#include <iomanip>
#include <sstream>

int main() {
    // Width and fill
    std::cout << std::setw(10) << std::setfill('*') << 42 << std::endl;
    // ********42

    // Precision and fixed
    std::cout << std::fixed << std::setprecision(2) << 3.14159 << std::endl;
    // 3.14

    // Hex output
    std::cout << std::hex << 255 << std::endl;
    // ff

    // Alignment
    std::cout << std::left << std::setw(10) << std::setfill('.') << "Hi" << std::endl;
    // Hi........

    return 0;
}
```

| Manipulator | Effect |
|-------------|--------|
| `std::setw(n)` | Sets minimum field width to `n` (applies to next output only) |
| `std::setfill(c)` | Sets the fill character (sticky — stays until changed) |
| `std::setprecision(n)` | Sets decimal precision to `n` digits (sticky) |
| `std::fixed` | Use fixed-point notation for floats (sticky) |
| `std::hex` / `std::oct` / `std::dec` | Set integer output base (sticky) |
| `std::left` / `std::right` | Align output within the field width (sticky) |

### Practice

5. 🌟🌟 **Fill:** Format a price with exactly 2 decimal places using `fixed` and `setprecision`, and display a hex value.

```cpp,editable
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cassert>

int main() {
    // Part 1: Format a price to 2 decimal places
    double price = 9.5;
    std::ostringstream oss1;
    oss1 << __ << __ << price;
    assert(oss1.str() == "9.50");

    // Part 2: Format 255 as hexadecimal
    std::ostringstream oss2;
    oss2 << __ << 255;
    assert(oss2.str() == "ff");

    // Part 3: Right-align a number in a field of width 8, filled with zeros
    std::ostringstream oss3;
    oss3 << __ << __ << __ << 42;
    assert(oss3.str() == "00000042");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
