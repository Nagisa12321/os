#include <unistd.h>
#include <time.h>
#include <sys/times.h>

#define HZ 100

void cpuio_bound(int last, int cpu_time, int io_time);
/*
 * 修改此模板，用 fork() 建立若干个同时运行的子进程，
 * 父进程等待所有子进程退出后才退出，每个子进程按照你的意愿做不同或相同的
 *  cpuio_bound()，从而完成一个个性化的样本程序。
 */
int main(int argc, char *argv[])
{
    int p1; 
    if (!fork())
    {
        printf("i am child [%d] and my parent is [%d]\n", getpid(), getppid());
        if (!fork())
        {
            printf("i am child [%d] and my parent is [%d]\n", getpid(), getppid());
            cpuio_bound(5, 2, 1);
        }
        else
        {
            wait(NULL);
            printf("after wait: [%d]\n", getpid());
        }
    }
    else if (!fork())
    {
        printf("i am child [%d] and my parent is [%d]\n", getpid(), getppid());
        cpuio_bound(4, 1, 1);
    }
    else
    {   
        /* wait for two son */
        wait(NULL);
        wait(NULL);
        printf("after wait for two son: [%d]\n", getpid());
    }
}
/*
 * 此函数按照参数占用CPU和I/O时间
 * last: 函数实际占用CPU和I/O的总时间，不含在就绪队列中的时间，>=0是必须的
 * cpu_time: 一次连续占用CPU的时间，>=0是必须的
 * io_time: 一次I/O消耗的时间，>=0是必须的
 * 如果last > cpu_time + io_time，则往复多次占用CPU和I/O，直到总运行时间超过last为止
 * 所有时间的单位为秒
 */
void cpuio_bound(int last, int cpu_time, int io_time)
{
    struct tms start_time, current_time;
    clock_t utime, stime;
    int sleep_time;

    while (last > 0)
    {
        /* CPU Burst */
        times(&start_time);
        /* 其实只有t.tms_utime是真正的CPU时间，但我们是在模拟一个只在用户状态运行的CPU大户
		 * 就像for(;;);所以把t.tms_stime加上很合理*/
        do
        {
            times(&current_time);
            utime = current_time.tms_utime - start_time.tms_utime;
            stime = current_time.tms_stime - start_time.tms_stime;
        } while (((utime + stime) / HZ) < cpu_time);
        last -= cpu_time;

        if (last <= 0)
            break;

        /* IO Burst */
        /* 用sleep(1)模拟1sI/O操作 */
        sleep_time = 0;
        while (sleep_time < io_time)
        {
            sleep(1);
            sleep_time++;
        }
        last -= sleep_time;
    }
}