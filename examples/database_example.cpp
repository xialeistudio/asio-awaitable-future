#include <asio_future/make_awaitable.hpp>
#include <asio.hpp>
#include <future>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>

// Simulate database connection
class DatabaseConnection {
public:
    std::future<std::string> query(const std::string& sql) {
        return std::async(std::launch::async, [sql] {
            // Simulate database latency
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500));
            
            // Simulate different query types
            if (sql.find("SELECT") != std::string::npos) {
                return "Result set: " + sql;
            } else if (sql.find("INSERT") != std::string::npos) {
                return std::string("Inserted 1 row");
            } else if (sql.find("UPDATE") != std::string::npos) {
                return std::string("Updated 3 rows");
            } else {
                return "Query executed: " + sql;
            }
        });
    }
    
    std::future<int> count(const std::string& table) {
        return std::async(std::launch::async, [table] {
            std::this_thread::sleep_for(std::chrono::milliseconds(50 + rand() % 200));
            // Simulate row count
            return 100 + rand() % 1000;
        });
    }
};

asio::awaitable<void> database_operations() {
    DatabaseConnection db;
    
    try {
        std::cout << "Starting database operations..." << std::endl;
        
        // Example 1: Simple SELECT query
        auto users = co_await asio_future::make_awaitable(
            db.query("SELECT * FROM users WHERE active = 1")
        );
        std::cout << "Users query result: " << users << std::endl;
        
        // Example 2: INSERT operation
        auto insert_result = co_await asio_future::make_awaitable(
            db.query("INSERT INTO users (name, email) VALUES ('John', 'john@example.com')")
        );
        std::cout << "Insert result: " << insert_result << std::endl;
        
        // Example 3: Get row count
        auto user_count = co_await asio_future::make_awaitable(
            db.count("users")
        );
        std::cout << "Total users: " << user_count << std::endl;
        
        // Example 4: Multiple concurrent queries
        std::cout << "Running concurrent queries..." << std::endl;
        auto query1 = db.query("SELECT * FROM products WHERE price > 100");
        auto query2 = db.query("SELECT * FROM orders WHERE status = 'pending'");
        auto query3 = db.count("products");
        
        auto result1 = co_await asio_future::make_awaitable(std::move(query1));
        auto result2 = co_await asio_future::make_awaitable(std::move(query2));
        auto result3 = co_await asio_future::make_awaitable(std::move(query3));
        
        std::cout << "Products: " << result1 << std::endl;
        std::cout << "Orders: " << result2 << std::endl;
        std::cout << "Product count: " << result3 << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "Database operation failed: " << e.what() << std::endl;
    }
    
    std::cout << "Database operations completed!" << std::endl;
}

int main() {
    std::cout << "Starting database example..." << std::endl;
    
    asio::io_context ctx;
    
    asio::co_spawn(ctx, database_operations(), asio::detached);
    
    ctx.run();
    
    std::cout << "Database example finished!" << std::endl;
    return 0;
} 