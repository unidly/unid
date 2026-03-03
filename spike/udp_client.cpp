// Client side implementation of UDP client-server model

#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

/**
 * @brief UDP clas
 */
class Udp {
  public:
    Udp() {}
    ~Udp() {}

    /**
     * @brief Create a UDP socket
     *
     * @see [socket](https://man7.org/linux/man-pages/man2/socket.2.html)
     */
    void create_socket() {

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
     * @param buffer Storage for received datagram
     * @param cliaddr Client address
     */
    void recieve(char* buffer, struct sockaddr_in* cliaddr, socklen_t* len) {
      int message_len = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL,
          (struct sockaddr*)&cliaddr, len);

      buffer[message_len] = '\0';
    }

    /**
     * @brief Send a UDP datagram
     * @param[in] message C string containing messege to send
     * @param[in] message_len Length of C string
     * @param[out] cliaddr Client address structure
     */
    void send(const char* message, struct sockaddr_in* cliaddr, socklen_t len) {
      sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
          (const struct sockaddr*)&cliaddr, len);
    }

  private:
    int sockfd;
};


int main() {
  /*
  Udp udp;
  char buffer[MAXLINE];

  udp.create_socket();

  // Prepare address structure for use
  struct sockaddr_in servaddr;
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;                      // IPv4
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP
  servaddr.sin_port = htons(PORT);                    // Server port

  // Send client message
  socklen_t len = sizeof(servaddr);
  udp.send("Hello from client\n", &servaddr, len);

  // Await server message
  udp.recieve(buffer, &servaddr, &len);

  std::cout << "Server: " << buffer << std::endl;
  */

  int sockfd;
  char buffer[MAXLINE];
  const char* hello = "Hello from client";
  struct sockaddr_in servaddr;

  // Create UDP socket
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Fill server address info
  servaddr.sin_family = AF_INET;                      // IPv4
  servaddr.sin_port = htons(PORT);                    // Server port
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP

  socklen_t len = sizeof(servaddr);

  // Send message to server
  sendto(sockfd, hello, strlen(hello), MSG_CONFIRM,
         (const struct sockaddr*)&servaddr, sizeof(servaddr));
  printf("Hello message sent.\n");

  // Receive reply from server
  int n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL,
                   (struct sockaddr*)&servaddr, &len);

  buffer[n] = '\0';  // Null terminate received data
  printf("Server: %s\n", buffer);

  // Close socket
  close(sockfd);

  return 0;
}
