# C++ By Practice

Learn C++ through hands-on exercises. Inspired by [rust-by-practice](https://practice.rs).

Built entirely in C++ using the [Crow](https://crowcpp.org/) web framework.

## Architecture

```
Browser (HTML + CodeMirror editor)
    |
    | HTTP
    v
Crow C++ Server (:8080)
    |-- Page routes: renders Markdown exercises as HTML (md4c)
    |-- API routes: POST /api/run → docker run gcc:14
    |
    v
Docker (gcc:14 container)
    |-- Compiles & runs C++ code in sandboxed container
    |-- --network=none, --read-only, --cap-drop ALL
    |-- Returns stdout/stderr
```

## Chapters

| # | Topic | Exercises |
|---|-------|-----------|
| 1 | Variables | Declaration, const, scope, auto |
| 2 | Basic Types | Numbers, char, bool, type casting |
| 3 | Functions | Overloading, default params, pass by ref, recursion |
| 4 | Flow Control | if/else, switch, for, while, break/continue |
| 5 | Arrays & Strings | C-arrays, std::array, std::string |
| 6 | Pointers & References | Raw pointers, references, const pointers |
| 7 | Structs & Classes | Structs, classes, constructors, copy |
| 8 | OOP | Inheritance, polymorphism, virtual, abstract |
| 9 | Templates | Function templates, class templates, variadic |
| 10 | STL | vector, map, algorithms |
| 11 | Memory Management | new/delete, unique_ptr, shared_ptr, weak_ptr |
| 12 | Modern C++ | auto, lambda, move semantics, forwarding |

## Prerequisites

- CMake 3.20+
- C++20 compiler (GCC 11+, Clang 14+)
- [vcpkg](https://github.com/microsoft/vcpkg) package manager
- Docker (for sandboxed code execution)

## Quick Start

### 1. Clone the repository

```bash
git clone https://github.com/duykha0205/cpp-by-practice.git
cd cpp-by-practice

# Install vcpkg if not already installed
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh
```

### 2. Build and run with Make

```bash
make setup    # Bootstrap vcpkg, install deps, pull gcc:14 image
make run      # Build + start the server
```

Open http://localhost:8080 in your browser.

### Available Make commands

| Command | Description |
|---------|-------------|
| `make setup` | Bootstrap vcpkg + install deps + pull gcc:14 |
| `make build` | Configure + build the project |
| `make run` | Build then run the server |
| `make rebuild` | Clean then build from scratch |
| `make deps` | Install vcpkg dependencies only |
| `make clean` | Remove build directory |
| `make docker-run` | Run everything with Docker Compose |
| `make help` | Show all available commands |

### Alternative: Run everything with Docker

```bash
make docker-run
# or directly:
docker compose up --build
```

### Manual build (without Make)

```bash
export VCPKG_ROOT=$(pwd)/vcpkg
./vcpkg/vcpkg install
cmake -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build --parallel
cd build && ./cpp-by-practice
```

**Troubleshooting:** If CMake reports `Could not find Crow`, clean and rebuild: `make rebuild`

## Environment Variables

| Variable | Default | Description |
|----------|---------|-------------|
| `PORT` | `8080` | Server port |
| `CONTENT_DIR` | `content` | Path to exercise content |
| `SANDBOX_IMAGE` | `gcc:14` | Docker image for compilation |
| `SANDBOX_TIMEOUT` | `5` | Max execution time (seconds) |
| `SANDBOX_MEMORY` | `128m` | Container memory limit |
| `SANDBOX_TMP_DIR` | (system temp) | Temp dir for code files |

## Project Structure

```
cpp-by-practice/
├── src/
│   ├── main.cpp              # Entry point, Crow app setup
│   ├── routes/               # HTTP route handlers
│   ├── services/             # Business logic (markdown, exercises, sandbox)
│   ├── models/               # Data structures
│   └── utils/                # File utilities
├── content/                  # Exercise Markdown files
│   ├── toc.json              # Table of contents
│   └── chapters/             # One directory per chapter
├── templates/                # Mustache HTML templates
├── static/                   # CSS, JS, frontend assets
├── CMakeLists.txt
├── Makefile                  # Convenience commands (make build, make run, ...)
├── vcpkg.json
├── Dockerfile
└── docker-compose.yml
```

## Dependencies

| Library | Purpose |
|---------|---------|
| [Crow](https://crowcpp.org/) | C++ web framework (routing, Mustache, JSON) |
| [md4c](https://github.com/mity/md4c) | Markdown to HTML parser |
| [nlohmann/json](https://github.com/nlohmann/json) | JSON parsing |
| [Docker](https://www.docker.com/) + gcc:14 | Sandboxed code execution |

## Sandbox Security

Each code execution runs in an ephemeral Docker container with:

| Flag | Purpose |
|------|---------|
| `--rm` | Auto-remove container after exit |
| `--network=none` | Zero network access |
| `--read-only` | Root filesystem is immutable |
| `--tmpfs /tmp:rw,exec,size=64m` | Writable area in RAM only |
| `--cap-drop ALL` | Drop all Linux capabilities |
| `--security-opt no-new-privileges` | Prevent privilege escalation |
| `--memory=128m` | Hard RAM limit |
| `--cpus=1` | CPU limit |
| `--pids-limit 64` | Prevent fork bombs |

## Exercise Format

Exercises are written in Markdown. Code blocks marked with `` ```cpp,editable `` become interactive editors in the browser:

```markdown
1. Fix the error below.

\`\`\`cpp,editable
#include <iostream>
int main() {
    const int x = 10;
    x = 20;  // ERROR!
    std::cout << x << std::endl;
    return 0;
}
\`\`\`
```

Difficulty levels: Easy | Medium | Hard

## License

MIT
