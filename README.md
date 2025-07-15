# asio-future

A header-only C++ library that seamlessly converts `std::future<T>` to `asio::awaitable<T>` for use in asio coroutines.

## Features

- **Header-only**: Easy to integrate into any project
- **Non-blocking**: Uses thread pool to avoid blocking IO threads
- **Exception-safe**: Proper exception propagation through coroutines
- **Flexible API**: Support for custom thread pools and completion tokens
- **Modern C++**: Requires C++20 coroutines and asio

## Quick Start

```cpp
#include <asio_future/make_awaitable.hpp>
#include <asio.hpp>
#include <future>
#include <iostream>

// Example: database query returning std::future
std::future<std::string> query_database(const std::string& sql) {
    return std::async(std::launch::async, [sql] {
        // Simulate database work
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return "Query result: " + sql;
    });
}

// Use in asio coroutine
asio::awaitable<void> example_coroutine() {
    try {
        // Convert future to awaitable seamlessly
        auto result = co_await asio_future::make_awaitable(
            query_database("SELECT * FROM users")
        );
        std::cout << "Success: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

int main() {
    asio::io_context ctx;
    asio::co_spawn(ctx, example_coroutine(), asio::detached);
    ctx.run();
    return 0;
}
```

## Requirements

- C++20 or later
- boost::asio or standalone asio
- CMake 3.15+ (for building examples and tests)

## Installation

### Header-only Integration

Simply copy the `include/asio_future/` directory to your project and include:

```cpp
#include <asio_future/make_awaitable.hpp>
```

### CMake Integration

```cmake
find_package(asio REQUIRED)
target_link_libraries(your_target PRIVATE asio::asio)
target_include_directories(your_target PRIVATE path/to/asio-future/include)
```

## API Reference

### Basic Usage

```cpp
template<typename T>
asio::awaitable<T> make_awaitable(std::future<T> future);
```

Converts a `std::future<T>` to `asio::awaitable<T>` using the default thread pool.

### Custom Thread Pool

```cpp
template<typename T>
asio::awaitable<T> make_awaitable(std::future<T> future, asio::thread_pool& pool);
```

Converts using a custom thread pool for blocking operations.

### Advanced Usage

```cpp
template<typename T, typename CompletionToken>
auto make_awaitable(std::future<T> future, CompletionToken&& token);
```

Low-level interface for advanced users who need custom completion tokens.

## Examples

See the `examples/` directory for complete examples:

- `basic_usage.cpp` - Simple usage example
- `database_example.cpp` - Database integration example
- `error_handling.cpp` - Exception handling demonstration
- `custom_thread_pool.cpp` - Using custom thread pools

## Performance

- **Non-blocking**: All blocking operations happen in separate thread pool
- **Zero-copy**: Results are moved between threads without copying
- **Efficient**: Uses `asio::async_initiate` for optimal integration

## License

MIT License - see LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## Similar Projects

- [asio](https://github.com/chriskohlhoff/asio) - The underlying asio library
- [cppcoro](https://github.com/lewissbaker/cppcoro) - C++ coroutine library

## Acknowledgments

Thanks to the asio community for providing the foundation for this library. 