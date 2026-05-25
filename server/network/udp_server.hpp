/**
 * @file
 * @brief A UDP server class that uses ASIO for asynch operation..
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include "common/mempool.hpp"

#include <asio.hpp>
#include <cstddef>    // std::size_t
#include <functional> // std::function()

using asio::ip::udp;

/**
 * @class Udp_server
 *
 * Provides UDP socket i/o. Uses asio to manage the interface with the
 * socket.
 */
class Udp_server {
public:
  using Callback_type = std::function<void(const asio::error_code& error,
                                           std::size_t bytes_tranferred)>;

  /**
   * @brief Constructor
   *
   * TODO Add support for ipv6 addressing
   * TODO Replace "port" with "service" for use of fqdn and port
   *
   * @param io_context Asio event loop manager for asynch ops
   * @param port Port number
   * @param logger Handle for accessing the logger
   * @param configure Handle for accessing config information
   */
  Udp_server(asio::io_context& io_context, short port, Mempool& mempool,
             quill::Logger* logger);
  ~Udp_server();

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
   *
   */
  void async_receive();

  /**
   * @brief Start an asynch send operation
   *
   * Asio implements an asynchonous interface that allows the UDP server to
   * send a UPD datagram, and be notified when it has been sent. This private
   * member function is called by send_to() and then operates asychronously.
   *
   * When the data has been sent, a callback function is called
   *
   * There is no guarantee that the datagram was received.
   *
   * @note asynch_send_callback_ must be set prior to async_send()
   *
   * @param buffer The buffer containing the data to send
   * @param length The length of the data to be transmitted
   */
  void async_send(char* buffer, std::size_t length);

private:
  quill::Logger* logger_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  Mempool& mempool_;
  std::size_t chunk_size_;
};

#endif // UDP_SERVER_HPP
