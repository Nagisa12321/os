
#include<unistd.h>
#include<linux/kernel.h>
#include<fcntl.h>

sem_t *sem_open(char* name, int value);
int sem_wait(sem_t* sem);
int sem_post(sem_t* sem) ;
int sem_unlink(char* name);