// gcc14-g++ unidserial.cpp -std=c++23 -O3 -lbenchmark -lpthread -o unidserial
/**
 * @file
 * @brief This demonstrates a serialization without a serializer library
 *
 * The purpose of this program is to understand the structure serialization
 * process, and the potential advantage to implement serializers as part of
 * the structure versus using a 3rd party library such as flatbuffers or
 * cap'n proto.
 */

#include <benchmark/benchmark.h>

#include <array>
#include <bit>  // std::bit_cast
#include <cassert>
#include <cstddef>  // std::byte and std::to_integer
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>  // std::invalid_argument

struct Unid_block {
  uint32_t ip;         /**< Destination IPv4 address **/
  uint16_t port;       /**< Destination port **/
  uint32_t seq;        /**< Sequence number **/
  uint32_t node;       /**< Node identifier **/
  uint64_t first_unid; /**< First unid in block **/
  uint32_t block_size; /**< Number of unids in block **/

  // Offsets for deserialize and the total buffer size needed
  static constexpr int ip_offset = 0;
  static constexpr int port_offset = sizeof(ip);
  static constexpr int seq_offset = port_offset + sizeof(port);
  static constexpr int node_offset = seq_offset + sizeof(seq);
  static constexpr int first_unid_offset = node_offset + sizeof(node);
  static constexpr int block_size_offset =
      first_unid_offset + sizeof(first_unid);
  static constexpr int buf_size = block_size_offset + sizeof(block_size);

  /**
   * @brief Serializes the structure into a buffer
   *
   * Efficently creates a serialized buffer containing the structure data. The
   * byte order in the buffer is network order (little-endian). Any padding in
   * the structure for alignment purposes is NOT serialized. This function
   * assumes the host architecture is Little-endian, so we byte swap where
   * needed.
   *
   * Caller can obtain the size of the serialized structure by calling
   * accessing the structure member value buf_size.
   *
   * @note Requires C++23 for byteswap()
   *
   * @return The serialized data buffer in network order (Big-endian)
   */
  std::array<std::byte, buf_size> serialize() {
    // Convert structure members to network order
    auto ip_no = std::byteswap(ip);
    auto port_no = std::byteswap(port);
    auto seq_no = std::byteswap(seq);
    auto node_no = std::byteswap(node);
    auto first_unid_no = std::byteswap(first_unid);
    auto block_size_no = std::byteswap(block_size);

    // Serialize structure members
    std::array<std::byte, sizeof(ip)> ip_arr =
        std::bit_cast<std::array<std::byte, 4>>(ip_no);

    std::array<std::byte, sizeof(port)> port_arr =
        std::bit_cast<std::array<std::byte, sizeof(port)>>(port_no);

    std::array<std::byte, sizeof(seq)> seq_arr =
        std::bit_cast<std::array<std::byte, sizeof(seq)>>(seq_no);

    std::array<std::byte, sizeof(node)> node_arr =
        std::bit_cast<std::array<std::byte, sizeof(node)>>(node_no);

    std::array<std::byte, sizeof(first_unid)> first_unid_arr =
        std::bit_cast<std::array<std::byte, sizeof(first_unid)>>(first_unid_no);

    std::array<std::byte, sizeof(block_size)> block_size_arr =
        std::bit_cast<std::array<std::byte, sizeof(block_size)>>(block_size_no);

    // Concatenate structure members into a results buffer
    std::array<std::byte, buf_size> result;

    auto it = std::copy(ip_arr.begin(), ip_arr.end(), result.begin());
    it = std::copy(port_arr.begin(), port_arr.end(), it);
    it = std::copy(seq_arr.begin(), seq_arr.end(), it);
    it = std::copy(node_arr.begin(), node_arr.end(), it);
    it = std::copy(first_unid_arr.begin(), first_unid_arr.end(), it);
    std::copy(block_size_arr.begin(), block_size_arr.end(), it);

    return result;
  }

  /**
   * @brief Deserializes a buffer into the structure
   *
   * Efficiently populates the structure with a serializes stream of bytes
   * provided in network order. This function assumes the host architecture
   * is Little-endian so byte swapping is used to convert network order to
   * Little-endian..
   *
   * @note Requires C++23 for byteswap()
   *
   * @param[in] buf Pointer to an allocated buffer
   * @param size[in] buf_size The size allocated (bytes)
   *
   * @return Number of bytes of serialized data in the buffer
   * @throws std::invalid_argument buf_size to small to  hold the data
   */
  void deserialize(const std::array<std::byte, buf_size>& buf) {
    // Test for correct buf_size
    if (buf.size() != buf_size) {
      throw std::invalid_argument(
          "deserialize() buf_size incorrect for struct Unid_block");
    }

    // Populate the structure data from the buffer
    // Buffers are always in network order
    std::memcpy(&ip, buf.data() + ip_offset, sizeof(ip));
    std::memcpy(&port, buf.data() + port_offset, sizeof(port));
    std::memcpy(&seq, buf.data() + seq_offset, sizeof(seq));
    std::memcpy(&node, buf.data() + node_offset, sizeof(node));
    std::memcpy(&first_unid, buf.data() + first_unid_offset,
                sizeof(first_unid));
    std::memcpy(&block_size, buf.data() + block_size_offset,
                sizeof(block_size));

    // Convert from network to Little-endian
    ip = std::byteswap(ip);
    port = std::byteswap(port);
    seq = std::byteswap(seq);
    node = std::byteswap(node);
    first_unid = std::byteswap(first_unid);
    block_size = std::byteswap(block_size);

    return;
  }

  void print() {
    std::cout << "----------------------\n";
    std::cout << "struct Unid_block data\n";
    std::cout << "----------------------\n";
    std::cout << "Ip: " << ip << std::endl;
    std::cout << "Port: " << port << std::endl;
    std::cout << "Seq: " << seq << std::endl;
    std::cout << "Node: " << node << std::endl;
    std::cout << "First unid: " << first_unid << std::endl;
    std::cout << "Block size: " << block_size << std::endl;
  }
};

void BM_MyFunction(benchmark::State& state, Unid_block ub1, Unid_block ub2) {
  std::array<std::byte, ub1.buf_size> buf;
  for (auto _ : state) {
    buf = ub1.serialize();
    ub2.deserialize(buf);
    benchmark::DoNotOptimize(buf);
  }
};

int main(int argc, char** argv) {
  // Serialize data from this block
  Unid_block ub1{
      0xc0a80001,  // Ip address - uint32 - 192.168.0.1 - +0
      44444,       // Port - uint16 - hex: 0xad9c - +4
      1234,        // Seq - uint32 - hex: 0x000004d2 - +6
      4321,        // Node id - uint32 - hex: 0x000010e1 - +10
      9876,        // First Unid - uint64 - hex: 0x0000000000002694 - +14
      6789         // Number of unids - uint32 - hex: 0x00001a85 - +22
  };
  // Deserialize data to this block
  Unid_block ub2{0, 0, 0, 0, 0};

  benchmark::Initialize(&argc, argv);
  benchmark::RegisterBenchmark("BM_UnidSerial", BM_MyFunction, ub1, ub2);

  benchmark::RunSpecifiedBenchmarks();

  return 0;
}
