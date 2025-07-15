# Project Structure

This document describes the structure of the asio-future project.

## Directory Structure

```
asio-future/
├── include/
│   └── asio_future/
│       └── make_awaitable.hpp    # Main header file
├── examples/
│   ├── basic_usage.cpp           # Basic usage example
│   ├── error_handling.cpp        # Error handling example
│   ├── database_example.cpp      # Database integration example
│   └── CMakeLists.txt           # Examples build configuration
├── tests/
│   ├── test_make_awaitable.cpp   # Unit tests
│   └── CMakeLists.txt           # Tests build configuration
├── cmake/
│   └── asio-future-config.cmake.in # CMake config template
├── docs/
│   └── api.md                   # API documentation
├── CMakeLists.txt               # Main build configuration
├── README.md                    # Project documentation
├── LICENSE                      # MIT License
├── .gitignore                   # Git ignore rules
└── PROJECT_STRUCTURE.md         # This file
```

## Key Components

### Header-only Library
- `include/asio_future/make_awaitable.hpp` - The main library header
- Provides `make_awaitable()` function to convert `std::future<T>` to `asio::awaitable<T>`

### Examples
- `basic_usage.cpp` - Shows basic usage patterns
- `error_handling.cpp` - Demonstrates exception handling
- `database_example.cpp` - Simulates database operations

### Build System
- CMake-based build system
- Supports both standalone build and integration as subdirectory
- Automatic dependency resolution for asio and threading

### Testing
- Unit tests using modern C++ testing framework
- Integration tests for real-world scenarios

## Getting Started

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/asio-future.git
   cd asio-future
   ```

2. **Build the project**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. **Run examples**
   ```bash
   ./examples/basic_usage
   ./examples/error_handling
   ./examples/database_example
   ```

4. **Run tests**
   ```bash
   make test
   ```

## Integration

### As a submodule
```bash
git submodule add https://github.com/yourusername/asio-future.git third_party/asio-future
```

### In CMakeLists.txt
```cmake
add_subdirectory(third_party/asio-future)
target_link_libraries(your_target PRIVATE asio-future::asio-future)
```

### Header-only usage
```cpp
#include <asio_future/make_awaitable.hpp>

// Use asio_future::make_awaitable() in your code
```

## Requirements

- C++20 or later
- boost::asio or standalone asio
- CMake 3.15+
- Modern C++ compiler (GCC 10+, Clang 11+, MSVC 19.20+)

## License

MIT License - see LICENSE file for details. 