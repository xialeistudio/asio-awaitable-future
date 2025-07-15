#include <asio_future/make_awaitable.hpp>
#include <asio.hpp>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector> // Added for std::vector

asio::thread_pool pool(std::max(1u, std::thread::hardware_concurrency()));

// Example: function that always throws an exception
std::future<std::string> failing_operation() {
    return std::async(std::launch::async, []() -> std::string {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        throw std::runtime_error("Operation failed!");
    });
}

// Example: function that may throw based on input
std::future<int> risky_calculation(int value) {
    return std::async(std::launch::async, [value] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (value < 0) {
            throw std::invalid_argument("Negative values not allowed");
        }
        if (value > 100) {
            throw std::out_of_range("Value too large");
        }
        return value * 2;
    });
}

asio::awaitable<void> error_handling_example() {
    std::cout << "Starting error handling example..." << std::endl;
    
    // Example 1: Handle guaranteed exception
    try {
        auto result = co_await asio_future::make_awaitable(failing_operation(), pool);
        std::cout << "This should not print: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught runtime_error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    
    // Example 2: Handle different exception types
    std::vector<int> test_values = {-5, 50, 150};
    
    for (int value : test_values) {
        try {
            std::cout << "Testing value: " << value << std::endl;
            auto result = co_await asio_future::make_awaitable(risky_calculation(value), pool);
            std::cout << "Success - Result: " << result << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Out of range: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Other exception: " << e.what() << std::endl;
        }
    }
    
    std::cout << "Error handling example finished!" << std::endl;
}

int main() {
    std::cout << "Starting error handling demo..." << std::endl;
    
    asio::io_context ctx;
    
    asio::co_spawn(ctx, error_handling_example(), asio::detached);
    
    ctx.run();
    
    std::cout << "Program finished!" << std::endl;
    return 0;
} 