/**
 * @file
 * @brief A UDP client class that uses ASIO for asynchronous I/O.
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include "common/mempool.hpp"

#include <asio.hpp>
#include <cstddef>    // std::size_t
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
  /**
   * @brief Constructor
   *
   * TODO Add support for ipv6 addressing
   *
   * @param io_context Asio event loop manager for asynch ops
   * @param host IPv4 address or FQDN of the server
   * @param service Port number or well known service name
   * @param mempool Memory pool object
   * @param logger Global logger pointer
   */
  Udp_client(asio::io_context& io_context, const std::string& host,
             const std::string& service, Mempool& mempool,
             quill::Logger* logger);

  ~Udp_client();

  /**
   * @brief Start an asynch receive operaton
   *
   * This function allocates a buffer from the mempool, and then starts
   * an asynchronous receive operation. When data is received, a callback
   * function (lambda) gets the data and copies into the application
   * buffer that is passed to the "receiving" funtion through its callback.
   *
   * and then frees the allocated buffer back to the mempool
   *
   * Important note: This function is first called by the constructor. To
   * maintain the client in "receive" mode, it is important that the callback
   * handler restart "receive" mode by calling this function
   */
  void async_receive();

  /**
   * @brief Start an asynch send operation
   *
   * Asio implements an asynchonous interface that allows the UDP client to
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
  void async_send(char* buffer, size_t length);

  unsigned short get_remote_port() { return remote_endpoint_.port(); }

  std::string get_remote_address() {
    return remote_endpoint_.address().to_string();
  }

private:
  // Private member variables
  quill::Logger* logger_;         /**< Access to logger */
  asio::io_context& io_context_;  /**< Access to io_context */
  udp::socket socket_;            /**< UDP socket */
  udp::endpoint remote_endpoint_; /**< Server endpoint */
  Mempool& mempool_;              /**< Memory pool object */
  std::size_t chunk_size_;        /**< Size of a chunk buffer for rcv ops */
};

#endif // UDP_CLIENT_HPP
