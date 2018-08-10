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

pthread_t tid1,tid2;
int flag=0;

static void sig_usr(int signo){
	if(signo==SIGUSR1)
	{
		printf("\n Signal handler called...........");
		flag=1;
	}
}

void* thread_fn1(void* id)
{
	int i,j,k,count;
	
	printf("\n Higher priority thread called \n");
	 for(i=0;i<900;i++)
	 {
	 	for(j=0;j<400;j++)
	 	{
	 		for(k=0;k<1000;k++)
	 		{
	 			count+=count;
	 		}
	 	}
	 }
	
	
	pthread_kill(tid2, SIGUSR1);
	printf("\n Thread id of Running Thread function  %ld",syscall(SYS_gettid));
	//sleep(2);
	printf("\n Higher priority thread exiting");
pthread_exit(NULL);
}

void* thread_fn2(void* id)
{
	printf("\n Lower priority thread called");
	sleep(2);
	while(!flag)
	{

	}
	
	printf("\n Thread id of the Runnable Thread function  %ld \n",syscall(SYS_gettid));
pthread_exit(NULL);
}




int main()
{
	int ret;
	struct sched_param mf,thr1,thr2;
	struct sigaction actions;	
	sigset_t main__set;
	pthread_attr_t thread1,thread2;


	actions.sa_handler=sig_usr;
	sigemptyset(&actions.sa_mask);
	actions.sa_flags=0;

	memset(&mf,0,sizeof(struct sched_param));
	memset(&thr1,0,sizeof(struct sched_param));
	memset(&thr2,0,sizeof(struct sched_param));

	pthread_attr_init (&thread1);                      
        //pthread_attr_getschedparam (&thread1, &thr1);           
        thr1.sched_priority = 6;                    
        pthread_attr_setschedpolicy(&thread1, SCHED_FIFO);      
        pthread_attr_setschedparam (&thread1, &thr1);
	
	pthread_attr_init (&thread2);                      
        //pthread_attr_getschedparam (&thread2, &thr2);           
        thr2.sched_priority = 1;                    
        pthread_attr_setschedpolicy(&thread2, SCHED_FIFO);      
        pthread_attr_setschedparam (&thread2, &thr2);
	

	printf("\n Thread id of main is %ld",syscall(SYS_gettid));
	if (sigaction(SIGUSR1,&actions,NULL)==-1)
	{
		printf("\n Sig action error");
	}

	mf.sched_priority=9;
	if (sched_setscheduler (0,SCHED_RR,&mf) !=0)
	{
		printf("error setting schedule policy\n");
	}
	sched_setparam(0,&mf);

	//thr1.sched_priority=6;
		
	//thr2.sched_priority=1;
		


	ret=pthread_create(&tid2,&thread1,thread_fn2,NULL);
	if(ret<0)
	{
		printf("\n error pthread");
	}

	ret=pthread_create(&tid1,&thread2,thread_fn1,NULL);
	if(ret<0)
	{
		printf("\n error pthread");
	}
	sleep(2);

	if (sched_setscheduler (0,SCHED_FIFO,&thr1) !=0)
	{
		printf("error setting schedule policy\n");
	}
	// sched_setparam(0,&thr1);
	if (sched_setscheduler (0,SCHED_FIFO,&thr2) !=0)
	{
		printf("error setting schedule policy\n");
	}
	// sched_setparam(0,&thr2);

	while (flag=0)
	{

	}

	return 0;


}

