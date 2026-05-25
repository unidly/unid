// g++ -O3 -std=c++17 server.cpp -o server -DASIO_STANDALONE -pthread
#include <array>
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <thread>

using asio::ip::udp;

class UdpServer {
public:
  UdpServer(asio::io_context& io_context, short port)
      : io_context_(io_context),
        socket_(io_context, udp::endpoint(udp::v4(), port)) {

    std::cout << "Server listening on port " << port << "...\n";
    start_receive();
  }

private:
  void start_receive() {
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        [this](asio::error_code ec, std::size_t bytes_recvd) {
          if (!ec) {
            std::string message(recv_buffer_.data(), bytes_recvd);
            std::cout << "[Server Received from " << remote_endpoint_
                      << "]: " << message << std::endl;

            std::string reply = "Echo: " + message;
            send_reply(reply, remote_endpoint_);

            start_receive();
          } else if (ec != asio::error::operation_aborted) {
            std::cerr << "Server receive error: " << ec.message() << std::endl;
          }
        });
  }

  void send_reply(const std::string& message,
                  const udp::endpoint& target_client) {
    asio::post(io_context_, [this, message, target_client]() {
      bool write_in_progress = !reply_queue_.empty();
      reply_queue_.push({message, target_client});

      if (!write_in_progress) {
        start_write();
      }
    });
  }

  void start_write() {
    const auto& [message, target_client] = reply_queue_.front();

    socket_.async_send_to(
        asio::buffer(message), target_client,
        [this](asio::error_code ec, std::size_t /*bytes_transferred*/) {
          if (!ec) {
            reply_queue_.pop();
            if (!reply_queue_.empty()) {
              start_write();
            }
          } else {
            std::cerr << "Server send error: " << ec.message() << std::endl;
          }
        });
  }

  asio::io_context& io_context_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;

  struct ReplyItem {
    std::string data;
    udp::endpoint client;
  };
  std::queue<ReplyItem> reply_queue_;
  std::array<char, 1024> recv_buffer_;
};

int main() {
  try {
    asio::io_context io_context;

    auto work_guard = asio::make_work_guard(io_context);

    std::thread server_thread([&io_context]() { io_context.run(); });

    UdpServer server(io_context, 8080);

    std::cout << "Server running. Press Enter to terminate..." << std::endl;
    std::cin.get();

    std::cout << "Shutting down server..." << std::endl;
    io_context.stop();
    work_guard.reset();

    if (server_thread.joinable()) {
      std::cout << "Joining thread" << std::endl;
      server_thread.join();
    }
  } catch (std::exception& e) {
    std::cerr << "Server Exception: " << e.what() << std::endl;
  }

  return 0;
}
