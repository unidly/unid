# Unid Protocol User Requirements

Unid servers and clients communicate with each other using a compact, binary
protocol. This is our application layer protocol, that uses QUIC and UDP to
implement the protocols required of the service.

User requirements are very simple:

* 100% reliability of getting the a Unid from a unid client
* Extremely low latency getting an Unid. An order of magnitude faster than a database query.(50ns vs 1000ns)
  * Assumes a shared memory access with the Unid Client on the same host as the application.

