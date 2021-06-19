
#define __LIBRARY__
#include<unistd.h>
#include<linux/kernel.h>
#include<fcntl.h>

_syscall2(sem_t, opensem, char*, name, int, value);
_syscall1(int, unlinksem, char*, name);
_syscall1(int, waitsem, sem_t, s);
_syscall1(int, postsem, sem_t, s);


#define BUFFER_SIZE 30
#define CUSTOMER 5
#define PRODUCT 1000
#define SEM_PRODUCT "product"
#define SEM_MUTEX "mutex"
#define SEM_FREE "free"
#define FILE_MODE S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP

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

/*
int main()
{
    sem_t* sem = sem_open("sem_open", 2);
    sem_t* sem2 = sem_open("sem_open:)", 1);

    printf("sem = %d, and sem2 = %d!\n", *sem, *sem2);
    sem_unlink("sem_open:)");

    sem_post(sem);
    sem_wait(sem);
    sem_wait(sem);
    sem_wait(sem);
    sem_wait(sem);
    sem_wait(sem);
    sem_wait(sem);
    sem_unlink("sem_open");
}
*/

int read_4byte(int fd) {
    int res = 0;
    char *buf = (char*) malloc(sizeof(char));
    int i;
    for (i = 0; i < 4; i++) {
        read(fd, buf, 1);
        res <<= 8;
        res += *buf;
    }
    return res;
}

void write_4byte(int fd, int val) {
    char stack[4] = { 0 };
    int stack_idx = 0;
    int i;
    for (i = 0; i < 4; i++) {
        stack[stack_idx++] = (val >> (8 * i)) & 0x0ff;
    }
    for (i = 0; i < 4; i++) {
        char data = stack[--stack_idx];
        write(fd, &data, 1);
    }
}

int poll(int fd) {
    int pos;
    int res;
    lseek(fd, 4, SEEK_SET);
    pos = read_4byte(fd);    
    lseek(fd, pos * 4, SEEK_SET);
    res = read_4byte(fd);
    lseek(fd, 4, SEEK_SET);
    write_4byte(fd, (pos - 1) % BUFFER_SIZE + 2);
    return res;
}

void offer(int fd, int val) {
    int pos;
    lseek(fd, 0, SEEK_SET);
    pos = read_4byte(fd);   
    lseek(fd, pos * 4, SEEK_SET);
    write_4byte(fd, val);
    lseek(fd, 0, SEEK_SET);
    write_4byte(fd, (pos - 1) % BUFFER_SIZE + 2);
}

void customer(int fd, int id) {
    int i;
    sem_t *product = sem_open(SEM_PRODUCT, 0);
    sem_t *empty = sem_open(SEM_FREE, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, 0);
    printf("customer %d!\n", id);

    for (i = 0; i < PRODUCT / CUSTOMER; i++) {
        int val;
        sem_wait(product);
        sem_wait(mutex);
        val = poll(fd);
        printf("%d:\t%d\n", id, val);
        sem_post(mutex);
        sem_post(empty);
    }
}

void producter(int fd) {
    int i;
    sem_t *product = sem_open(SEM_PRODUCT, 0);
    sem_t *empty = sem_open(SEM_FREE, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, 0);

    for (i = 0; i < PRODUCT; i++) {
        sem_wait(empty);
        sem_wait(mutex);
        offer(fd, i);
        printf("make a [%d]\n", i);
        sem_post(mutex);
        sem_post(product);
    }
}

int main()
{
    int fd;
    int i;
    sem_t *product = sem_open(SEM_PRODUCT,  0);
    sem_t *empty = sem_open(SEM_FREE, BUFFER_SIZE);
    sem_t *mutex = sem_open(SEM_MUTEX, 1);
    fd = open("filebuf", O_RDWR | O_CREAT);
    write_4byte(fd, 2);
    write_4byte(fd, 2);

    if (!fork()) {
        producter(fd);
    } else if (!fork()) {
        customer(fd, 0);
    } else if (!fork()) {
        customer(fd, 1);
    } else if (!fork()) {
        customer(fd, 1);
    } else if (!fork()) {
        customer(fd, 1);
    } else if (!fork()) {
        customer(fd, 1);
    } else {
        for (i = 0; i <= CUSTOMER; i++) 
            wait(NULL);

        sem_unlink(SEM_PRODUCT);
        sem_unlink(SEM_FREE);
        sem_unlink(SEM_MUTEX);
    }
}
