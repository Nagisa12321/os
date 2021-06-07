1. sys_waitpid 中的 **p怎么理解？ 
```
struct task_struct ** p; // ???

....


for(p = &LAST_TASK ; p > &FIRST_TASK ; --p) {
	
	....
```

2. 什么是信号位图? schedlue() 和 sys_waitpid中两句话怎么理解？ 

3. wait是当有一个子进程结束就直接结束吗? 

4. 执行./hallo为什么有两个进程create? 

5. switch_to的ecx是什么？ 