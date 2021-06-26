#include <unistd.h>

int shmget(key_t key, size_t size, int flag);
void *shmat(int shmid, void *addr, int flag);