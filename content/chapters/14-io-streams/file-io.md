# File I/O

File I/O in C++ uses **streams** — the same abstraction as `std::cin` and `std::cout`. The `<fstream>` header provides `std::ofstream` for writing and `std::ifstream` for reading. Streams manage buffering, encoding, and error handling so you can work with files using familiar `<<` and `>>` operators.

## Writing Files with ofstream

`std::ofstream` opens a file for writing. By default it creates the file if it doesn't exist and truncates it if it does. You can choose different opening modes like `std::ios::app` (append) to add to the end instead.

### How it works

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Write to a file (creates or truncates)
    std::ofstream out("/tmp/greeting.txt");
    out << "Hello, file!" << std::endl;
    out << "Line two." << std::endl;
    out.close();

    // Append to the same file
    std::ofstream app("/tmp/greeting.txt", std::ios::app);
    app << "Appended line." << std::endl;
    app.close();

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::ofstream out(path)` | Opens file for writing (truncates by default) |
| `std::ios::app` | Append mode — writes go to end of file |
| `std::ios::trunc` | Truncate mode (default) — clears existing content |
| `out << data` | Writes data to file, just like `std::cout` |
| `out.close()` | Closes the file (also happens automatically at destruction) |

### Practice

1. 🌟 **Fill:** Open a file for writing and write two lines to it.

```cpp,editable
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

int main() {
    // Open /tmp/test_write.txt for writing
    std::ofstream out(__);
    out << "First line" << std::endl;
    out << "Second line" << std::endl;
    out.close();

    // Verify by reading it back
    std::ifstream in("/tmp/test_write.txt");
    std::string line1, line2;
    std::getline(in, line1);
    std::getline(in, line2);
    in.close();

    assert(line1 == "First line");
    assert(line2 == "Second line");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Reading Files with ifstream

`std::ifstream` opens a file for reading. You can read word-by-word with `>>`, or line-by-line with `std::getline`. Always check that the file opened successfully before reading.

### How it works

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    // First, create a file to read
    std::ofstream out("/tmp/data.txt");
    out << "Alice 90\nBob 85\nCarol 95\n";
    out.close();

    // Read line by line
    std::ifstream in("/tmp/data.txt");
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
    in.close();

    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::ifstream in(path)` | Opens file for reading |
| `std::getline(in, str)` | Reads one full line into `str`; returns the stream (false at EOF) |
| `in >> var` | Reads one whitespace-delimited token |
| `if (in.is_open())` | Checks if the file was opened successfully |
| `while (std::getline(...))` | Idiomatic loop to read all lines |

### Practice

2. 🌟🌟 **Complete:** Write a file with three names, then read them back line-by-line into a vector.

```cpp,editable
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

int main() {
    // Write a file with three names
    std::ofstream out("/tmp/names.txt");
    out << "Alice" << std::endl;
    out << "Bob" << std::endl;
    out << "Carol" << std::endl;
    out.close();

    // Read all lines into a vector
    std::ifstream in("/tmp/names.txt");
    std::vector<std::string> names;
    std::string line;
    while (__) {
        __;
    }
    in.close();

    assert(names.size() == 3);
    assert(names[0] == "Alice");
    assert(names[1] == "Bob");
    assert(names[2] == "Carol");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Stream State Flags

Every stream tracks its state with flags: `good()`, `fail()`, `eof()`, and `bad()`. After a failed operation (e.g., reading past end-of-file or parsing an invalid number), the stream enters a fail state and all subsequent reads silently do nothing until you `clear()` the error.

### How it works

```cpp
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ofstream out("/tmp/state_demo.txt");
    out << "42 hello";
    out.close();

    std::ifstream in("/tmp/state_demo.txt");
    int n;
    in >> n;    // reads 42
    std::cout << "good: " << in.good() << std::endl;  // 1

    in >> n;    // tries to read "hello" as int — fails
    std::cout << "fail: " << in.fail() << std::endl;  // 1

    in.clear();  // reset error flags
    std::string s;
    in >> s;     // now reads "hello" as string
    std::cout << s << std::endl;  // hello

    return 0;
}
```

| Flag | Meaning |
|------|---------|
| `good()` | No errors; stream is ready |
| `fail()` | A logical error occurred (e.g., type mismatch) |
| `eof()` | End-of-file was reached |
| `bad()` | A serious I/O error occurred (e.g., disk failure) |
| `clear()` | Resets all error flags back to `good` |

### Practice

3. 🌟🌟 **Fill:** Check the stream state flags at each step.

```cpp,editable
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

int main() {
    std::ofstream out("/tmp/flags.txt");
    out << "100 abc";
    out.close();

    std::ifstream in("/tmp/flags.txt");
    int value;

    in >> value;  // reads 100
    assert(value == 100);
    assert(in.__() == true);   // stream is good

    in >> value;  // tries to read "abc" as int — fails
    assert(in.__() == true);   // stream is in fail state

    in.__();      // reset the error flags
    std::string word;
    in >> word;   // now reads "abc" as string
    assert(word == "abc");

    std::cout << "Success!" << std::endl;
    return 0;
}
```

## Binary File I/O

Binary mode (`std::ios::binary`) writes raw bytes instead of formatted text. Use `write()` and `read()` with `reinterpret_cast<char*>` to store and retrieve data in its exact memory representation. This is faster and more compact than text, but not human-readable.

### How it works

```cpp
#include <iostream>
#include <fstream>

int main() {
    // Write binary data
    double pi = 3.14159265358979;
    std::ofstream out("/tmp/pi.bin", std::ios::binary);
    out.write(reinterpret_cast<const char*>(&pi), sizeof(pi));
    out.close();

    // Read it back
    double loaded = 0.0;
    std::ifstream in("/tmp/pi.bin", std::ios::binary);
    in.read(reinterpret_cast<char*>(&loaded), sizeof(loaded));
    in.close();

    std::cout << loaded << std::endl;  // 3.14159265358979
    return 0;
}
```

| Concept | Detail |
|---------|--------|
| `std::ios::binary` | Opens in binary mode (no newline translation) |
| `out.write(ptr, size)` | Writes `size` raw bytes from `ptr` |
| `in.read(ptr, size)` | Reads `size` raw bytes into `ptr` |
| `reinterpret_cast<char*>(&var)` | Treats the variable's memory as a byte array |
| `sizeof(var)` | Number of bytes the variable occupies |

### Practice

4. 🌟🌟🌟 **Complete:** Write an array of integers to a binary file, then read them back and verify.

```cpp,editable
#include <iostream>
#include <fstream>
#include <cassert>

int main() {
    int data[] = {10, 20, 30, 40, 50};
    int count = 5;

    // Write the array to a binary file
    std::ofstream out("/tmp/array.bin", std::ios::binary);
    out.write(__, __);
    out.close();

    // Read it back into a new array
    int loaded[5] = {};
    std::ifstream in("/tmp/array.bin", std::ios::binary);
    in.read(__, __);
    in.close();

    for (int i = 0; i < count; ++i) {
        assert(loaded[i] == data[i]);
    }

    std::cout << "Success!" << std::endl;
    return 0;
}
```

5. 🌟🌟🌟 **Complete:** Write a struct to a binary file, then read it back and verify all fields.

```cpp,editable
#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>

struct Record {
    int id;
    double score;
    char name[32];
};

int main() {
    Record original;
    original.id = 42;
    original.score = 98.5;
    std::strncpy(original.name, "Alice", sizeof(original.name));

    // Write the struct to binary file
    std::ofstream out(__, __);
    out.write(reinterpret_cast<const char*>(&original), __);
    out.close();

    // Read it back
    Record loaded;
    std::ifstream in(__, __);
    in.read(reinterpret_cast<char*>(&loaded), __);
    in.close();

    assert(loaded.id == 42);
    assert(loaded.score == 98.5);
    assert(std::strcmp(loaded.name, "Alice") == 0);

    std::cout << "Success!" << std::endl;
    return 0;
}
```

> You can find the solutions [here](https://github.com/duykha0205/cpp-by-practice) (under the solutions path), but only use it when you need it.
