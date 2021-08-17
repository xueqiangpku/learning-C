#include<signal.h>
#include<stdio.h>

int signal_catch(int signo){
    printf("gogo %d\n", signo);
}
int main(void){
    struct sigaction sa_old;
	struct sigaction sa_new;
    sa_new.sa_handler = signal_catch;
	sigemptyset(&sa_new.sa_mask);
    //清空信号集
    //sigaddset(&sa_new.sa_mask, SIGQUIT);
    //sigaddset(&sa_new.sa_mask, SIGTSTP); 
    //将3号信号添加到信号集，即：3号信号就是我们在处理2号信号的时候需要屏蔽的那个信号
	sa_new.sa_flags = 0;
    //sa_flags：通常设置为0，表使用默认属性：为0的时候，可以屏蔽正在处理的信号
    //（若在处理2号信号时又有2号信号，则此时传来的2号信号会被屏蔽）
	sigaction(SIGINT, &sa_new, &sa_old);
	sigaction(SIGTERM, &sa_new, &sa_old);
	sigaction(SIGCHLD, &sa_new, &sa_old);
    while(1);
    return 0;
}

/*sigset_t sa_mask 是一个信号集，在调用该信号捕捉函数之前，将需要block的信号加入这个sa_mask，仅当信号捕捉函数正在执行时，才阻塞sa_mask中的信号，当从信号捕捉函数返回时进程的信号屏蔽字复位为原先值。
Q1：这个复位动作是sigaction函数内部处理，还是由调用者自己处理呢？

　　由sigaction函数自动复位，不用我自己再去处理。

Q2：设置sa_mask的目的？

　　在调用信号处理程序时就能阻塞某些信号。注意仅仅是在信号处理程序正在执行时才能阻塞某些信号，如果信号处理程序执行完了，那么依然能接收到这些信号。
在信号处理程序被调用时，操作系统建立的新信号屏蔽字包括正被递送的信号，也就是说自己也被阻塞，除非设置了SA_NODEFER。
因此保证了在处理一个给定的信号时，如果这种信号再次发生，通常并不将它们排队，所以如果在某种信号被阻塞时它发生了5次，那么对这种信号解除阻塞后，其信号处理函数通常只会被调用一次。

Q3：对于不同信号，当信号A被捕捉到并信号A的handler正被调用时，信号B产生了，
　　3.1如果信号B没有被设置阻塞，那么正常接收信号B并调用自己的信号处理程序。另外，如果信号A的信号处理程序中有sleep函数，那么当进程接收到信号B并处理完后，sleep函数立即返回(如果睡眠时间足够长的话)
　　3.2如果信号B有被设置成阻塞，那么信号B被阻塞，直到信号A的信号处理程序结束，信号B才被接收并执行信号B的信号处理程序。

　　　　如果在信号A的信号处理程序正在执行时，信号B连续发生了多次，那么当信号B的阻塞解除后，信号B的信号处理程序只执行一次。
　　　　如果信号A的信号处理程序没有执行或已经执行完，信号B不会被阻塞，正常接收并执行信号B的信号处理程序。
Q4：对于相同信号，当一个信号A被捕捉到并信号A的handler正被调用时，
　　4.1 又产生了一个信号A，第二次产生的信号被阻塞，直到第一次产生的信号A处理完后才被递送；
　　4.2 如果连续产生了多次信号，当信号解除阻塞后，信号处理函数只执行一次。
*/

/*
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void func(int signal)
{
    printf("%d号信号被捕捉。\n", signal);
}

int main(void)

{

    struct sigaction act, oldact;
    act.sa_handler = func;
    act.sa_flags = 0;//为0的时候，可以屏蔽正在处理的信号（若在处理2号信号时又有2号信号，则>会被屏蔽）
    sigemptyset(&act.sa_mask);//sa_mask是一个临时信号集，将其清零（初始化I）
    sigaddset(&act.sa_mask, SIGQUIT);//将3号信号添加到信号集，即：3号信号就是我们在处理2号信号的时候需要屏蔽的那个信号，也许可以添加其他信号，比如20号信号
    sigaddset(&act.sa_mask, SIGTSTP);
    int s_ret = sigaction(SIGINT, &act, &oldact);

    if (0>s_ret)

    {

        perror("sigaction error");

        exit(1);

    }

    while (1);

    return 0;

} */