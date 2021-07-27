## 本次实验的基本内容是：

1. 用 Bochs 调试工具跟踪 Linux 0.11 的地址翻译（地址映射）过程，了解 IA-32 和 Linux 0.11 的内存管理机制；
2. 在 Ubuntu 上编写多进程的生产者—消费者程序，用共享内存做缓冲区；
3. 在信号量实验的基础上，为 Linux 0.11 增加共享内存功能，并将生产者—消费者程序移植到 Linux 0.11。


## 对于地址映射实验部分，列出你认为最重要的那几步（不超过 4 步），并给出你获得的实验数据。


## test.c 退出后，如果马上再运行一次，并再进行地址跟踪，你发现有哪些异同？为什么？


## share mem

#### key_t key = ftok("/home/jt/project/c/os", 1);
- 由项目路径 + id产生键