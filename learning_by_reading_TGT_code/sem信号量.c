#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>

union semun
{
int val;
struct semid_ds *buf;
unsigned short *array;
};
void P(int sid)
{
    struct sembuf sem_p;
    sem_p.sem_num = 0;
    sem_p.sem_op = -1;
    sem_p.sem_flg = 0;
    //sem_p.sem_flg = 0;
    if (semop(sid, &sem_p, 1) == -1)
    {
        printf("p op failed\n");
    }
}

void V(int sid)
{
    struct sembuf sem_p;
    sem_p.sem_num = 0;
    sem_p.sem_op = 1;
    sem_p.sem_flg = 0;
    //sem_p.sem_flg = 0;
    if (semop(sid, &sem_p, 1) == -1)
    {
        printf("v op failed\n");
    }
}
int main(void){
    int sem_id;
    int pid;

    if ((sem_id = semget((key_t)1334, 1, IPC_CREAT | 0666)) == -1){
        printf("semget failed\n");
        exit(1);
    }
    if (semctl(sem_id, 0, SETVAL, 1) == -1){
        printf("semctl failed\n");
        exit(1);
    }
    
    printf("begin fork\n");
    pid = fork();
    if (pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (pid == 0){
        sleep(1);
        P(sem_id);
        printf("child writing\n");
        sleep(10);
        printf("child finish writing\n");
        V(sem_id);
        sleep(50);
        return 0;
    }
    else{
        int i = 0;
        while (i < 20) {
            P(sem_id);
            printf("farther writing\n");
            sleep(1);
            printf("farther finish writing\n");
            V(sem_id);
            i++;
        }
    }

    semctl(sem_id, 0, IPC_RMID, NULL);

    return 0;

}

/*
信号量是一个计数器
struct  sembuf{
         unsigned short   sem_num;      //信号在信号集中的索引，0代表第一个信号，1代表第二个信号  
         short            sem_op;      //操作类型
         short            sem_flg;    //操作标志
     };
    下面详细介绍一下 sembuf 的几个参数：
--------------------------------------------------------------------------------------------------

    sem_op 参数：

                    sem_op > 0          信号加上 sem_op 的值，表示进程释放控制的资源；
 
                    sem_op = 0          如果没有设置 IPC_NOWAIT，则调用进程进入睡眠状态，直到信号                                         量的值为0；否则进程不回睡眠，直接返回 EAGAIN
 
                    sem_op < 0          信号加上 sem_op 的值。若没有设置 IPC_NOWAIT ，则调用进程阻
                                        塞，直到资源可用；否则进程直接返回EAGAIN

    sem_flg 参数：

         该参数可设置为 IPC_NOWAIT 或 SEM_UNDO 两种状态。只有将 sem_flg 指定为 SEM_UNDO 标志后，semadj （所指定信号量针对调用进程的调整值）才会更新。   此外，如果此操作指定SEM_UNDO，系统更新过程中会撤消此信号灯的计数（semadj）。此操作可以随时进行---它永远不会强制等待的过程。调用进程必须有改变信号量集的权限。
 
         sem_flg公认的标志是 IPC_NOWAIT 和 SEM_UNDO。如果操作指定SEM_UNDO，它将会自动撤消该进程终止时。

        在标准操作程序中的操作是在数组的顺序执行、原子的，那就是，该操作要么作为一个完整的单元，要么不。如果不是所有操作都可以立即执行的系统调用的行为取决于在个人sem_flg领域的IPC_NOWAIT标志的存在。
 -------------------------------------------------------------------------------------------------     

#include <stdio.h>
//……此处省略了头文件
void P(int sid)
{
    struct sembuf sem_p;
    sem_p.sem_num = 0;
    sem_p.sem_op = -1;
    sem_p.sem_flg = 0;
    if (semop(sid, &sem_p, 1) == -1)
    {
        perror("p op failed");
        exit(1);
    }
}

void V(int sid)
{
    struct sembuf sem_p;
    sem_p.sem_num = 0;
    sem_p.sem_op = 1;
    //sem_p.sem_flg = SEM_UNDO;
    sem_p.sem_flg = 0;
    if (semop(sid, &sem_p, 1) == -1)
    {
        perror("v op failed");
        exit(1);
    }
}

int main(int argc, char * argv[ ])
{
    pid_t pid;
    int fd;
    key_t key;
    int sid;

    if ((sid = semget((key_t)1324, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("createSemset");
        exit(-1);
    }
    if( -1==semctl(sid, 0, SETVAL, 1) )
    {
        perror("SETVAL");
        exit(1);
    }
    if ((pid=fork()) == -1)
    {
        perror("fork");
        exit(-1);
    }
    else if ( 0 == pid )
    {
        while(1)
        {
            P(sid); 
            printf("child writing\n");
            sleep(1);
            printf("child finish post\n");
            V(sid);
        }
    }
    else
    {
        while(1)
        {
            P(sid);
            printf("parent writing");
            sleep(1);
            printf("parent writing finish post\n");
            V(sid);
        }
    }
    return 0;
}
*/