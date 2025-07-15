#pragma once

#include <asio.hpp>
#include <future>
#include <optional>
#include <tuple>

namespace asio_future {

/**
 * @brief Converts std::future<T> to asio::awaitable<T>
 * 
 * This is the low-level interface that uses asio::async_initiate.
 * Most users should use the simplified make_awaitable() function instead.
 * 
 * @tparam T The type contained in the future
 * @tparam CompletionToken The completion token type
 * @param future The future to convert
 * @param token The completion token
 * @param pool The thread pool to use for blocking operations
 * @return An awaitable that can be co_awaited
 */
template<typename T, typename CompletionToken>
auto make_awaitable(std::future<T> future, CompletionToken&& token, asio::thread_pool& pool) {
    return asio::async_initiate<CompletionToken, void(std::tuple<std::optional<T>, std::exception_ptr>)>(
        [future = std::move(future), &pool](auto&& handler) mutable {
            auto executor = asio::get_associated_executor(handler);
            
            // Execute blocking operation in thread pool to avoid blocking IO thread
            asio::post(pool, [future = std::move(future), handler = std::move(handler), executor]() mutable {
                std::tuple<std::optional<T>, std::exception_ptr> result;
                
                try {
                    T value = future.get();
                    result = std::make_tuple(std::make_optional(std::move(value)), nullptr);
                } catch (...) {
                    result = std::make_tuple(std::nullopt, std::current_exception());
                }
                
                // Call handler in original executor context
                asio::post(executor, [handler = std::move(handler), result = std::move(result)]() mutable {
                    handler(std::move(result));
                });
            });
        },
        token
    );
}

/**
 * @brief Converts std::future<T> to asio::awaitable<T> using custom thread pool
 * 
 * This version allows you to specify a custom thread pool for blocking operations.
 * 
 * @tparam T The type contained in the future
 * @param future The future to convert
 * @param pool The thread pool to use for blocking operations
 * @return An awaitable that can be co_awaited
 */
template<typename T>
asio::awaitable<T> make_awaitable(std::future<T> future, asio::thread_pool& pool) {
    auto [result, exception] = co_await make_awaitable(
        std::move(future), 
        asio::use_awaitable,
        pool
    );
    
    if (exception) {
        std::rethrow_exception(exception);
    }
    
    if (result) {
        co_return std::move(*result);
    }
    
    throw std::runtime_error("asio_future: Unknown error - no result and no exception");
}

} // namespace asio_future 