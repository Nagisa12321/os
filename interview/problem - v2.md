fork()
exec()
1.进程 线程 协程的区别
2.32位和64位操作系统的区别（不知道）
3.上下文？？？（没听说过）
4.缓存置换算法
操作系统  僵尸进程
------------------------------------------
## 进程通信的几种方式
1. 共享存储区: 
	- 进程A想和进程B通信, 首先进程A无法访问进程B的地址空间, 因为这样不安全. 
	- 操作系统会为这两个进程分配共享存储区
	- A可以把数据写入该区域, B可以从该区域读数据, 对该区域的读写是互斥的(PV操作)。 
	- 共享存储区有两种类型一种是基于数据结构的共享存储区, 另一种是基于应用的共享存储区, 后者较为高级
2. 管道通信
	- 进程A想对进程B通信, 就好像在A, B之间开设一条半双工的管道。(就是一个页表的大小 4kb)
	- 对于这条管道的读写也是互斥的. 而且管道之间的通信在任意时刻只能是单向的. 
	- 如果要实现双向通信, 那么要开两条管道. 
	- 以字符流的形式写入管道, 当管道满的时候write()的系统调用会被阻塞, 当管道空的时候, read() 的系统调用会被阻塞. 
	- 对于这条管道有两点特性, 写要写满, 读要读完
	- 读取进程最多只能有一个, 不然读不全数据
3. 信箱通信
	- 进程A想对进程B通信, 就把信息放在B的消息队列中. 
	- 消息还会添加首部, 因此这种机制有点像计网中的信息传递. 
	- 对于消息的发送的读取是原语操作。 如果不是原语操作就有可能A发一半的时候切换进程B, B就会开始读取, 而且读到不完整信息. 
	- 另外还有一种机制是A直接把信息放在操作系统管理的信箱中, 之后在适合的时候在放到B的消息队列中. 

## 说说核心态和用户态
- CPU处于用户态只能执行非特权指令, CPU处于核心态, 能执行特权指令和非特权指令. 
- 在内核态运行的程序叫做内核程序, 在用户态运行的程序叫应用程序, 
- 用户态转为内核态, 需要中断, 内核态转为用户态只需修改寄存器中的程序状态字即可(0 为核心态, 3 为用户态)
- 内核态运行时钟管理, 原语操作, 进程管理等等. 
- 不同操作系统的内核态大小有所不同, 分为微内核和大内核. 

## 线程和进程的区别是什么, 线程的实现方式? 
- 定义方面: 线程是程序运行的最小单位, 进程是资源分配的最小单位
- 资源方面: 线程一般不会用到系统资源, 因为都是用进程的资源
- 切换开销方面: 进程之间的切换开销比较大, 需要转到内核态, 线程之间的切换开销比较小. 
- 运行方面: 进程A的结束不会影响到进程B, 反过来一个进程里面的多个线程, 当一个线程异常终止可能会导致进程异常终止

- 实现方式: 内核级线程/用户级线程. 
## 说说用户级线程和内核级线程? 多线程模型, 优点? 
- 用户级线程就是只能用户看到的线程, 操作系统看不到, 内核级线程就是操作系统看到的线程
- 操作系统只会对内核级线程分配内核. 

- 多线程模型
1. 多对一: 多个用户级线程对应一个内核级线程. 在操作系统看来这就是一个进程
	优点是: 进程切换块, 只需要在应用层面就可以切换. 
	缺点是: 因为它们只用到一个核, 所以当一个线程阻塞的时候, 其他的线程也会阻塞. 并发度不是很高. 
2. 一对一： 一个用户级线程对应一个内核级线程. 
	优点是: 并发度很高, 可以同时在多个核并行执行, 即使一个线程阻塞了, 其他线程还能执行. 
	缺点是: 因为是内核级线程, 切换线程要调用内核代码, 开销较大. 
3. 多对多: n个用户级线程对应m个内核级线程. 
	- 进程切换块, 只需要在应用层面就可以切换.  并发度很高, 可以同时在多个核并行执行, 即使一个线程阻塞了, 其他线程还能执行. 

## 说说三层调度。 
1. 高级调度: 就是进程创建 -> 进入内存 -> 进程销毁的过程: 一般都是从外存加载到内存中, 并且创建进程PCB, 关系到内存的分配和回收
2. 中级调度: 当内存紧张的时候, 会将一部分进程从内存移到外存中, 外存指的是文件系统中的..区该区域IO较快, 然而
	进程的PCB会被放在内存中, 一般会放在就绪挂起队列或者阻塞挂起队列之后. 当内存有空间的时候再将外村中的进程放入内存中
3. 初级调度: 就是就绪和运行态的转换. 

## 讲讲进程调度方式? 什么时候不能进程调度？ 
- 抢占式和非抢占式. 

1. 响应中断过程中. 
2. 执行原语期间
3. 在内核临界区时

## 说说几种调度算法
1. 先来先服务：维护一个队列, 加入内存的进程先来先执行
	- 优点: 公平
	- 缺点: 执行时间较短的进程要等很久. 
2. 短时间优先: 维护一个队列, 按进程的服务时间排序, 如果是抢占式的, 进程加入的时候, 如果当前进程剩余时间>加入进程的时间,
	那么会抢占当前进程
	- 优点: 执行时间短的进程友好, 不用等待很久
	- 缺点: 如果一直加入执行时间短的进程, 那么执行时间长的进程得不到执行, 会导致饥饿,
3. 高响应比优先：和短时间优先差不多, 只是判定标准变成响应比, 响应比就是(服务时间 + 等待时间) / 服务时间. 
	因此等待的时间越长响应比越高、
	- 优点：不会导致饥饿, 等待时间短的进程也可以优先得到运行. 
	- 缺点: 耗费系统性能比较高.
	
4. 时间片轮转： 跟先来先服务差不多道理, 只不过达到相应时间片如果没运行完, 会被加下来的进程抢占. 
	- 优点: 公平, 用户等待的时间比较短. 
	- 缺点: 会花大量cpu资源用于进程调度. 
5. 线程优先级:  为每个进程添加进程优先级属性. 操作系统按照进程的优先级来先后运行, 也份抢式和非抢占式
	- 优点: 区分任务的紧急性. 
	- 缺点: 还是会导致饥饿
6. 多级反馈队列: 维护多个队列, 这些队列从上到下优先级降低, 时间片变长
	1. 进程进入的时候先进入第一级队列. 如果时间片用完的话还没执行完, 放入下一级队列
	2. 如果执行一半被优先级更高的进程抢占, 那么放到本级队列末尾
	3. 只有k级没有进程才会从k + 1级开始执行

## 进程互斥的实现方法(软件/硬件)?
- 软件: 
1. 单标志法: 
```
int now = 1; // 当前执行进程

// 进程1 
p1 {
	while (now == 2) {}
	// do something...
	
	now = 2; 
}

// 进程2 
p2 { 
	while (now == 1) {}
	// do something...
	
	now = 1;
}
```
- 缺点: 
	1. 会循环等待. 
	2. 不满足空闲则进原则. 

2. 双标志
(1) 双标志先判断
```
// 假设现在有两个进程. 
boolean[] visit = new boolean[2]; // 表示进程想进入临界区的意愿
visit[0] = false;
visit[1] = false;

// 进程0 
p1 {
	while (visit[1]) {}
	visit[0] = true;
	// do something....
	visit[0] = false;
}

p2 {
	while (visit[0]) {}
	visit[1] = true;
	// do something....
	visit[1] = false;
}
```
- 缺点: 会同时进入临界区
(2) 双标志后判断: 
```
// 假设现在有两个进程. 
boolean[] visit = new boolean[2]; // 表示进程想进入临界区的意愿
visit[0] = false;
visit[1] = false;

// 进程0 
p1 {
	visit[0] = true; // 先表名自己的意愿 
	while (visit[1]) {}
	// do something....
	visit[0] = false;
}

p2 {
	visit[1] = true;
	while (visit[0]) {}
	// do something....
	visit[1] = false;
}
```
- 缺点: 会导致死锁. 
3. Peterson算法
```
// 假设现在有两个进程. 
bool visit[2]; // 表示进程想进入临界区的意愿
visit[0] = false;
visit[1] = false;
int last = 0;

// 进程0 
p1 {
	visit[0] = true; // 先表明自己想进入临界区的意愿
	last = 1; // 将机会让给别人. 
	while (visit[1] && last == 1) {}
	// do something....
	visit[0] = false;
}

p2 {
	visit[1] = true;
	last = 0;
	while (visit[0] && last == 0) {}
	// do something....
	visit[1] = false;
}
```
- 缺点: 会导致cpu空转

- 硬件
1. 开关中断实现
```
关中断;
// do something... 
开中断;
```

2. test and set lock
```
// 硬件实现的逻辑
bool TSL(bool lock) {
	bool tmp = lock;
	lock  = true;
	return tmp;
}

bool lock = false;

// 进程
p { 
	while (TSL(lock)) {}
	// do something...
	lock = false; 
}
```

3. swap 
```
swap 意在原子操作交换两个值 (XCHG)

bool lock; 

// 进程
p { 
	bool tmp = true;
	while (swap(lock, tmp)) {}
	// dosomething...
	lock = false;
}
```

## 原语具体怎么实现的? 
- 通过关中断和开中断实现
- 或者通过软件? 

## 信号量怎么实现的? 有什么类型?
## 说说两种类型的PV操作
1. 整数类型
```
void wait(int mutex) {
	mutex--;
	while (mutex < 0) {}
}

void signal(int mutex) {
	mutex++;
}

int mutex = 1; // 表示资源数为1
// 进程1
p1 {
	wait(mutex);
	// do something 
	signal(mutex);
}
```
2. 结构类型
```
struct {
	int val; // 资源数量
	struct process *queue; // 等待队列
} semaphore

void wait(semaphore mutex) {
	mutex -> val--;
	
	while (mutex -> val < 0) {
		// 阻塞原语
		block(mutex -> queue);
	}
}

void signal(semaphore mutex) {
	mutex++;
	
	while (mutex <= 0) {
		// 唤醒原语
		// wakeup(mutex -> queue);
		wakeupall(mutex -> queue);
	}
}

//

p1 {
	wait(mutex);
	// do something 
	signal(mutex);
}
```
## PV操作是怎么实现互斥, 同步, 前驱的？ 
1. 互斥
```
semaphore mutex;
mutex.val = 1;
// 进程1 
p1 {
	wait(mutex);
	// do something 
	signal(mutex);
}
// 进程2
p2 {
	wait(mutex);
	// do something 
	signal(mutex);
}
```
2. 同步
```
semaphore mutex;
mutex.val = 0;
// 规定语句4必须在语句2 之后执行
p1 {
	语句1;
	语句2;
	signal(mutex);
	语句3;
}

p1 {
	wait(mutex);
	语句4;
	语句5;
	语句6;
}

```
3. 前驱: 和同步差不多, 把PV操作理解为请求资源和释放资源即可. 

## 说说生产者和消费者问题(多消费者生产者呢? )
- 生产者生产产品, 消费者消费产品. 

```
semaphore product = 0;
semaphore free = n;    // 缓冲区可以放n个产品
semaphore mutex = 1;

prodecer {
	while (true) {
		// 生产产品
		// ...
		wait(free);
		// 放入缓冲区
		wait(mutex);
		// ... 
		signal(mutex);
		signal(prodect);
	}
	
}

customer {
	while (true) {
		wait(prodect);
		// 从缓冲区拿出产品
		wait(mutex);
		// ... 
		signal(mutex);
		signal(free);
		// 使用产品
		// ...
	}
}
```
## 说说吸烟者问题
// ... 
## 说说读者写着问题
1. 允许多个读者进行读操作
2. 只允许一个写者进行写操作
3. 任意写着在完成写之前不允许其他读者/写着
4. 写者想进行写要等到所有读者,写者退出.
```

semaphore rw = 1; // 读写锁
semaphore mutex = 1; // 保证对count的互斥访问
semaphore fair = 1; // 保证读者写者的公平 ==> 对wait(rw)的访问不会抢占
int count = 0; // 读者数量

writer {
	wait(fair);
	wait(rw);
	signal(fair);
	// 写文件
	// ....
	signal(rw);
} 

reader {
	wait(fair);
	wait(mutex);
	// 没有读者
	if (count == 0) {
		// 请求读写锁
		wait(rw);
	}
	count++;
	signal(mutex);
	signal(fair);
	// 读
	// ...
	
	wait(mutex);
	// 释放读写锁
	count--;
	if (count == 1) {
		// 请求读写锁
		signal(rw);
	}
	signal(mutex);
}

```
## 说说管程
- 管程是把临界区抽象化的一种对象或者数据结构. 
- 访问管程内的数据只能通过调用管程的函数来进行. 

## 死锁产生的条件? 
- 互斥
- 不可剥夺
- 请求和保持
- 循环等待条件

- (用哲学家进餐问题举例)

## 说说死锁和死循环、饥饿的区别
// ... 

## 怎么避免死锁? 
- 破坏四个条件之一即可避免死锁
1. 破坏互斥条件: 比如对一些外部资源的使用是互斥的, 比如打印机。 我们可以利用有关技术(SPOOling)
	将打印机的访问变成不是互斥的, 用一个进程来分配和调剂打印机的访问(比如设置队列), 
	其他进程看来好像同时拥有打印机(逻辑上共享)
2. 破坏不可剥夺性：进程请求不到想要的资源应该释放其他资源. 或者利用进程的优先级
	这种方法造成严重资源浪费 
3. 破坏请求和保持条件: 静态分配方法, 进程运行前分配所有资源, 会造成资源浪费.
4. 破坏循环等待条件: 给资源编号, 并且按照序号申请资源.

- 通过安全序列避免死锁. 
	- 安全序列指的是系统资源分配顺序
	- 当一个进程想申请资源的时候, 检查一下如果分给这个进程相应资源, 能否找到一个安全序列
	- 如果能的话, 那么便给进程分配资源, 如果不能, 则让线程阻塞. 

## 说说银行家算法? 

## 说说死锁检测算法? 

------------------------------------------------------------

## 什么是内存? 有什么作用? 
- 内存是区别于磁盘的一种存储单位
- 它的存取速度很快
- 因为CPU必须要取指执行, 这里的指就是内存中的地址. 
## 装入的几种方式? 链接的三种方式? 
- 绝对装入, 静态重定位(装入到内存固定位置), 运行时装入(重定位寄存器决定)
- 链接: 
	1. 静态链接: 编译期间把各个模块连接成一个大的模块, 之后就不在拆开. 
	2. 装入时动态链接: 装入的时候才动态的链接
	3. 运行时动态链接: 在需要某个模块的时候才把该模块放到内存中. 

## 操作系统怎么管理内存? 
1. 内存的分配和回收
	- 分配: 
		1. 连续分配
			1. 单一分区分配
			2. 固定分区分配
			3. 动态分区分配
				1. 首次适应
				2. 最佳适应
				3. 最坏适应
				4. 邻近适应
		2. 非连续分配
			1. 基本分页
			2. 基本分段
			3. 段也式
			- 快表
			- 两级页表
	- 回收: 
		- 只要回收完了有空闲分区连在一起, 就要把他们合并
2. 内存空间的扩展
	1. 覆盖技术
	2. 交换技术
	3. 虚拟内存技术
		- 请求分页
			- 页面置换
				- OPT
				- FIFO
				- LRU
				- NRU
			- 请求调页(缺页中断)
3. 地址转换
	- 逻辑地址转化为物理地址. 
4. 存储保护
	1. 上下界寄存器
	2. 重定位寄存器

## 怎么实现内存的扩充? 
1. 覆盖技术
- 把内存分为覆盖区和常驻区
```
main() -> a, b
a -> c
b -> d

常驻区    	main()
覆盖区0		a, b
覆盖区1		c, d
```
2. 交换技术
- 就是实现内存和外存的转换
- 当内存空间不足的时候, 操作系统会把优先级较低, 或者阻塞的进程放到外存中, 外存就是磁盘的对换区
- 而且进程的PCB会存留在内存中, 会被放到阻塞队列或者就绪队列之中
- 当内存有空间的时候, 这些进程会被换入。 
3. 虚拟内存技术
// ... 

## 怎么实现内存的分配? ⭐
- 连续分配: 
	1. 单一分配: 在开始有计算机还没有多道程序的时候, 比如DOS系统, 每次只为一道程序分配内存, 因此可以让程序员自由分配
		也没有所谓的内部碎片 外部碎片. 
	2. 固定分区分配: 有两种, 一种是真的每个分区都一样大小, 另外是逐级递增分区, 但它们都是一样, 进程只能占有一个分区, 
		因此会产生内部碎片, 不会产生外部碎片
	3. 动态分区分配: 进程加入内存的时候才动态的选择要分给他哪个分区, 会产生外部碎片. 要维护空闲分区表/空闲分区链
- 非连续分配: 
	- // ... 

## 什么是内部/外部碎片? 
- 外部碎片指的是: 进行内存分配的时候有一些小的内存空间无法进行分配, 这些就是外部碎片
- 内部碎片指的是: 进行内存分配的时候, 一个进程不得不沾满一个分区, 即使这个进程的内存不用这么大, 
	那么剩下来的这些就是内部碎片. 

## 动态分区的分配算法是什么? （4种）
- 首次适应算法
	- 维护一个空闲分区表, 按照内存从高到低的顺序进行分配
	- 优点: 不用排序, 效率比较高, 对大内存进程友好
	- 缺点: 会产生一些外部碎片
- 最佳适应算法
	- 维护一个空闲分区表, 每次分区都要按照空闲分区的大小进行排序. 优先选择分区最小的进行分配
	- 优点: 对大内存的进程友好. 
	- 缺点: 需要找出最小空闲内存, 耗费性能, 也会造成外部碎片
- 最差适应算法、
	- 维护一个空闲分区表, 每次分区都要按照空闲分区的大小进行排序. 优先选择分区最大的进行分配
	- 内存分配均匀, 产生较少的外部碎片
	- 大内存进程进入的时候, 可能分配不到内存, 要采用紧凑技术. 耗费时间, 还要进行排序. 
- 邻近适应算法. 
	- 维护一个空闲分区表, 每次分配都找当前分配到的下一个分区, 环形绕表走. 
	- 内存分配均匀, 产生较少的外部碎片
	- 大内存进程进入的时候, 可能分配不到内存, 要采用紧凑技术. 耗费时间

## 基本分页存储管理怎么实现逻辑地址到物理地址的转换？从计算和硬件说. (## 讲一讲页表)
- 首先说说分页机制: 把内存分为一个个相同大小的页面. 进入内存的进程也分为一个个大小相同的页面
- 因为我们是非连续存储方式, 这些页面会放在内存的不同区域. 因此我们需要通过页表这一数据结构来找到相应内存地址
- 页表由一个个页表项组成, 页表项的元素有页号和内存块号. 
- 通常进程PCB中会存放页表起始地址, 页表长度
- 当我们的cPU加载该进程的运行环境, 页表起始地址和页表长度将会被加载到页表寄存器中， 
```
举个例子： 当前逻辑地址为85, 页面大小为10
我们的 页偏移量就是85 % 10 = 5, 页号就是85 / 10 = 8; 
```
- 由于寄存器是二进制存放比特位的, 因此我们直接截取位数即可. 
- 比如说低10位是偏移量, 那么高22位就是页号
- 我们拿到页号, 先和页表长度做对比, 如果>=页表长度, 要产生越界中断. 
- 否则我们通过页表地址, 找到相应页表的位置, 再通过内存偏移相应位数, 
- 找到内存块号
- 因此内存起始地址 = 内存块号 * 页面大小
- 物理地址 = 内存起始地址 + 页面偏移量, 这个页面偏移量可以通过去低位得出. 
## 如果有快表那又会是怎么样
- 快表是一种高速缓冲的存储器. 但是它存储能力是有限的 
- 如果想要找的页号在快表中, 就不用区内存找页表了, 速度也会快得多. 
- 如果在快表中没有的话, 我们在查询页表的时候也要同时更新快表. 以保证最近访问的内存都在快表中. 
## 什么是两级页表
```
假设一个页面4KB. 
我们有4G内存, 而且是32位操作系统

那我们的页偏移量最大 = 4KB = 4 * 2^10 = 2 ^ 12 B
因此偏移地址占12位

那么页面号占20位
那么就有最多 2 ^ 20个页表项

假设一个页表项4B 
4KB / 4B = 2 ^ 10 个页表项. 
```
- 如果只是单级页表, 这个页表将会很庞大, 不符合我们非连续 分配地址的行为
- 我们把页表分为两级, 一级是页目录表, 二级才是页表
- 那么每一个小页表的大小都是4KB, 那么就可以依次放到内存块之中
- 那么我们的逻辑地址将会被分割为三部分, 分别是一级页号, 二级页号和偏移量.  
- 页目录表存放一级页号和其对应的页表的内存块号
- 我们首先通过一级页号找到其内存块号
- 页表地址 = 内存块号 * 页面大小
- 之后正常继续执行即可

- 不过我们搞了二级页表以后, 以前本来是两次访存, 现在要三次访存, 空间换时间. 

## 说说基本分段管理
- 内存中分为不固定长的段, 程序员可以根据代码层次分段, 各段被加载到不同的内存地址中
- 我们寻找段的地址就要通过段表, 段表和页表一样//...(段号, 段长度, 段起始地址)
- // ... 
## 分段和分页有啥区别? 
- 分页是操作系统帮我们做好的, 对用户是不可见的, 分段需要程序员自己来分... 
- 分页查找物理地址, 只需给出逻辑地址
- 分段查找物理地址, 要给出段号, 段偏移量
## 什么是段页式管理方式? 
// ... 

```
PC --> 段号 | 页号 | 页偏移量
段表 --> 段号 | 页表长度 | 页表内存块号
页表 --> 页号 | 内存块号
```
// ... 

## 什么是虚拟内存管理方式? 
- 程序加入内存中, 不一定要把程序中所有的页一次性放入内存
- 因此在页表中会多了几项 --> (是否在内存中 | 访问标志 | 修改标志 ... )
- 加载进程运行环境, 查询页表, 发现页号对应的内存块不在内存中
- 那么线程会产生缺页中断, 阻塞等待IO. 
- 如果当前内存很满, 要发生页面置换, 就是选出一些页面换出内存 --> 页面置换算法. 
## 具体说下请求分页式管理方式? 页表和基本分页存储管理有什么区别? 
// ... 
## 说说页面置换算法？ 
- OPT: 哪个会最后用到
- FIFO: 先进先出
- LRU: 哪个最久没用到
- NRU clock算法 
-------------------------------------------------------------
## 讲一讲系统调用? 
- 首先说说用户态和内核态的区别: 
	1. 用CS寄存器的低2位判断是用户态还是内核态, 3为用户态, 0为内核态
	2. 内核态执行原语操作, 时钟管理, 进程调度等... 
	3. 用户态就是应用程序, 不能随意访问内核态的东西
	4. 所以会通过系统调用来进行操作内核. 
- 系统调用
	- 系统调用就是操作系统对上级提供的接口, 用来操作内核
	- 一般采用int 0x80中断进入系统调用. 
	- 为什么是0x80 - set_system_gate(0x80, &system_call) (system_call 是call eax里面存放的地址..)
		- 初始化时故意设下的"陷阱"
- 一般系统调用函数展开里面有
	- 把系统调用号放在eax --> int 0x80(syscall宏)
	- int 0x80 -> dpl设为3, eax取出系统调用号, call table中的函数地址. 
	- 函数地址正是内核态的函数地址, 而且这个地址刚好cs后面两位为0, 就是核心态
- CPL(CS (当前特权级)), DPL(目标特权级) 
	- int 0x80 --> dpl = 3
	- call table[...] --> cpl = 0

数据密集型应用系统设计