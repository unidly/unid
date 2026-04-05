/**
 * @file
 * @brief A UDP client class that uses ASIO for asynchronous I/O.
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "asio.hpp"

#include <cstdint>    // uintXX_t
#include <functional> // std::function()

using asio::ip::udp;

/**
 * @class Udp_client
 *
 * Provides UDPsocket i/o. Uses asio to manage the interface with the
 * socket.
 */
class Udp_client {
public:
  using Callback_type = std::function<void(const asio::error_code &error,
                                           std::size_t bytes_transferred)>;

  /**
   * @brief Constructor
   *
   * TODO Add support for ipv6 addressing
   *
   * @param io_context Asio event loop manager for asynch ops
   * @param host IPv4 address of the server
   * @param port Well known service port name or port number
   */
  Udp_client(asio::io_context &io_context, const std::string &host,
             const std::string &service);

  ~Udp_client();

  /**
   * @brief Set a callback for the receive datagram
   *
   * This callback is called when a datagram has been received. The caller
   * passes the data on, and takes care of buffer management as needed
   * (typically retirns it to the buffer pool).
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_receive_callback(Callback_type cb) {
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
   * it was sent!
   *
   * @param cb The callback function, lambda or functor
   */
  void set_async_send_callback(Callback_type cb) { async_send_callback_ = cb; }

  /**
   * @brief Set up to receive a udp datagram
   *
   * The caller supplies a buffer with its length to the Udp Client, and
   * starts an anynchrouous receive operation by calling asynch_receive().
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
   * @brief Set up to send a udp datagram
   *
   * The caller supplies a buffer with data and the length of the data to
   * the UDP Client. The destination is also provide with the socaddr_in
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
   * to wait for a datagram from a client. When a datagram is recieved, a
   * callback function notifies the caller who consumes the data and frees
   * the memory chunk container of the data.
   *
   * The receve function is (re)started whenever a call to receive_from is
   * made. It is not started by the constructor.
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
  asio::io_context &io_context_;
  udp::socket socket_;
  udp::endpoint receiver_endpoint_;
  Callback_type async_receive_callback_;
  Callback_type async_send_callback_;

  char *rx_buffer_;       /**< Pointer to the supplied receive data buffer */
  const char *tx_buffer_; /**< Pointer to the supplied transmit data buffer */
  std::size_t rx_length_; /**< Total length of rx buffer */
  std::size_t tx_length_; /**< Actual length of tx data */
};

#endif // UDP_CLIENT_HPP
