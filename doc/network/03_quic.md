# QUIC Transport Protocol

QUIC is a modern, UDP-based, secure, and multiplexed transport protocol
designed for low latency, featuring integrated TLS 1.3 encryption and
connection-level congestion control. Standardized by the IETF in RFC 9000, it
enables faster connection establishment (0-RTT), independent streams to
eliminate head-of-line blocking, and seamless path migration, primarily
powering HTTP/3.

IETF Standards:

* [RFC 9000 (Transport)](https://www.rfc-editor.org/info/rfc9000)
* [RFC 9001 (TLS)](https://www.rfc-editor.org/info/rfc9001)
* [RFC 9002 (Loss Detection)](https://www.rfc-editor.org/info/rfc9002)
* [RFC 9308 (Applicability)](https://www.rfc-editor.org/info/rfc9308)

Specifications:

* [QUIC Wire Layout Specification](https://docs.google.com/document/d/1WJvyZflAO2pq77yOLbp9NsGjC1CHetAXV8I0fQe-B_U/edit?tab=t.0#heading=h.o9jvitkc5d2g()

Implementations:

* [Quiche (Google)](https://github.com/google/quiche)
* [Abseil Quiche dependency](https://github.com/abseil/abseil-cpp)
* [MsQuic (Microsoft)](https://microsoft.github.io/msquic/msquicdocs/docs/API.html)
* [TQUIC](https://github.com/tencent/tquid)
* [TQUIC (Rust w/ C API](tquic.net)

## Functional design

QUIC provides a standards based way of interfacing over the internet using UDP
as the tranport mechanism. It takes care off connections, encryption, flow
control and other mechanisms to providing fast and reliable connection between
a server and client.

QUIC is a required protocol for HTTP3, and currently takes care of 25% to 30%
if global internet traffic. Google, Facebook and YouTube are the driving
forces in this new protocol.

Why use QUIC?

* Proven protocol powering the next genneration web (HTTP3)
* More performant and quicker that TCP/IP
* Handles encryption, acks, retransmissions, etc.
* Available with supported proven libraries

Which implementation?

Candidate implementations are Google's Quiche and Tencents TQUIC. These have
different approaches regarding integeration. With Quiche, their is no API
inteface and the code must be incorporated directly into the Unid software
with a custom API. With TQUIC, an API provides a more direct implementation
but the underlying question is ongoing support and evolution of the TQUIC
software.

Note that TQUIC was developed by Tencent. Tencent is a massive Chinese
multinational technology and entertainment conglomerate. Tencent is
recognized as the world's largest video game vendor.

In order to decide, we need to work with both solutions enough to understand
the pros and cons of these approaches. At a minimum, we need to develop
functional prototypes of each with simple toy implementations. Understanding
that the Google implementation is used within their infrastructure, it has
an advantage as it will continue to be maintained for use by Google. We need
to understand the path for TQUIC and its longer term viability.

## Detailed design

### C++ API for the Google Quiche Library

Developing a c++ API is best achieved by leveraging the libraries built-in,
thin C API via FFI (--features ffi in Cargo) to produce a static libquiche.a
library.

Our approach

* Leverage Existing C FFI
* Generate the static C library and header files
* Implement the platform dependent functionality (UDP transport layer and the
event driven loop gto manage packet i/o (see [Fuchsia documentation](https://fuchsia.googlesource.com/third_party/rust-mirrors/quiche/+/refs/tags/tokio-quiche-0.9.0).
* Review Chromium and Envoy source code for ways to wrap the library for their
respective architectures.
* Wrap the C functions in RAII-compliant C++ classes to manage the lifecycle
of quiche_config, quiche_conn, and packet data.

Step 1) Build development environment and get toy example working
Step 2) Implement the UDP socket interface



## AI Notes

[Using Google Quiche Implementation Advice](https://share.google/aimode/dGgDDsTBeqZq4pupI)

