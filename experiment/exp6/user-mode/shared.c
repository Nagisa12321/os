#define __LIBRARY__
#include <unistd.h>
#include <linux/kernel.h>

_syscall2(int, getshm, int, key, int, size)
_syscall1(void *, atshm, int, shmid)

int shmget(key_t key, size_t size, int flag) {
    int i;

    if ((i = getshm(key, size)) == -1) {
        printf("system call error!\n");
        return -1;
    }
    return i;
}

void *shmat(int shmid, void *addr, int flag) {
    void* ptr;
    
    /* ptr = (void *) malloc(sizeof(void)); */
    if (!(ptr = atshm(shmid))) {
        printf("system call error!\n");
        return NULL;
    }
    /* printf("the ptr to shm is %p\n", ptr);*/
    
    return ptr;
}
