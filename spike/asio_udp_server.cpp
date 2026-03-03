
#include <array>
#include <asio.hpp>  // Use <boost/asio.hpp> if using Boost
#include <ctime>
#include <iostream>
#include <string>

using asio::ip::udp;

// Function to generate the current time string
std::string make_daytime_string() {
  std::time_t now = std::time(0);
  return std::ctime(&now);
}

int main() {
  try {
    // All ASIO operations need an io_context
    asio::io_context io_context;

    // Create a UDP socket and bind it to listen on any IPv4 address on port 13
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    std::cout << "UDP Server listening on port 13..." << std::endl;

    for (;;) {
      // Buffer to receive data from a client (UDP packets have a maximum size,
      // 512 bytes is a common default for simple examples)
      // asio::array<char, 512> recv_buf;
      std::array<char, 512> recv_buf;
      // Endpoint object that will be populated with the sender's details
      udp::endpoint remote_endpoint;
      asio::error_code error;

      // Wait to receive data
      socket.receive_from(asio::buffer(recv_buf), remote_endpoint, 0, error);

      if (error && error != asio::error::message_size) {
        throw asio::system_error(error);
      }

      // Generate the response message
      std::string message = make_daytime_string();

      // Send the response back to the client that sent the data
      // The remote endpoint is the client address:port
      asio::error_code ignored_error;
      socket.send_to(asio::buffer(message), remote_endpoint, 0, ignored_error);

      std::cout << "Sent message to " << remote_endpoint.address().to_string()
                << ":" << remote_endpoint.port() << std::endl;
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}

