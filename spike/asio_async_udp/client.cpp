// g++ -O3 -std=c++17 client.cpp -o client -DASIO_STANDALONE -pthread
#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>

using asio::ip::udp;

class UdpClient {
public:
  UdpClient(asio::io_context& io_context, const std::string& host,
            const std::string& port)
      : io_context_(io_context), socket_(io_context, udp::v4()) {

    udp::resolver resolver(io_context_);
    remote_endpoint_ = *resolver.resolve(udp::v4(), host, port).begin();

    start_receive();
  }

  void send(const std::string& message) {
    asio::post(io_context_, [this, message]() {
      bool write_in_progress = !write_queue_.empty();
      write_queue_.push(message);

      if (!write_in_progress) {
        start_write();
      }
    });
  }

private:
  void start_write() {
    socket_.async_send_to(
        asio::buffer(write_queue_.front()), remote_endpoint_,
        [this](asio::error_code ec, std::size_t /*bytes_transferred*/) {
          if (!ec) {
            write_queue_.pop();
            if (!write_queue_.empty()) {
              start_write();
            }
          } else {
            std::cerr << "Send error: " << ec.message() << std::endl;
          }
        });
  }

  void start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), sender_endpoint_,
        [this](asio::error_code ec, std::size_t bytes_recvd) {
          if (!ec) {
            std::string message(recv_buffer_.data(), bytes_recvd);
            std::cout << "\n[Client Received from " << sender_endpoint_
                      << "]: " << message << std::endl;

            start_receive();
          } else if (ec != asio::error::operation_aborted) {
            std::cerr << "Receive error: " << ec.message() << std::endl;
          }
        });
  }

  asio::io_context& io_context_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  udp::endpoint sender_endpoint_;

  std::queue<std::string> write_queue_;
  std::array<char, 1024> recv_buffer_;
};

int main() {
  try {
    asio::io_context io_context;

    auto work_guard = asio::make_work_guard(io_context);

    std::thread network_thread([&io_context]() { io_context.run(); });

    // Target localhost on port 8080
    UdpClient client(io_context, "127.0.0.1", "8080");

    std::cout << "Client sending messages..." << std::endl;
    client.send("Hello Standalone Server!");
    client.send("Testing the async loop logic.");

    std::this_thread::sleep_for(std::chrono::seconds(3));

    std::cout << "Shutting down client..." << std::endl;
    work_guard.reset();
    if (network_thread.joinable()) {
      network_thread.join();
    }
  } catch (std::exception& e) {
    std::cerr << "Client Exception: " << e.what() << std::endl;
  }

  return 0;
}
