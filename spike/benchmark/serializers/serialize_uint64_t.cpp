#include <iostream>
#include <bit>
#include <array>
#include <cstdint>
#include <iomanip>

int main() {
    // 1. Define our initial 64-bit values
    std::uint64_t value1 = 0x0123456789ABCDEF;
    std::cout << "value1: 0x" << std::hex << value1 << "\n";

    std::uint64_t value2 = 0x4567;
    std::cout << "value2: 0x" << std::hex << value2 << "\n";

    // 2. Convert original_value1 to Big-endian for transport over network
    std::uint64_t value1_no = std::byteswap(value1);;
    std::uint64_t value2_no = std::byteswap(value2);

    // 3. Convert uint64_t -> std::array<uint8_t, 8>
    std::array<std::uint8_t, sizeof(std::uint64_t)> bytes = std::bit_cast<std::array<std::uint8_t, sizeof(std::uint64_t)>>(value1_no);

    std::cout << "value1 (network order): 0x" << std::hex << value1_no << "\n";
    std::cout << "Bytes: ";
    for (auto byte : bytes) {
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << "\n";

    // 3. Convert std::array<uint8_t, 8> -> uint64_t
    /*
    uint64_t  restored_value1_no = std::bit_cast<std::uint64_t>(byte_array);
    auto restored_value1 = std::byteswap(restored_value_no);
    std::cout << "Restored: 0x" << std::hex << restored_value1 << "\n";
    */

    return 0;
}
