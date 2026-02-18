#include <iostream>
#include <vector>
#include <cstdint>

// Generates the CRC-32 lookup table for the standard polynomial 0xEDB88320
void generate_crc32_table(uint32_t table[256]) {
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (size_t j = 0; j < 8; j++) {
            if (c & 1) c = polynomial ^ (c >> 1);
            else c >>= 1;
        }
        table[i] = c;
    }
}

// Computes the CRC-32 checksum of a given buffer
uint32_t compute_crc32(const void* buf, size_t len, uint32_t initial = 0) {
    static uint32_t table[256];
    static bool table_generated = false;
    if (!table_generated) {
        generate_crc32_table(table);
        table_generated = true;
    }

    uint32_t crc = initial ^ 0xFFFFFFFF;
    const uint8_t* u = static_cast<const uint8_t*>(buf);
    for (size_t i = 0; i < len; ++i) {
        crc = table[(crc ^ u[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFF;
}

int main() {
    std::string data = "The quick brown fox jumps over the lazy dog";
    uint32_t crc = compute_crc32(data.data(), data.size());
    std::cout << "CRC-32: " << std::hex << crc << std::endl; // Expected: 414fa339
    return 0;
}

