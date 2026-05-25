#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

using boost::asio::ip::udp;

class UdpClient {
public:
  UdpClient(boost::asio::io_context& io_context, const std::string& host,
            const std::string& port)
      : io_context_(io_context), socket_(io_context, udp::v4()) {

    // Resolve the server address
    udp::resolver resolver(io_context_);
    remote_endpoint_ = *resolver.resolve(udp::v4(), host, port).begin();
  }

  // Public API to send data from any thread
  void send(const std::string& message) {
    // Post the work to the io_context thread to ensure thread safety
    boost::asio::post(io_context_, [this, message]() {
      bool write_in_progress = !write_queue_.empty();
      write_queue_.push(message);

      if (!write_in_progress) {
        start_write();
      }
    });
  }

private:
  void start_write() {
    // UDP sends the entire string at once using async_send_to
    socket_.async_send_to(
        boost::asio::buffer(write_queue_.front()), remote_endpoint_,
        [this](boost::system::error_code ec,
               std::size_t /*bytes_transferred*/) {
          if (!ec) {
            write_queue_.pop();

            // If more messages are waiting, keep writing
            if (!write_queue_.empty()) {
              start_write();
            }
          } else {
            std::cerr << "Send error: " << ec.message() << std::endl;
          }
        });
  }

  boost::asio::io_context& io_context_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::queue<std::string>
      write_queue_; // Managed strictly inside the io_context thread
};

int main() {
  try {
    boost::asio::io_context io_context;

    auto work_guard = boost::asio::make_work_guard(io_context);

    std::thread network_thread([&io_context]() { io_context.run(); });

    // Initialize the client (Targeting localhost on port 8080)
    UdpClient client(io_context, "127.0.0.1", "8080");

    std::cout << "Sending messages and listening for replies..." << std::endl;
    client.send("Hello Server!");
    client.send("Can you hear me?");

    // Keep main thread alive to see incoming responses
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Shutting down..." << std::endl;
    work_guard.reset();
    if (network_thread.joinable()) {
      network_thread.join();
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
