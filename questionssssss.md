1. sys_waitpid 中的 **p怎么理解？ 
```
struct task_struct ** p; // ???

....


for(p = &LAST_TASK ; p > &FIRST_TASK ; --p) {
	
	....
```

2. 什么是信号位图? schedlue() 和 sys_waitpid中两句话怎么理解？ 


3. 4号进程是啥? 