
#include <array>
#include <asio.hpp>  // Use <boost/asio.hpp> if using Boost
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <iostream>

using asio::ip::udp;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    asio::io_context io_context;

    // Use a resolver to find the remote endpoint based on host and service name
    // ("daytime" or port "13")
    // The receiver endpoint is the address:port of the server
    udp::resolver resolver(io_context);
    udp::endpoint receiver_endpoint =
        *resolver.resolve(udp::v4(), argv[1], "13").begin();

    // Create a UDP socket
    udp::socket socket(io_context);
    socket.open(udp::v4());

    // Send a small message to initiate contact
    std::string send_buf = "Hello Server!";
    socket.send_to(asio::buffer(send_buf), receiver_endpoint);

    // Buffer to hold the response
    //asio::array<char, 512> recv_buf;
    std::array<char, 512> recv_buf;
    udp::endpoint sender_endpoint;

    // Wait to receive a response
    // The serder endpoint is the address:port of the client (this program)
    size_t len = socket.receive_from(asio::buffer(recv_buf), sender_endpoint);

    // Print the received message
    std::cout << "Received from server: ";
    std::cout.write(recv_buf.data(), len);
    std::cout << std::endl;

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}

