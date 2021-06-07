# exp4

## 回答下面三个题：

- 问题1 : 针对下面的代码片段：
    ```
    movl tss,%ecx
    addl $4096,%ebx
    movl %ebx,ESP0(%ecx)
    copy
    ```
    - 回答问题：

      1. 为什么要加 4096；
      2. 为什么没有设置 tss 中的 ss0。
- 问题 2 : 针对代码片段：
    ```
    *(--krnstack) = ebp;
    *(--krnstack) = ecx;
    *(--krnstack) = ebx;
    *(--krnstack) = 0;
    ```
    - 回答问题：

      1. 子进程第一次执行时，eax=？为什么要等于这个数？哪里的工作让 eax 等于这样一个数？
      2. 这段代码中的 ebx 和 ecx 来自哪里，是什么含义，为什么要通过这些代码将其写到子进程的内核栈中？
      3. 这段代码中的 ebp 来自哪里，是什么含义，为什么要做这样的设置？可以不设置吗？为什么？
- 问题 3
- 为什么要在切换完 LDT 之后要重新设置 fs=0x17？而且为什么重设操作要出现在切换完 LDT 之后，出现在 LDT 之前又会怎么样？

### link
- [寄存器逻辑结构图](https://www.zhihu.com/question/291255701/answer/474405270)
- [lldt指令](http://blog.sina.com.cn/s/blog_5a75aaa501013ktq.html)


### 实验目标

- 本次实践项目就是将 Linux 0.11 中采用的 TSS 切换部分去掉，取而代之的是基于堆栈的切换程序。具体的说，就是将 Linux 0.11 中的 switch_to 实现去掉，写成一段基于堆栈切换的代码。

本次实验包括如下内容：

1. 编写汇编程序 switch_to：
2. 完成主体框架；
   - 在主体框架下依次完成 PCB 切换、内核栈切换、LDT 切换等；
   - 修改 fork()，由于是基于内核栈的切换，所以进程需要创建出能完成内核栈切换的样子。
   - 修改 PCB，即 task_struct 结构，增加相应的内容域，同时处理由于修改了 task_struct 所造成的影响。
   - 用修改后的 Linux 0.11 仍然可以启动、可以正常使用。
    （选做）分析实验 3 的日志体会修改前后系统运行的差别。


### TSS 切换
- 所谓的 TSS 切换就将 CPU 中几乎所有的寄存器都复制到 TR 指向的那个 TSS 结构体中保存起来，同时找到一个目标 TSS，即要切换到的下一个进程对应的 TSS，将其中存放的寄存器映像“扣在” CPU 上，就完成了执行现场的切换.

### switch_to要做的事
```
    pushl %ebp
    movl %esp, %ebp # 栈基址
    pushl %ecx      # push当前参数
    pushl %ebx
    pushl %eax
    movl 8(%ebp), %ebx # 把下一个进程地址放在ebx中
    cmpl %ebx, current # 如果当前进程和next相同, 则什么也不做. 
    je 1f
```

1. 切换PCB
    ```
    mov %ebx, %eax
    xchgl %eax, current
    ```

2. TSS中内核栈指针的重写
   - tss: 0号进程的tss地址.
   - %ebx: next进程的地址
   - ESP0: tss结构中esp0的偏移量
    ```
    movl tss, %ecx          # 将0号进程的tss地址放到ecx中
    addl $4096, %ebx        # 下一个进程的地址向上增长4K(一页), 刚好就是栈顶
    movl %ebx, ESP0(%ecx)   # 把栈顶地址放在tss结构中
    ```
3. 切换内核栈
   - 现在的esp是当前栈顶, 我们要拿到下一个esp, 对当前esp进行修改, 栈就切过去了!
   - 切换esp之后我们要拿到当前栈基址ebp
   - 我们之前已经push ebp ---- 在自己的栈中放好自己的ebp, 然后切换. 
   - 因此我们待会pop出来的ebp, 就是当前栈基址!!!
   ```
    mov %ebx, %esp
   ```
4. 切换LDT
    ```
    mov 12(%ebp), %eax
    lldt %ax

    movl $0x17, %ecx
    mov %cx, %fs
    ```
5. 切换pc指针. 
    - 执行switch中的ret会跳到schedule() 的 "}", 然后再返回会跳到中断处理中(ret_from_sys_call);
    - 因此会有iret, pop各种当前内核栈中存的当前用户态的寄存器, 然后再跳到用户态
    

### fork.c要做的事
- 把进程变成"能切"的样子
1. 要"抄袭"父进程的用户态: INT 0x80: ss, esp, eflag, cs, eip
2. 思考一下父进程怎么到达copy_process() 的? 
    - sys_call
    - sys_fork
### sched.h 和 sched.c，