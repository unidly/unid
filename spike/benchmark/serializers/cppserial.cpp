// Compile as gcc14-g++ cppserial.cpp -std=c++23 -o cppserial
/**
 * @file
 * @brief This demonstrates a serialization without a serializer library
 *
 * The purpose of this program is to understand the structure serialization
 * process, and the potential advantage to implement serializers as part of
 * the structure versus using a 3rd party library such as flatbuffers or
 * cap'n proto.
 */

#include <array>
#include <bit>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>

struct Unid_block {
  uint32_t ip;       /**< Destination IPv4 address **/
  uint16_t port;     /**< Destination port **/
  uint32_t sequence; /**< Sequence number **/
  uint32_t node;     /**< Node identifier **/
  uint64_t unid;     /**< First unid in block **/
  uint32_t nunids;   /**< Number of unids in block **/

  /**
   * @brief Returns the size in bytes of the serialized stream
   */
  static constexpr uint16_t stream_size = sizeof(uint32_t) + sizeof(uint16_t) +
                                          sizeof(uint32_t) + sizeof(uint32_t) +
                                          sizeof(uint64_t) + sizeof(uint32_t);
  constexpr uint16_t get_stream_size() { return stream_size; }

  /**
   * @brief Serializes the structure into a caller supplied buffer
   *
   * Efficently creates a serialized buffer containing the structure data. The
   * byte order in the buffer is network order (little-endian). Any padding in
   * the structure for alignment purposes is NOT serialized.
   *
   * Best performance occurs if the caller buf is aligned on a 16-byte boundary.
   * Create the buffer as follows:
   *     alignas(16) std::array<std::byte, buf_size> buf;
   *
   * Caller can obtain the size of the serialized structure by calling
   * get_stream_size().
   *
   * @note Requires C++23
   * @note The caller supplied buffer maximum size is 65536 bytes
   *
   * @param[in,out] buf Pointer to an allocated buffer
   * @param size[in] buf_size The size allocated (bytes)
   *
   * @return Number of bytes of serialized data in the buffer
   * @throws std::invalid_argument buf_size != stream_size
   */
  std::uint16_t serialize(std::array<std::byte, stream_size>& buf,
                          const std::uint16_t buf_size) {
    // Test for buf_size too small
    if (buf_size != stream_size) {
      throw std::invalid_argument(
          "serialize() buf_size incorrect for  struct Unid_block");
    }

    // Convert structure data to network order if needed
    uint16_t buf_index = 0;
    if constexpr (std::endian::native == std::endian::little) {
      auto ip_no = std::byteswap(ip);
      auto port_no = std::byteswap(port);
      auto sequence_no = std::byteswap(sequence);
      auto node_no = std::byteswap(node);
      auto unid_no = std::byteswap(unid);
      auto nunids_no = std::byteswap(nunids);

      // Serialize structure members
      std::array<std::byte, sizeof(std::uint32_t)> ip_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(ip_no);
      std::array<std::byte, sizeof(std::uint16_t)> port_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint16_t)>>(port_no);
      std::array<std::byte, sizeof(std::uint32_t)> sequence_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(
            sequence_no);
      std::array<std::byte, sizeof(std::uint32_t)> node_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(node_no);
      std::array<std::byte, sizeof(std::uint64_t)> unid_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint64_t)>>(unid_no);
      std::array<std::byte, sizeof(std::uint32_t)> nunids_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(nunids_no);

      // Populate the buffer
      for (auto byte : ip_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : port_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : sequence_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : node_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : unid_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : nunids_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
    } else {
      // Serialize structure members
      std::array<std::byte, sizeof(std::uint32_t)> ip_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(ip);
      std::array<std::byte, sizeof(std::uint16_t)> port_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint16_t)>>(port);
      std::array<std::byte, sizeof(std::uint32_t)> sequence_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(
            sequence);
      std::array<std::byte, sizeof(std::uint32_t)> node_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(node);
      std::array<std::byte, sizeof(std::uint64_t)> unid_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint64_t)>>(unid);
      std::array<std::byte, sizeof(std::uint32_t)> nunids_bytes =
        std::bit_cast<std::array<std::byte, sizeof(std::uint32_t)>>(nunids);

      // Populate the buffer
      uint16_t buf_index = 0;
      for (auto byte : ip_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : port_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : sequence_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : node_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : unid_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
      for (auto byte : nunids_bytes) {
        buf[buf_index] = byte;
        ++buf_index;
      }
    }

    return buf_index;
  }

  /**
   * @brief Deserializes a buffer into the structure
   *
   * Efficiently populates the structure with a serializes stream of bytes
   * provided in network order.
   *
   * Best performance occurs if the caller buf aligned on a 16-byte boundary.
   * Create the buffer as follows:
   *     alignas(16) std::array<std::byte, buf_size> buf;
   *
   * @note Requires C++23
   * @note The caller supplied buffer maximum size is 65536 bytes
   *
   * @param[in] buf Pointer to an allocated buffer
   * @param size[in] buf_size The size allocated (bytes)
   *
   * @return Number of bytes of serialized data in the buffer
   * @throws std::invalid_argument buf_size to small to  hold the data
   */
  std::uint16_t deserialize(const std::array<std::byte, stream_size>& buf,
                            const std::uint16_t buf_size) {
    // Test for correct buf_size
    if (buf_size != stream_size) {
      throw std::invalid_argument(
          "deserialize() buf_size incorrect for  struct Unid_block");
    }

    // Populate the structure data from the buffer
    std::memcpy(&ip, buf.data() + 0, sizeof(uint32_t));
    std::memcpy(&port, buf.data() + 4, sizeof(uint16_t));
    std::memcpy(&sequence, buf.data() + 6, sizeof(uint32_t));
    std::memcpy(&node, buf.data() + 10, sizeof(uint32_t));
    std::memcpy(&unid, buf.data() + 14, sizeof(uint64_t));
    std::memcpy(&nunids, buf.data() + 22, sizeof(uint32_t));

    // Convert structure data from network order to host order if needed
    if constexpr (std::endian::native == std::endian::little) {
      ip = std::byteswap(ip);
      port = std::byteswap(port);
      sequence = std::byteswap(sequence);
      node = std::byteswap(node);
      unid = std::byteswap(unid);
      nunids = std::byteswap(nunids);
    }

    return stream_size;
  }
};

int main() {
  Unid_block unid_block{
      0xc0a80001,  // Ip address - uint32 - 192.168.0.1 - +0
      44444,       // Port - uint16 - hex: 0xad9c - +4
      1234,        // Sequence - uint32 - hex: 0x000004d2 - +6
      4321,        // Node id - uint32 - hex: 0x000010e1 - +10
      9876,        // First Unid - uint64 - hex: 0x0000000000002694 - +14
      6789         // Number of unids - uint32 - hex: 0x00001a85 - +22
  };

  std::array<std::byte, unid_block.get_stream_size()> buf;

  auto nbytes = unid_block.serialize(buf, unid_block.stream_size);

  int i = 0;
  for (auto byte : buf) {
    std::cout << "buf[" << std::dec << i++ << "]: 0x" << std::setfill('0')
              << std::setw(2) << std::hex << std::to_integer<int>(byte)
              << std::endl;
  }

  std::cout << "Serialized data number of bytes: " << std::dec
            << unid_block.get_stream_size() << std::endl;

  return 0;
}
