#include <asio_future/make_awaitable.hpp>
#include <asio.hpp>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// Example: simulate some async work that returns std::future
std::future<std::string> simulate_async_work(const std::string& task) {
    return std::async(std::launch::async, [task] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return "Completed: " + task;
    });
}

// Example: CPU-intensive task
std::future<int> calculate_fibonacci(int n) {
    return std::async(std::launch::async, [n] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (n <= 1) return n;
        
        int a = 0, b = 1;
        for (int i = 2; i <= n; ++i) {
            int temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    });
}

asio::awaitable<void> example_coroutine() {
    std::cout << "Starting coroutine..." << std::endl;
    
    try {
        // Example 1: Basic string result
        auto result1 = co_await asio_future::make_awaitable(
            simulate_async_work("Database query")
        );
        std::cout << "Result 1: " << result1 << std::endl;
        
        // Example 2: Multiple concurrent operations
        auto future1 = simulate_async_work("Task 1");
        auto future2 = simulate_async_work("Task 2");
        auto future3 = calculate_fibonacci(10);
        
        auto result2 = co_await asio_future::make_awaitable(std::move(future1));
        auto result3 = co_await asio_future::make_awaitable(std::move(future2));
        auto result4 = co_await asio_future::make_awaitable(std::move(future3));
        
        std::cout << "Result 2: " << result2 << std::endl;
        std::cout << "Result 3: " << result3 << std::endl;
        std::cout << "Result 4: " << result4 << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    std::cout << "Coroutine finished!" << std::endl;
}

int main() {
    std::cout << "Starting basic usage example..." << std::endl;
    
    asio::io_context ctx;
    
    asio::co_spawn(ctx, example_coroutine(), asio::detached);
    
    std::cout << "Running io_context..." << std::endl;
    ctx.run();
    
    std::cout << "Program finished!" << std::endl;
    return 0;
} 