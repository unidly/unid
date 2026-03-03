# UPD

## Functional Design

UDP is a high speed IP transport layer protocol that is connectionless and
prioritizes speed and efficiency over reliability. The Unid software uses very
simple data architecture consisting of Plan Old Data (POD). It does not
communicate arrays, variable length strings, or large amounts of data, so UDP
is an ideal mechanism for tranfering the data between client, server, and
backup nodes.

In operation, a unid server may connect to hundreds or thousands of clients.
In order to manage the complexity of this, the asio c++ library is used to
provide an asynchronous connection to the clients. The 

* [Asio C++ Documentation](https://think-async.com/Asio/asio-1.36.0/doc/asio/overview/model.html)

The low-level interface to the UDP sockets is optimized to feed the QUIC
network layer with the sources and sinks of data to/from the client.
