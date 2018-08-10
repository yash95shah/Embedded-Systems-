#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <syscall.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <syscall.h>

struct timespec time1,time2;
void* thread_func(void* id);
static void sig_usr(int signo);

int main()
{
	pthread_t tid;
	int ret;
	struct sigaction actions;
	sigset_t main_set;
	time1.tv_sec=10;
	printf("\n Thread is of main() %ld",syscall(SYS_gettid));
	ret=pthread_create(&tid,NULL,thread_func,NULL);
	if (ret<0)
	{
		printf("\n Thread create error \n");
	}
	actions.sa_handler=sig_usr;
	sigemptyset (&actions.sa_mask);
	actions.sa_flags=SA_RESTART;
	if(sigaction(SIGUSR1,&actions,NULL)==-1)
	{
		printf("\n Sigaction error \n");
	}
sleep(2);

pthread_kill(tid,SIGUSR1);

pthread_join(tid,NULL);

return 0;
}

static void sig_usr(int signo)
{
	if(signo==SIGUSR1)
	{
		printf("\n Signal Handler used \n");
	}
}

void* thread_func(void* id)
{
	int ret;
	printf("\n In thread \n");
	printf("\n Thread id is of thread function %ld \n",syscall(SYS_gettid));
	ret= nanosleep(&time1,&time2);
	if (ret<0)
	{
		printf("\n Time remaining is %f \n",time2.tv_sec + (float)time2.tv_nsec/1000000000);

	}
}
