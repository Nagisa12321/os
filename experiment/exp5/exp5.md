## 实验要求
1. 用信号量解决生产者—消费者问题
- 在 Ubuntu 上编写应用程序“pc.c”，解决经典的生产者—消费者问题，完成下面的功能：
  1. 建立一个生产者进程，N 个消费者进程（N>1）；
  2. 用文件建立一个共享缓冲区；
  3. 生产者进程依次向缓冲区写入整数 0,1,2,...,M，M>=500；
  4. 消费者进程从缓冲区读数，每次读一个，并将读出的数字从缓冲区删除，然后将本进程 ID 和 + 数字输出到标准输出；
  5. 缓冲区同时最多只能保存 10 个数。
- 实现信号量
    - 实现一套山寨版的完全符合 POSIX 规范的信号量
```
sem_t *sem_open(const char *name, unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_unlink(const char *name);
```
1. sem_open() 的功能是创建一个信号量，或打开一个已经存在的信号量。
    - sem_t 是信号量类型，根据实现的需要自定义。
        - name 是信号量的名字。不同的进程可以通过提供同样的 name 而共享同一个信号量。如果该信号量不存在，就创建新的名为 name 的信号量；如果存在，就打开已经存在的名为 name 的信号量。
        - value 是信号量的初值，仅当新建信号量时，此参数才有效，其余情况下它被忽略。当成功时，返回值是该信号量的唯一标识（比如，在内核的地址、ID 等），由另两个系统调用使用。如失败，返回值是 NULL。
    - sem_wait() 就是信号量的 P 原子操作。如果继续运行的条件不满足，则令调用进程等待在信号量 sem 上。返回 0 表示成功，返回 -1 表示失败。
    -  sem_post() 就是信号量的 V 原子操作。如果有等待 sem 的进程，它会唤醒其中的一个。返回 0 表示成功，返回 -1 表示失败。
    - sem_unlink() 的功能是删除名为 name 的信号量。返回 0 表示成功，返回 -1 表示失败。

在 kernel 目录下新建 sem.c 文件实现如上功能。然后将 pc.c 从 Ubuntu 移植到 0.11 下，测试自己实现的信号量。

## 踩坑记录

1. sem_open()的用法, 和sem_init()的初始化
2. pc.c文件我本来写了个队列, 但是发现进程之间不能共享堆空间!!!准备改写共享内存/文件
3. open的信号量如果没有destory,是跟内核共存的....., 因此如果程序写的不好, 忘记destory了, 会导致奇怪的结果4
4. 传进去read的buf要先申请内存~~~
5. 在栈区的char*不能直接分配给内存中的struct


## POXI信号量
- https://blog.csdn.net/anonymalias/article/details/9219945

## 多进程共享文件
- off_t lseek(int fd, off_t offset, int whence)
    1. whence = SEEK_SET, 则将该文件的偏移量设置为据文件开始处offset个字节
    2. whence = SEEK_CUR, 则将该文件的偏移量设置为当前值加offset. 
    3. whence = SEEK_END, 则将该文件的偏移量设置为文件长度加offset. 

- 