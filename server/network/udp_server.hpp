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

#include <cstddef>    // std::size_t
#include <functional> // std::function()

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
                                           std::size_t bytes_tranferred)>;

  /**
   * @brief Constructor
   *
   * TODO Add support for ipv6 addressing
   *
   * @param io_context Asio event loop manager for asynch ops
   * @param port Port number
   */
  Udp_server(asio::io_context &io_context, short port, quill::Logger *logger);
  ~Udp_server();

  /**
   * @brief Returns the local endpoint address of the socket
   *
   * The local endpoint contains address and port of server's local socket.
   *
   * @return The local endpoint port
   */
  std::string get_local_address() {
    return local_endpoint_.address().to_string();
  }

  /**
   * @brief Returns the local endpoint port of the socket
   *
   * The local endpoint contains address and port of server's local socket.
   *
   * @return The local endpoint port
   */
  short get_local_port() { return local_endpoint_.port(); }

  /**
   * @brief Set a callback for the receive datagram
   *
   * This callback is called when a datagram has been recieved.
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_receive_callback(Callback_type cb) {
    LOG_INFO(Udp_server::logger_, "Receive callback set");
    async_receive_callback_ = cb;
  }

  /**
   * @brief Set a callback for the send datagram
   *
   * This callback is called when the datagram has been sent. The caller
   * passes the status on, and takes care of buffer management as needed
   * (typically returns it to the buffer pool).
   *
   * The callback does not indicate that the datagram was received, only that
   * it was send!
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_send_callback(Callback_type cb) {
    LOG_INFO(Udp_server::logger_, "Send callback set");
    async_send_callback_ = cb;
  }

  /**
   * @brief Setup to receive a udp datagram
   *
   * The caller supplies a buffer with its length to the Udp Server, and
   * starts an anynchrouous receive operation by calling receiver_from().
   *
   * When asynch_receive() receives a datagram, it executes a callback to
   * the caller with the data and address information.
   *
   * The caller provides the buffer for storing the datagram. The caller must
   * not freee or destroy the buffer until it is returned to the callback.
   *
   * Note that if the datagram is larger than the buffer, then the data is
   * truncated at the buffer length and the remaining data is lost forever.
   * The caller must take care to provide buffers large enough to receive
   * the datagram.
   *
   * @param buffer A buffer supplied by the caller
   * @param length The size of the buffer
   */
  void receive_from(char *buffer, std::size_t length);

  /**
   * @brief Setup to send a udp datagram
   *
   * The server sends a datagram to a client using this member function. A
   * buffer is provided that contains the data, along with the length of the
   * data in bytes. The destination is also provide with the socaddr_in
   * member structure.
   *
   * This member function calls asynch_send() that is responsible for
   * sending the buffer data to the client. When asynch_send() is done, it
   * calls the callback to notify the caller.
   *
   * @param buffer A caller supplied buffer with data to send
   * @para length The length of data in bytes
   * @param client_address The address info required to deliver the datagram
   */
  void send_to(const char *buffer, std::size_t length,
               const struct sockaddr_in &client_address);

private:
  // Private member functions
  /**
   * @ brief Start an asynch receive operaton
   *
   * Asio implements an asynchronous interface that allows the UDP server
   * to wait for a datagram from a client. When a datagram is received, a
   * callback function notifies the caller who consumes the data and frees
   * the memory chunk container of the data.
   *
   * The receve function is started when the first call to receive_from is
   * made. The receive callback function restarts async_receive() with
   * a free buffer before processing or transferring the received data.
   *
   * @note asynch_receive_callback_ must be set prior to async_receive()
   */
  void async_receive();

  /**
   * @brief Start an asynch send operation
   *
   * Asio implements an asynchonous interface that allows the UDP serve to
   * send a UPD datagram, and be notified when it has been sent. This private
   * member function is called by send_to() and then operates asychronously.
   *
   * When the data has been sent, a callback function is called. The caller
   * program is notified that the datagram was sent.
   *
   * There is no guarantee that the datagram was received. The application
   * is responsible for acknowledgement that the data was received by the
   * client via a data layer protocol.
   *
   * @note asynch_send_callback_ must be set prior to async_send()
   *
   * @param length The length of the data to be transmitted
   */
  void async_send();

  // Private member variables
  // @TODO The data buffers are provided by the application in the final
  // design. A buffer pool is implemented and the async sendto and
  // recieve_from functions provide the buffers used by this server
  quill::Logger *logger_;
  udp::socket socket_;
  udp::endpoint local_endpoint_;
  Callback_type async_receive_callback_;
  Callback_type async_send_callback_;

  char *rx_buffer_;       /**< Pointer to the supplied receive data buffer */
  const char *tx_buffer_; /**< Pointer to the supplied transmit data buffer */
  std::size_t rx_length_; /**< Total length of rx buffer */
  std::size_t tx_length_; /**< Actual length of tx data */
};

#endif // UDP_SERVER_HPP
