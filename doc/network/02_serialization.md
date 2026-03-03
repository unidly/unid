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

An alternative for simple data structures is to implement serialization and
deserialization within the structure as member functions. Given that many
simple structures have fixed data lengths, constexpr can be used to caluclate
offset at compile time, and makes the serialization/deserialization processes
relatively simple.

### Analysis

Many of the modern c++ serialization libraries follow an approach that is
taken in Google's flatbuffers library. A schema file is generated that is
used by a separate compiler to generate code that is included in the main
program. The generated code provides methods to serialize the data, as well
as the extract data from the serialized buffer.

We benchmarked flatbuffer as a proxy for other modern software, such as
Cap`n Proto. We also implemented serialization and deserialization methods
in the simple structure that represents a Unid_block.

The unid block, like most unid structures, consists entirely of integers
types of fixed length.

### Selection of serializer/deserializer for project

When running benchmark tests, the flatbuffer implementation required almost
1ms to serialize and serialize the structure. The hand-creafted version took
around 1ns to perform the same operation. For simple structures that consist
of fixed length integer paramenters, we will use the hand-crafted
member functions to perform the tasks.
