#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <setjmp.h>
#include <sys/poll.h>
#include <unistd.h>
#include <time.h>
#include <syscall.h>

sigset_t signal_set;
int flag;
pthread_t tid[3];

static void sig_usr(int signo)
{   if(signo==SIGIO)
{
	printf("\n Signal Handler called for signal SIGIO \n");
}

}
void* thread_fn1(void* id);
void* thread_fn2(void* id);
void* thread_fn3(void* id);

int main()
{
	int i,j,ret;
	struct sigaction actions;
	sigset_t main_set;

	flag=0;
	actions.sa_handler=sig_usr;
	sigemptyset(&actions.sa_mask);

	if(sigaction(SIGIO,&actions,NULL)==-1)
	{
		printf("\n Sigaction error \n");
	}
	sigemptyset(&main_set);
	sigaddset(&main_set,SIGIO);

	sigfillset(&signal_set);
	sigdelset(&signal_set,SIGIO);


	if(pthread_sigmask(SIG_BLOCK,&main_set,NULL) !=0)
	{
		printf("Signal mask error\n");
	}
   
	ret=pthread_create(&tid[0],NULL,thread_fn1,NULL);
	if(ret<0)
	{
		printf("Pthread creation error\n" );
	}
	
	ret=pthread_create(&tid[1],NULL,thread_fn2,NULL);
	if(ret<0)
	{
		printf("Pthread creation error\n");

	}

	 ret=pthread_create(&tid[2],NULL,thread_fn3,NULL);
	if(ret<0)
	{
		printf("Pthread creation error\n" );
	}
	
	sleep(5);
	
      if(flag!=1)
		{
			kill(getpid(),SIGIO);

		}

    //sleep(5);
		//printf("Exiting program\n" );
		while(flag==0)
		{

		}
		sleep(1);
	printf("Exiting program\n" );
	return 0;
}

	void* thread_fn1(void* id)
	{
		printf("\n Thread 1 waiting for signal \n");
		
		sleep(5);
		
	sigsuspend(&signal_set);
	    flag=1;
		printf("Thread 1 with thread id %d got signal\n",(unsigned int)pthread_self() );
		return NULL;
     	pthread_exit(NULL);

	}

	void* thread_fn2(void* id)
	{
		printf("\n Thread 2 waiting for signal \n");
		sleep(5);
		
		sigsuspend(&signal_set);
		flag=1;
		printf("Thread 2 with thread id %d got signal\n",(unsigned int)pthread_self() );
		return NULL;
     	pthread_exit(NULL);

	}

	void* thread_fn3(void* id)
	{
		printf("\n Thread 3 waiting for signal \n");
		sleep(5);
		sigsuspend(&signal_set);
		flag=1;

		printf("Thread 3 with thread id %d got signal\n",(unsigned int)pthread_self() );
		return NULL;
     	pthread_exit(NULL);

	}


