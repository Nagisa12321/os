#include <sys/stat.h>

#define BUFFER_SIZE 4
#define FREE        "/free_c"
#define MUTEX       "/mutex_c"
#define PRODUCT     "/product_c"
#define FILE_MODE   S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP
#define KEY         1234
#define FLAG        0666

struct buf {
    int head;
    int tail;
    int buffer[BUFFER_SIZE];
};

int poll();
void offer(int val);