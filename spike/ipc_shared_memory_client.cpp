// gcc14-g++ ipc_shared_memory_client.cpp -lrt -o bin/ipc_shared_memory_client
#include <iostream>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "ipc_shared_memory.hpp"

int main() {
    int shm_fd;
    SharedData* shared_data;
    sem_t* sem;

    // 1. Open the shared memory object
    shm_fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. Map the shared memory object (read-only)
    shared_data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 3. Open the semaphore
    sem = sem_open(SEM_NAME, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // 4. Wait for the server to signal that data is ready
    std::cout << "Client waiting for data..." << std::endl;
    sem_wait(sem); // Decrement semaphore, blocks if value is 0

    // 5. Read data from shared memory
    std::cout << "Client read status: " << shared_data->status << std::endl;
    std::cout << "Client read data: " << shared_data->message << std::endl;

    // 6. Clean up
    if (munmap(shared_data, sizeof(SharedData)) == -1) {
        perror("munmap");
    }
    close(shm_fd);
    sem_close(sem);

    return 0;
}

