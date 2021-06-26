#include <asm/memory.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/segment.h>

#define LOW_MEM 0x100000
#define PAGING_MEMORY (15*1024*1024)
#define PAGING_PAGES (PAGING_MEMORY>>12)

extern unsigned char mem_map [ PAGING_PAGES ] ;
struct shm_struct * shms[SHARED_SIZE] = { NULL, };

int sys_shmget(int key, int size) {
    int i;
    struct shm_struct* tmp;
    // printk("sys_shmget(%d)\n", key);
    
    if (size <= 0 || size > PAGE_SIZE) 
        return -1;

    // find the same key
    for (i = 0; i < SHARED_SIZE; ++i) {
        if (shms[i] && shms[i]->key == key) {
            // printk("find the same key [%d] and will return.\n", key);
            return i;
        }
    }

    // find the empty slot
    for (i = 0; i < SHARED_SIZE; ++i) {
        if (!shms[i]); 
            break;
    }

    // no empty slot
    if (i == SHARED_SIZE) {
        printk("the is no empty slot!\n");
        return -1;
    }
    // make the struct
    tmp = (struct shm_struct*) malloc(sizeof(struct shm_struct));

    // init tmp
    tmp->key = key;
    tmp->addr = 0;
    if (!(tmp->addr = get_free_page())) {
        printk("can't not get a free page!\n");
        free(tmp);
        return -1;
    }

    shms[i] = tmp;
    return i;
}

// make the addr to the buf
void * sys_shmat(int shmid) {
    unsigned long addr;
    unsigned long tmp, *page_table;
    // printk("sys_shmat(%d)\n", shmid);
    if (!shms[shmid]) {
        printk("shms[%d] is NULL:( !!\n", shmid);
        return NULL;
    }
    // printk("start code: 0x%lx\n", current->start_code);

    addr = current->brk;
    current->brk += PAGE_SIZE;
    put_page(shms[shmid]->addr, addr + current->start_code);
    mem_map[(shms[shmid]->addr-LOW_MEM)>>12]++;
    
    // printk("physical addr is 0x%x\n", shms[shmid]->addr);
    // printk("linear addr is 0x%x\n", addr + current->start_code);
    // printk("logic addr is 0x%x\n", addr);

    return (void *) addr;
}