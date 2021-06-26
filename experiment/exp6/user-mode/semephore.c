
#define __LIBRARY__
#include "sem.h"

_syscall2(sem_t, opensem, char*, name, int, value);
_syscall1(int, unlinksem, char*, name);
_syscall1(int, waitsem, sem_t, s);
_syscall1(int, postsem, sem_t, s);


sem_t *sem_open(char* name, int value)
{
    char sem_name[20] = { 0 };
    int i = 0;
    sem_t* s = (sem_t*)malloc(sizeof(sem_t));
    while (name[i]) {
        sem_name[i] = name[i];
        i++;
    }
    sem_name[i] = '\0';
    *s = opensem(sem_name, value);
    return s;
}

int sem_wait(sem_t* sem) {
    return waitsem(*sem);
}

int sem_post(sem_t* sem) {
    return postsem(*sem);
}

int sem_unlink(char* name) {
    return unlinksem(name);
}
