#include <stdio.h>  
#include <stdlib.h>  
#include <pthread.h>  
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

// ���� A �� n �ڴ�֮ǰ�Ѿ�����ͳ�ʼ��  
const int n=1000;
float A[n][n]; // ����ʹ�ö�̬����Ķ�ά����  

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
    int k; // ��ȥ���ִ�  
    int t_id; // �߳�id  
} threadParam_t;

void* threadFunc(void* param) {
    threadParam_t* p = (threadParam_t*)param;
    int k = p->k; // ��ȥ���ִ�  
    int t_id = p->t_id; // �̱߳��  
    int i = k + t_id + 1; // ��ȡ�Լ��ļ�������  

    // ȷ�� k �������һ��  
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
    for (int k = 0; k < n - 1; ++k) { // ע��ѭ����������Ϊ����Ҫ�������0  
        // ���߳�����������  
        for (int j = k + 1; j < n; ++j) {
            if (A[k][k] != 0.0) { // ����ĸ�Ƿ�Ϊ0  
                A[k][j] = A[k][j] / A[k][k];
            }
        }
        A[k][k] = 1.0; // �����Խ����ϵ�Ԫ����Ϊ1  

        // ���������̣߳�������ȥ����  
        int worker_count = n - 1 - k; // �����߳�����  
        pthread_t* handles = (pthread_t*)malloc(worker_count * sizeof(pthread_t));
        threadParam_t* param = (threadParam_t*)malloc(worker_count * sizeof(threadParam_t));

        // ��������  
        for (int t_id = 0; t_id < worker_count; ++t_id) {
            param[t_id].k = k;
            param[t_id].t_id = t_id;
            // ע������Ӧ�õ��� pthread_create ��������ȷ�Ĳ���  
            pthread_create(&handles[t_id], NULL, threadFunc, &param[t_id]);
        }

        // ���̹߳���ȴ����еĹ����߳���ɴ�����ȥ����  
        for (int t_id = 0; t_id < worker_count; ++t_id) {
            pthread_join(handles[t_id], NULL);
        }

        // �ͷ��ڴ�  
        free(handles);
        free(param);
    }
    gettimeofday(&tend, NULL);
    timeUsed = 1000000 * (tend.tv_sec - tstart.tv_sec) + tend.tv_usec - tstart.tv_usec;
    std::cout << " Time=" << timeUsed / 1000 << " ms" << std::endl;
    return 0;
}