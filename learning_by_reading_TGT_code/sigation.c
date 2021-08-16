#include<signal.h>
#include<stdio.h>

int signal_catch(int signo){
    printf("%d", signo);
}
int main(void){
    struct sigaction sa_old;
	struct sigaction sa_new;
    sa_new.sa_handler = signal_catch;
	sigemptyset(&sa_new.sa_mask);
    //清空信号集
    sigaddset(&sa_new.sa_mask, SIGQUIT);
    //将3号信号添加到信号集，即：3号信号就是我们在处理2号信号的时候需要屏蔽的那个信号
	sa_new.sa_flags = 0;
    //sa_flags：通常设置为0，表使用默认属性：为0的时候，可以屏蔽正在处理的信号
    //（若在处理2号信号时又有2号信号，则此时传来的2号信号会被屏蔽）
	sigaction(SIGINT, &sa_new, &sa_old);
	sigaction(SIGTERM, &sa_new, &sa_old);
	sigaction(SIGCHLD, &sa_new, &sa_old);

}