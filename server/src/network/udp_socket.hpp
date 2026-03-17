/**
 * @file
 * @brief Connects to and uses a UDP socket for datagram transport
 *
 * (C)opyright 2026, Unidl, LLC. All rights reserved
 */

/**
 * @class Udp_socket
 */
class Udp_socket {
public:
  Udp_socket();
  ~Udp_socket();

  ssize_t sendto(int sockfd, const void buf[.len], size_t len, int flags,
                 const struct sockaddr *dest_addr, socklen_t addrlen);
  sendto(){} = 0;
}
