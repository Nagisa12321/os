#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct running_task {
    int state;        // 进程状态: 运行1等待0
    int create;       // 开始时间片
    int end;          // 结束时间片
    int running_time; // 运行总时间
    int waiting_time; // 等待总时间

    int w;            // 上次等待的时间
    int r;            // 上次运行的时间
};

void dealWithProcess(int pid, char state, int time_peaces, struct running_task * tasks);

int main(int argc, char const *argv[]) {

    // 只计算12/13/14/15
    // 平均周转时间: 进程结束时间片 - 进程开始时间片
    // 平均等待时间: 进程等待时间
    // 吞吐率: 完成多少道作业/总时间
    FILE *log = fopen("process.log", "r");

    struct running_task *tasks = (struct running_task *)malloc(4 * sizeof(struct running_task));
    // init
    for (int i = 0; i < 4; i++) {
        tasks[i].state = 0;
        tasks[i].create = 0;
        tasks[i].end = 0;
        tasks[i].running_time = 0;
        tasks[i].waiting_time = 0;

        tasks[i].w = 0;
        tasks[i].r = 0;
    }
    char *bf = (char *)malloc(sizeof(char) * 1024);
    char *buf;
    while (buf = fgets(bf, 1024, log)) {
        int pid = 0;
        char state = 0;
        int time_peaces = 0;

        int i = 0;
        while (buf[i] != '\n') {
            if (buf[i] >= 'A' && buf[i] <= 'Z') {
                state = buf[i];
            }
            else if (buf[i] == '\t') {
                /* do nothing */
            }
            else {
                if (state == 0) {
                    pid = pid * 10 + buf[i] - '0';
                }
                else {
                    time_peaces = time_peaces * 10 + buf[i] - '0';
                }
            }
            i++;
        }
        dealWithProcess(pid, state, time_peaces, tasks);
    }
    for (int i = 0; i < 4; i++) {
        printf("pid = %d, create = %d, end = %d, waiting_time = %d, running_time = %d\n", 
                    12 + i, tasks[i].create, tasks[i].end, tasks[i].waiting_time, tasks[i].running_time);
    }
    return 0;
}

void dealWithProcess(int pid, char state, int time_peaces, struct running_task * tasks) {
    if (pid < 12 || pid > 15) return;

    switch (state) {
        case 'N': // 新建
            tasks[pid - 12].w = time_peaces;
            tasks[pid - 12].create = time_peaces;
            break;
        case 'J': // 就绪
            tasks[pid - 12].w = time_peaces;
            tasks[pid - 12].waiting_time += time_peaces - tasks[pid - 12].w;
            break;
        case 'R': // 运行
            tasks[pid - 12].r = time_peaces;
            tasks[pid - 12].waiting_time += time_peaces - tasks[pid - 12].w;
        case 'W': // 阻塞
            tasks[pid - 12].w = time_peaces;
            tasks[pid - 12].running_time += time_peaces - tasks[pid - 12].r;
            break;
        case 'E': // 退出
            tasks[pid - 12].running_time += time_peaces - tasks[pid - 12].r;
            tasks[pid - 12].end = time_peaces;
            break;
    }
}
/*
PS C:\Users\12164\Documents\HBuilderProjects\OS\experiment\exp3> .\log.exe
pid = 12, create = 16035, end = 16648, waiting_time = 2, running_time = 3
pid = 13, create = 16038, end = 16647, waiting_time = 16, running_time = 1
pid = 14, create = 16038, end = 16550, waiting_time = 197, running_time = 0
pid = 15, create = 16054, end = 16646, waiting_time = 295, running_time = 96

tips: 还没计算sleep的时间
*/