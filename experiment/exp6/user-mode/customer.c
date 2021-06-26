#include <stdio.h>
#include <fcntl.h>

#include "buff.h"
#include "sem.h"

int poll();

int main(int argc, char *argv[])
{
    int id;
    int procucts;
    int i;
    sem_t *empty;
    sem_t *product;
    sem_t *mutex;
    char* s;

    if (argc != 3) {
        printf("error params! please input like this: ./cus [id] [products]\n");
        return -1;
    }
    for (i = 0, id = 0, s = argv[1]; s[i]; ++i) {
        id *= 10;
        id += s[i] - '0';
    }
    for (i = 0, procucts = 0, s = argv[2]; s[i]; ++i) {
        procucts *= 10;
        procucts += s[i] - '0';
    }
    printf("customer[%d] start to get [%d] products!\n", id, procucts);

    if (!(product = sem_open(PRODUCT, 0))) {
        printf("error to open sem product\n");
        return -1;
    }
    if (!(empty = sem_open(FREE, BUFFER_SIZE))) {
        printf("error to open sem empty\n");
        return -1;
    }
    if (!(mutex = sem_open(MUTEX, 1))) {
        printf("error to open sem mutex\n");
       return -1;
    }

    for (i = 0; i < procucts; i++) {
        int val;
        sem_wait(product);
        sem_wait(mutex);
        val = poll();
        printf("customer [%d] get a product [%d]!\n", id, val);
        sem_post(mutex);
        sem_post(empty);
    }
    
    return 0;
}
