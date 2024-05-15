#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

// 假设 A 和 n 在此之前已经定义和初始化  
const int n=1000;
float A[n][n]; // 或者使用动态分配的二维数组  

void init()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            A[i][j] = 0;
        }
        A[i][i] = 1.0;
        for (int j = i + 1; j < n; j++)
            A[i][j] = rand() % 100;
    }

    for (int i = 0; i < n; i++)
    {
        int k1 = rand() % n;
        int k2 = rand() % n;
        for (int j = 0; j < n; j++)
        {
            A[i][j] += A[0][j];
            A[k1][j] += A[k2][j];
        }
    }
}

typedef struct {
    int k; // 消去的轮次  
    int t_id; // 线程id  
} threadParam_t;

void* threadFunc(void* param) {
    threadParam_t* p = (threadParam_t*)param;
    int k = p->k; // 消去的轮次  
    int t_id = p->t_id; // 线程编号  
    int i = k + t_id + 1; // 获取自己的计算任务  

    // 确保 k 不是最后一列  
    if (k < n - 1) {
        for (int j = k + 1; j < n; ++j) {
            A[i][j] = A[i][j] - A[i][k] * A[k][j];
        }
        A[i][k] = 0;
    }

    pthread_exit(NULL);
}

int main() {
    init();
    struct timeval tstart, tend;
    double timeUsed;
    gettimeofday(&tstart, NULL);
    for (int k = 0; k < n - 1; ++k) { // 注意循环条件，因为我们要避免除以0  
        // 主线程做除法操作  
        for (int j = k + 1; j < n; ++j) {
            if (A[k][k] != 0.0) { // 检查分母是否为0  
                A[k][j] = A[k][j] / A[k][k];
            }
        }
        A[k][k] = 1.0; // 将主对角线上的元素设为1  

        // 创建工作线程，进行消去操作  
        int worker_count = n - 1 - k; // 工作线程数量  
        pthread_t* handles = (pthread_t*)malloc(worker_count * sizeof(pthread_t));
        threadParam_t* param = (threadParam_t*)malloc(worker_count * sizeof(threadParam_t));

        // 分配任务  
        for (int t_id = 0; t_id < worker_count; ++t_id) {
            param[t_id].k = k;
            param[t_id].t_id = t_id;
            // 注意这里应该调用 pthread_create 并传入正确的参数  
            pthread_create(&handles[t_id], NULL, threadFunc, &param[t_id]);
        }

        // 主线程挂起等待所有的工作线程完成此轮消去工作  
        for (int t_id = 0; t_id < worker_count; ++t_id) {
            pthread_join(handles[t_id], NULL);
        }

        // 释放内存  
        free(handles);
        free(param);
    }
    gettimeofday(&tend, NULL);
    timeUsed = 1000000 * (tend.tv_sec - tstart.tv_sec) + tend.tv_usec - tstart.tv_usec;
    std::cout << " Time=" << timeUsed / 1000 << " ms" << std::endl;
    return 0;
}