// ipc_shared_memory.hpp
#include <fcntl.h>   // For O_* constants
#include <sys/stat.h> // For mode constants
#include <semaphore.h> // For semaphore operations

const char* SHM_NAME = "/my_shm";
const char* SEM_NAME = "/my_sem";
const int BUFFER_SIZE = 256;

struct SharedData {
    int status; // 0: empty, 1: data ready, 2: processed
    char message[BUFFER_SIZE];
};

