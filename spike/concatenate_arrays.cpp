#include <iostream>
#include <array>
#include <cstddef> // For std::byte
#include <algorithm> // For std::copy
#include <numeric> // For std::accumulate (or simply use fold expression for sum)

// Helper to sum the sizes of the input arrays at compile time using a fold expression
template<std::size_t... Ns>
constexpr std::size_t sum_sizes(std::index_sequence<Ns...>) {
    return (Ns + ...);
}

// Variadic template function to concatenate std::array<std::byte>
template<std::size_t... Ns>
constexpr auto concatenate_byte_arrays(const std::array<std::byte, Ns>&... arrs)
    -> std::array<std::byte, (Ns + ...)> // Return type uses fold expression for total size
{
    constexpr std::size_t total_size = (Ns + ...);
    std::array<std::byte, total_size> result;
    std::size_t offset = 0;

    // Fold expression over the comma operator to perform the copy for each array
    ((std::copy(arrs.begin(), arrs.end(), result.begin() + offset), offset += Ns), ...);

    return result;
}

int main() {
    constexpr std::array<std::byte, 3> a1 = {std::byte{0x01}, std::byte{0x02}, std::byte{0x03}};
    constexpr std::array<std::byte, 2> a2 = {std::byte{0x04}, std::byte{0x05}};
    constexpr std::array<std::byte, 4> a3 = {std::byte{0x06}, std::byte{0x07}, std::byte{0x08}, std::byte{0x09}};

    // Concatenate the arrays
    constexpr auto combined = concatenate_byte_arrays(a1, a2, a3);

    // Print the result
    std::cout << "Combined array size: " << combined.size() << std::endl;
    std::cout << "Combined array data: ";
    for (const auto& byte : combined) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    return 0;
}

