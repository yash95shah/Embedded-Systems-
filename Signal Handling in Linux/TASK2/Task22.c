#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <syscall.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
sem_t sem;
struct timespec t1;

static void sig_usr (int signo)
{
	int value;
	if (signo==SIGUSR1)
	{
		printf("\n signal handler called");
	}
	sem_post(&sem);
}

void* thread_func(void* id)
{
int value,ret,i=0;
printf("Thread id of the thread function is %ld \n",syscall(SYS_gettid));
printf("\n Waiting for semaphore \n");
sem_wait(&sem);
printf("\n Got semaphore \n");
}

int main()
{

	int ret,value;
	struct sigaction actions;
	pthread_t p1,p2;
	actions.sa_handler=sig_usr;
	sigemptyset(&actions.sa_mask);

	printf("\n Thread id of main %ld \n",syscall(SYS_gettid));
	if(sigaction(SIGUSR1,&actions,NULL)==-1)
	{
		printf("\n Sigaction error");

	}
	sem_init(&sem,0,1);
	sem_wait(&sem);
	ret=pthread_create(&p1,NULL,thread_func,NULL);
	if (ret!=0){
 printf("\n Pthread create error");
	}
	sleep(5);

	pthread_kill(p1,SIGUSR1);
	pthread_join(p1,NULL);
	return 0;
}