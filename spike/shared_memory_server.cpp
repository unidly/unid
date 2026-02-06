/**
 * @file
 * @brief An example program that demonstrates setting up shared memory
 *
 * @copyright Copyright 2026, Fhir First LLC - All rights reserved
 */
/* Compile with shared_memory_server.cpp -lrt -o shared_memory */

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>  // For perror
#include <cstring>
#include <iostream>

int main() {
  const char* name = "/my_shared_memory";
  const char* message = "Hello, Shared Memory!";
  const size_t SIZE = strlen(message) + 1;  // Size needed for the string

  // 1. Create and open the shared memory object
  int shm_fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (shm_fd == -1) {
    perror("shm_open");
    return 1;
  }

  // 2. Set the size of the shared memory object
  if (ftruncate(shm_fd, SIZE) == -1) {
    perror("ftruncate");
    return 1;
  }

  // 3. Map the shared memory into the address space
  void* ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  // 4. Write to the shared memory
  memcpy(ptr, message, SIZE);
  std::cout << "Wrote to shared memory: \"" << (char*)ptr << "\"" << std::endl;

  // 5. Unmap the shared memory (optional for this single process, but good
  // practice)
  if (munmap(ptr, SIZE) == -1) {
    perror("munmap");
    return 1;
  }

  // 6. Close the file descriptor
  if (close(shm_fd) == -1) {
    perror("close");
    return 1;
  }

  // 7. Unlink the shared memory object (removes it from the system)
  if (shm_unlink(name) == -1) {
    perror("shm_unlink");
    return 1;
  }

  return 0;
}

