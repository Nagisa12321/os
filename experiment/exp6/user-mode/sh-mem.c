#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "buff.h"
struct buf* shmbuf;
int poll() {
    int shmid;
    int res;
    if (!shmbuf) {
        if((shmid = shmget(KEY, sizeof(struct buf), FLAG)) == -1) {
            printf("shmget error!\n");
            return -1;
        }
        if(!(shmbuf = shmat(shmid, 0, 0))) {
            printf("shmget error!\n");
            return -1;
        }
    }
    res = shmbuf->buffer[shmbuf->tail];
    shmbuf->tail = (shmbuf->tail + 1) % BUFFER_SIZE;
    return res;
}

void offer(int val) {
    int shmid;
    if (!shmbuf) {
        if((shmid = shmget(KEY, sizeof(struct buf), FLAG)) == -1) {
            printf("shmget error!\n");
            return;
        }
        if(!(shmbuf = shmat(shmid, 0, 0))) {
            printf("shmget error!\n");
            return;
        }
    }
    shmbuf->buffer[shmbuf->head] = val;
    shmbuf->head = (shmbuf->head + 1) % BUFFER_SIZE;
}