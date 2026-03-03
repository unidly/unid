// Server side implementation of UDP client-server model
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

/**
 * @brief UDP clas
 */
class Udp {
 public:
  Udp() {}
  ~Udp() { close(sockfd); }

  /**
   * @brief Create a UDP socket
   *
   * @see [socket](https://man7.org/linux/man-pages/man2/socket.2.html)
   */
  void create_udp_socket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }
  }

  /**
   * @brief Bind the UDP socket to the server's address
   *
   * @see [bind](https://man7.org/linux/man-pages/man2/bind.2.html)
   *
   * @param[in] servaddr Server's socket address structure
   * @param servaddr_size Size of server's socket address structure
   */
  void bind_socket(const struct sockaddr_in* servaddr, socklen_t addrlen) {
    if (bind(sockfd, (const struct sockaddr*)servaddr, addrlen) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
  }

  /**
   * @brief Recieve a UDP datagram
   *
   * Blocks on entry until a message is available on the port.
   *
   * @param buffer Storage for received datagram
   * @param cliaddr Client address
   */
  void recieve(char* buffer, struct sockaddr_in* cliaddr, socklen_t len) {
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                               (struct sockaddr*)cliaddr, &len);
    buffer[n] = '\0';
  }

  /**
   * @brief Send a UDP datagram
   * @param[in] message C string containing messege to send
   * @param[in] message_len Length of C string
   * @param[out] cliaddr Client address structure
   */
  void send(const char* message, int message_len, struct sockaddr_in* cliaddr, socklen_t len) {
    std::cout << "Message: " << message << std::endl;
    std::cout << "Message length: " << message_len << std::endl;
    sendto(sockfd, message, message_len, MSG_CONFIRM,
           (const struct sockaddr*)cliaddr, len);
  }

  int get_sockfd() {
    return sockfd;
  }

 private:
  int sockfd;
};


int main() {
  Udp udp;
  char buffer[MAXLINE];
  const char *hello = "Hello from server";

  udp.create_udp_socket();

  // Prepare address structures for use
  struct sockaddr_in servaddr, cliaddr;    // Use for UDP
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  servaddr.sin_family = AF_INET;           // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;   // Server listen on any address
  servaddr.sin_port = htons(PORT);         // Server listen port

  socklen_t addrlen = sizeof(servaddr);
  udp.bind_socket(&servaddr, addrlen);

  // Await client messge
  int sockfd = udp.get_sockfd();
  socklen_t len = sizeof(cliaddr);
  udp.recieve(buffer, &cliaddr, len);
  std::cout << "Client: " << buffer << std::endl;

  // Respond
  int msg_len = strlen(hello);
  udp.send(hello, msg_len, &cliaddr, len);
  std::cout << "Hello message sent." << std::endl;

  return 0;
}


/*
// Driver code
int main() {
  int sockfd;
  char buffer[MAXLINE];
  const char *hello = "Hello from server";
  struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;  // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  socklen_t len = sizeof(cliaddr);  // len is value/result

  int n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
               (struct sockaddr *)&cliaddr, &len);
  buffer[n] = '\0';
  printf("Client : %s\n", buffer);
  sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr *)&cliaddr, len);
  std::cout << "Hello message sent." << std::endl;

  return 0;
}
*/
