// gcc14-g++ ipc_shared_memory_server.cpp -lrt -o bin/ipc_shared_memory_server
#include <iostream>
#include <cstring>
#include <sys/mman.h>
#include <unistd.h>
#include "ipc_shared_memory.hpp"

int main() {
    int shm_fd;
    SharedData* shared_data;
    sem_t* sem;

    // 1. Create and open the shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // 2. Set the size of the shared memory object
    ftruncate(shm_fd, sizeof(SharedData));

    // 3. Map the shared memory object
    shared_data = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 4. Create and initialize the semaphore
    // Initial value 0: client waits for server to write first
    sem = sem_open(SEM_NAME, O_CREAT, 0666, 0); 
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // 5. Write data to shared memory
    shared_data->status = 1; // Data ready
    strncpy(shared_data->message, "Hello from server!", BUFFER_SIZE);
    std::cout << "Server wrote data: " << shared_data->message << std::endl;

    // 6. Signal the client that data is ready
    sem_post(sem); // Increment semaphore, allowing client to proceed

    // Wait a moment for client to read (optional, for demonstration)
    sleep(10);

    // 7. Clean up
    if (munmap(shared_data, sizeof(SharedData)) == -1) {
        perror("munmap");
    }
    close(shm_fd);
    shm_unlink(SHM_NAME); // Mark for deletion
    sem_close(sem);
    sem_unlink(SEM_NAME); // Unlink semaphore

    return 0;
}

