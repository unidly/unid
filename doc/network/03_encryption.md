# UDP Encryption

## Functional design
Encryption hides the transmitted between nodes by encryption on the sender,
and decryption on the receiver. Without encryption, a malign actor can
intercept a datagram, and potential reverse engineer the content, and then
spoof the network with incorrect or disruptive datagrams.

* All data trasmitted over the internet is encrypted
* Encryption method - see Detailed Design
* Network level of encryption - see Detailed Design
* Key management - see Detailed Design

## Detailed design

Various protocols have been designed to enhance udp security. These include

* Datagram Transport Layer Security (DTLS)
* Quick UDP Internet Connections (QUIC)
* Internet Protocol Security (IPsec
* Application-level Encryption (AES-GCM for example)

### Analysis

#### DTLS

* Pros - uses TLS, a well known and supported protocols, lower latency than
TCP, takes care of packet ordering
* Cons - increased overhead per packet, increased complexity in handling
retransmission, redundant with Unid datagram protocols

Given the nature of Unid datagrams, the additional overhead to support the
protocol adds complexity that is unnecesary. The Unid network protocol
includes sequency and error detection within the datagram, so DTLS overhead
is not needed. The use of TLS is handled at the network layer, so this is a plus.

#### QUIC

* Pros - faster connection vs TCP (less handshaking), no head-of-line blocking,
built-in encryption, encryption faster than TLS, FEC and error detection
* Cons - less popular than DTLS, redundant with Unid error detection and re-
transmission protocls, increased overhead and complexity.

The use case for both QUIC and DTLS is to bridge the gap between TCP and UDP.
Unfortunately, this adds complexity in scenarios like Unid where the data
structures are small, and the sequencing and error detection is built in.

#### IPsec

IPsec is designed to allow VPN traffic to pass through NAT that would
otherwise block it. It includes strong encryption with both a Tunnel mode and
a Transport mode. This protocol is specific for VPN usage, and is inapplicable
for Unid.

#### AEGIS-256

AEGIS-256 is a modern AES-based cipher with unique properties making it easier
and saver to use than common alternatives.


* targets 256 bit confidentiality
* 256-bit authentication tags, ensuring collision resistance within a give
key; a tag can thus be used as a unique identifier for a message.
* It can safely encrypt a practically unlimited number of messages, without
any practical limits on their lengths.
* It has a very large nonce size (256 bits), allowing random nonces to be
used without any practical limits either.
* It has a better security margin than AES-GCM
* Leaking the state doesn’t leak the key
* It is assumed to be key-committing, preventing partitioning attacks affecting
other ciphers when used with low-entropy keys such as passwords. Namely, it is
difficult to find distinct keys and/or nonces that successfully verify the
same (ad, ciphertext, tag) tuple.
* AEGIS-256 is also extremely fast on recent CPUs with AES pipelines, with
lower memory usage than AES-GCM.
* However, on platforms without hardware AES support, it is slow and not
guaranteed to be protected against side channels. In that scenario,
XChaCha20-Poly1305 is a better choice.

Available through a library implementation:

* [libsodium](https://libsodium.gitbook.io/doc/secret-key_cryptography/aead/aes-256-gcm)

Critical rules:

* Never reuse an Initial Vector (IV) with the same key
* Always verify the authentication tag
* Use the recommened IV size

*** Decision

We select an implementation of AES-GCM as your encryption algorithm, simply
because it is well supported and known, it includes error detection, and it
is widely available through various open source projects.


*** Testing

We will test sample implementations using the 4 available libraries noted
above. We will select a library considering:

1. Performance
1. API simplicity
1. Popularity
1. Well documented, in the code and available documentation

