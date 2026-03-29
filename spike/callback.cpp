#include <asio.hpp>
#include <functional>
#include <iostream>

// Define the type for the callback function
using TimerCallback = std::function<void(const asio::error_code &ec)>;

void perform_async_operation(asio::steady_timer &timer, TimerCallback handler) {
  // Use a lambda to wrap the std::function and pass it to Asio
  // The handler is captured by value in the lambda to ensure it persists
  // until the async operation completes.
  timer.async_wait([handler](const asio::error_code &ec) {
    // Inside the lambda, invoke the stored std::function handler
    handler(ec);
  });
}

int main() {
  asio::io_context io_context;

  // Create a timer
  asio::steady_timer timer(io_context, asio::chrono::seconds(2));

  // Define the client's actual callback function
  auto client_callback = [](const asio::error_code &ec) {
    if (!ec) {
      std::cout << "Timer expired!" << std::endl;
    } else {
      std::cout << "Error: " << ec.message() << std::endl;
    }
  };

  // Pass the client callback (as a std::function) to the operation
  perform_async_operation(timer, client_callback);

  // You must call io_context::run() for the callback to be invoked
  io_context.run();

  return 0;
}
