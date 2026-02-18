# Data Serialization

## Functional design

Unid software is written in C++, and makes use of data structures that consist
of POD. The C++ compiler makes use of processor requirements for data
alignment by inserting padding into the structured data. Their is also the
issue of bigendian vs littleendian architectures that unfortunately make
the data structures not compatible without addressing this ordering.

The Unid software serializes the data before sending it over the network. The
serialization removes padding from the data structure, and also orders the
data where the most significant byte of a multi-byte datatype is in "network
order".

On the receiving node, the received data packet is returned to the order of
the receiving system's CPU architecture, and is padded to match the original
structure for the transmitting node.

## Detailed design

Serializing C++ data types and structures can be implemented using popular
libraries that take the data stored in a C++ structure and converting it to
a stream of bytes. The serialized data is sent over the network and received
by receiving application. It is then deserialized and stored or used as
needed.

* [FlatBuffers](https://flatbuffers.dev)
* [ProtocolBuffers](https://protobuf.dev/)
* [felixguendling/cista](https://github.com/felixguendling/cista)
* [capnproto/capnproto](https://github.com/capnproto/capnproto)

### Analysis



### Selection of serializer/deserializer for project
