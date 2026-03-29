/**
 * @file
 * @brief A UDP server class that uses ASIO for asynch operation..
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "asio.hpp"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <cstdlib>
#include <functional>
#include <vector>

using asio::ip::udp;

/**
 * @class Udp_server
 *
 * Provides UDP socket i/o. Uses asio to manage the asynchronous interface
 * with the socket.
 */
class Udp_server {
public:
  using Callback_type = std::function<void(const asio::error_code &error,
                                           std::size_t bytes_tranferres)>;

  Udp_server(asio::io_context &io_context, short port);

  ~Udp_server();

  /**
   * @brief Set a callback for the receive datagram
   *
   * This callback is called when a datagram has been recieved.
   *
   * The caller can pass on the data and free the buffer
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_receive_callback(Callback_type cb) {
    async_receive_callback_ = cb;
  }

  /**
   * @brief Set a callback for the transmit datagram
   *
   * This callback is called when the datagram has been sent.
   *
   * The caller can pass on the data and free the buffer
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_send_callback(Callback_type cb) { async_send_callback_ = cb; }

  /**
   * @brief Receive a udp datagram from a client
   *
   * The caller supplies a buffer with its length to the Udp Server. The
   * Server awaits an incomming datagram from the client. When the datagram
   * arrives, the Server places the data in the buffer, and returns via
   * reference th buffer, and the structure containing the client's address
   * information. The return value is the number of bytes of received data in
   * the buffer.
   *
   * Note that this implementation is asynchronous. After making a call to
   * this function, the application can continue doing other things. A
   * callback is made when the received data is available to the original
   * caller.
   *
   * Note that if the datagram is larger than the buffer, then the data is
   * truncated at the buffer length and the remaining data is lost forever.
   *
   * @param buffer A buffer supplied by the caller
   * @param length The size of the buffer
   * @param sockaddr_in The address info of the client that sent the data
   *
   * @return The number of bytes received
   */
  ssize_t receive_from(char *buffer, std::uint16_t length,
                       struct sockaddr_in &client_address);

  /**
   * @brief Send a udp datagram to a client
   *
   * The server sends a datagram to a client using this function. A
   * buffer is provided that contains the data, along with the length of the
   * data in bytes. The datagram is sent to the client address and port as
   * specified in the sockaddr_in structure. The name sockaddr_in is used as
   * the same structure is provided to the server when receiving a client
   * datagram (see receive_from()).
   *
   * @param buffer A caller supplied buffer with data to send
   * @para length The length of data in bytes
   * @param client_address The address info required to deliver the datagram
   *
   * @return The number of bytes sent, or -1 for error
   */
  ssize_t send_to(const char *buffer, std::uint16_t length,
                  const struct sockaddr_in &client_address);

  /**
   * @ brief Closes the socket
   *
   * This function closes the socket. The socket must be closed to prevent
   * resource leaks. Note that ASIO automatically closes the socket in
   * the destructor. This call is available should the application chose to
   * close the socket independent of class destructor.
   *
   * This server uses ASIO that creates and manages the socket. The socket is
   * closed by calling the socket.close method. Since the server uses async
   * programming, this function handles the closure of the system socket along
   * with handling the object's lifetime correctly so it is only destroyed
   * after all handlers have completed.
   */
  void close_socket();

private:
  /**
   * @ brief Start an asynch receive operaton
   *
   * Asio implements an asynchronous interface that allows the UDP server
   * to wait for a datagram from a client. When a datagram is recieved, a
   * callback function notifies the caller who consumes the data and frees
   * the memory chunk container of the data.
   */
  void async_receive();

  /**
   * @brief Start an asynch send operation
   *
   * Asio implements an asynchonous interface that allows the UDP serve to
   * send a UPD datagram, and be notified when it has been sent.
   */
  void async_send(std::size_t length);

  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  Callback_type async_receive_callback_;
  Callback_type async_send_callback_;

  // @TODO The data buffers are provided by the application in the final
  // design. A buffer pool is implemented and the async sendto and
  // recieve_from functions provide the buffers used by this server
  char *rx_buffer_;
  const char *tx_buffer_;
  std::uint16_t rx_max_length_; // UDP max datagram size fits uint16_t
  std::uint16_t tx_max_length_; // UDP max datagram size fits uint16_t
};

#endif // UDP_SERVER_HPP
