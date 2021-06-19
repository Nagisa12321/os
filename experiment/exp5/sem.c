#include <linux/kernel.h>
#include <asm/segment.h>  /* linux-0.11/include/asm */
#include <unistd.h>
#include <signal.h>
#include <linux/sched.h>
#include <asm/system.h>

#define MAX_NAMELEN 20

struct semaphore * sems[SEM_SIZE] = { NULL };

void show_sem() {
    int i;
    for (i = 0; i < SEM_SIZE; i++) {
        if (sems[i]) {
            printk("%s:%d ", sems[i]->name, sems[i]->value);
        }
    }
    printk("\n");
}

// return the array idx of semaphore
sem_t sys_opensem(char* name, int value) {
    char sem_name[MAX_NAMELEN] = { 0 };
    int namelen = 0, i = 0;
    while (get_fs_byte(name + namelen) != '\0') {
        namelen++;
    }
    if (namelen > MAX_NAMELEN) return -1;

    while (i < namelen) {
        char ch =  get_fs_byte(name + i);
        sem_name[i++] = ch;
    } 
    sem_name[i] = '\0';
    printk("sys_opensem(%s, %d)!\n",sem_name, value);

    // find an empty slot and create;
    i = 0;
    int tmp;
    int same;
    // check if there is same name?
    while (i < SEM_SIZE) {
        if (!sems[i]) {
            i++;
            continue;
        }
        tmp = 0;
        char *name = sems[i]->name;
        while (name[tmp]) 
            tmp++;

        if (tmp != namelen) {
            i++;
            continue;
        }


        same = 1;
        tmp = 0;
        while (tmp < namelen) {
            if (name[tmp] != sem_name[tmp]) {
                same = 0;
                break;
            }
            tmp++;
        }
        if (same) {
            // printk("exist the sem [%s], will just return.\n", sem_name);
            return i;
        }
        i++;
    }

    // now should create a sem;
    i = 0;
    while (i < SEM_SIZE) {
        if (!sems[i]) {
            // printk("create a sem [%s] :)\n", sem_name);
            struct semaphore *s;
            s = (struct semaphore *) malloc(sizeof(struct semaphore));
            char* n = (char*) malloc(sizeof(char) * namelen);
            int j = 0;
            while (sem_name[j]) {
                n[j] = sem_name[j];
                j++;
            }
            s->name = n;
            s->value = value;

            s->p = (struct task_struct*) malloc(sizeof(struct task_struct));
            sems[i] = s;
            return i;
        }
        i++;
    }
    return NULL;
}

int sys_waitsem(sem_t s) {
    // show_sem();
    cli();
    // printk("sys_waitsem(%d)!\n", s);
    if (!sems[s]) return -1;

    sems[s]->value--;
    // printk("decrease sem [%s]'s value, and it is [%d]\n", 
    //         sems[s]->name, sems[s]->value);
    if (sems[s]->value < 0) {
        sleep_on(&(sems[s]->p));
    } 
    sti();
    return 0;
}

int sys_postsem(sem_t s) {
    cli();
    // printk("sys_postsem(%d)!\n",s);
    if (!sems[s]) return -1;

    sems[s]->value++;
    // printk("increase sem [%s]'s value, and it is [%d]\n", 
    //         sems[s]->name, sems[s]->value);
    if (sems[s]->value <= 0) {
        wake_up(&(sems[s]->p));
    }
    sti();
    return 0;
}

int sys_unlinksem(char* name) {
    char sem_name[MAX_NAMELEN] = { 0 };
    int namelen = 0, i = 0;
    while (get_fs_byte(name + namelen) != '\0') {
        namelen++;
    }
    if (namelen > MAX_NAMELEN) return -1;

    while (i < namelen) {
        char ch =  get_fs_byte(name + i);
        sem_name[i++] = ch;
    } 
    sem_name[i] = '\0';
    printk("sys_unlinksem(%s)!\n", sem_name);

    i = 0;
    int tmp;
    int same;
    while (i < SEM_SIZE) {
        if (!sems[i]) {
            i++;
            continue;
        }
        tmp = 0;
        char *name = sems[i]->name;
        while (name[tmp]) 
            tmp++;

        if (tmp != namelen) {
            i++;
            continue;
        }

        same = 1;
        tmp = 0;
        while (tmp < namelen) {
            if (name[tmp] != sem_name[tmp]) {
                same = 0;
                break;
            }
            tmp++;
        }
        if (same) {
            printk("exist the sem [%s], now delete it.\n", sem_name);
            sems[i] = NULL;
            return 0;
        }
        i++;
    }
    printk("not find the same [%s] :(\n", sem_name);
    return -1;
}