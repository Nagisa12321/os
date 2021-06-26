## 关闭标准输出, 然后输出到文件中
    ```c
    #include<fcntl.h>
    #include<unistd.h>
    #include<stdio.h>

    int main() {
        close(STDOUT_FILENO);
        int fd = open("stdout.txt", O_CREAT | O_RDWR);
        printf("%d\n", fd);
        printf("Hello world!\n");
    }

    ```

## lseek

1. 对于管道/套接字... lseek返回-1, 表示不是random access file!
2. 注意文件偏移量可以大于当前文件长度, 这会在文件中形成一个空洞, 全部用'\0'代替, 但这是允许的. 下次write的时候会增加文件长度. 中间的空洞不用分配磁盘块

- 使用od命令可以查看文件内容!(字符形式)

## read

- 创建的buf的size最好比read的size大1. 

## 用O_APPEND标志可以防止文件错乱~且不用原子操作