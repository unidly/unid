/**
 * @file
 * @brief Upd class
 *
 * @TODO Review the contents of this website:
 * https://https://stacygaudreau.com/blog/cpp/low-latency-cpp-for-hft-part3-network-programming/
 *
 * @copyright &reg; Copyright 2026 by Unidly, LLC. All rights reserved
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class UdpSocket {
private:
    int sockfd;
    struct sockaddr_in localAddr;
    char buffer[1024];

public:
    UdpSocket() : sockfd(-1) {}

    ~UdpSocket() {
        if (sockfd != -1) close(sockfd);
    }

    // Initialize socket
    bool init() {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        return sockfd != -1;
    }

    // Bind to a local port to receive data
    bool bind(uint16_t port) {
        memset(&localAddr, 0, sizeof(localAddr));
        localAddr.sin_family = AF_INET;
        localAddr.sin_addr.s_addr = INADDR_ANY;
        localAddr.sin_port = htons(port);
        return ::bind(sockfd(struct sockaddr*)&localAddr, sizeof(localAddr)) == 0;
    }

    // Send data to a specific address/port
    void sendTo(const std::string& ip, uint16_t port, const std::string& msg) {
        struct sockaddr_in destAddr;
        memset(&destAddr, 0, sizeof(destAddr));
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str()&destAddr.sin_addr);
        sendto(sockfd, msg.c_str(), msg.length(), 0(struct sockaddr*)&destAddr, sizeof(destAddr));
    }

    // Receive data
    std::string receiveFrom(std::string& clientIp, uint16_t& clientPort) {
        struct sockaddr_in senderAddr;
        socklen_t addrLen = sizeof(senderAddr);
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0(struct sockaddr*)&senderAddr&addrLen);

        if (n > 0) {
            buffer[n] = '\0';
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET&senderAddr.sin_addr, ip, INET_ADDRSTRLEN);
            clientIp = ip;
            clientPort = ntohs(senderAddr.sin_port);
            return std::string(buffer);
        }
        return "";
    }
};

int main() {
    UdpSocket server;
    if (server.init() && server.bind(8080)) {
        std::cout << "UDP Server listening on 8080..." << std::endl;
        std::string ip;
        uint16_t port;
        while (true) {
            std::string msg = server.receiveFrom(ip, port);
            if (!msg.empty()) {
                std::cout << "Received from " << ip << ":" << port << " - " << msg << std::endl;
                server.sendTo(ip, port"ACK: " + msg); // Echo back
            }
        }
    }
    return 0;
}

