/**
 * @file
 * @brief Simple performance benchmarks using Google's FlatBuffers library
 */

#include <iostream>

#include "unid_block_generated.h"

struct Unid_block {
  uint32_t ip_addr = 0xc0a80001;  // 192.168.0.1
  uint16_t port = 44444;
  uint32_t sequence = 1234;
  uint32_t node = 4321;
  uint64_t unid = 9876;
  uint32_t size = 6789;
};

int main() {
  // Used to build the flatbuffer
  flatbuffers::FlatBufferBuilder builder;

  // Unis block with defaul dat
  Unid_block unid_block;

  // Auto-generated function emitted from `flatc` and the input
  // `unid_block.fbs` schema
  auto unid_block_t = CreateUnid_block_t(
      builder, unid_block.ip_addr, unid_block.port, unid_block.sequence,
      unid_block.node, unid_block.unid, unid_block.size);

  // Finalize the buffer.
  builder.Finish(unid_block_t);

  // Get pointer to the flatbuffer
  const uint8_t* flatbuffer = builder.GetBufferPointer();

  const Unid_block_t* ub = GetUnid_block_t(flatbuffer);

  // Display the deserialized data
  std::cout << "IP address (binary): " << ub->ip_addr() << std::endl;
  std::cout << "Port: " << ub->port() << std::endl;
  std::cout << "Sequence: " << ub->sequence() << std::endl;
  std::cout << "Node: " << ub->node() << std::endl;
  std::cout << "Unid: " << ub->unid() << std::endl;
  std::cout << "Size: " << ub->size() << std::endl;
}
