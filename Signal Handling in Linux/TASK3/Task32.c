#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include "Task32.h"

sigset_t mask_block;
sigset_t mask_unblock;

pthread_t pthread_id[5] = {0};

int flag=0, t_flag=0;


void sig_chk_func(int sig_num)
{
	if (sig_num == SIGIO)
	{

		if (pthread_equal(pthread_self(),pthread_id[0]))
		{
			printf("Thread 1 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),pthread_id[1]))
		{
			printf("Thread 2 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),pthread_id[2]))
		{
			printf("Thread 3 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),pthread_id[3]))
		{
			printf("Thread 4 signal handler was called by the SIGIO event\n");
		}
		else if (pthread_equal(pthread_self(),pthread_id[4]))
		{
			printf("Thread 5 signal handler was called by the SIGIO event\n");
		}
		else
		{
			printf("Error signal handling\n");
		}
	}

	return;
}


void* thread_func1(void* arg)
{
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);
	printf("Waiting for signal in Thread 1\n");
	sleep(5);
	sigsuspend(&mask_unblock);
	printf("Received signal in Thread 1\n");
	printf("Thread 1 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}


void* thread_func2(void* arg)
{
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);
	printf("Waiting for signal in Thread 2\n");
	sleep(5);
	sigsuspend(&mask_unblock);
	printf("Received signal in Thread 2\n");
	printf("Thread 2 PID : %u\n", (unsigned int)pthread_self());
	//pthread_kill(pthread_id[3],SIGIO);
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}

void* thread_func3(void* arg)
{
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);
	printf("Waiting for signal in Thread 3\n");
	sleep(5);
	sigsuspend(&mask_unblock);
	printf("Received signal in Thread 3\n");
	printf("Thread 3 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}


void* thread_func4(void* arg)
{
	
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);
	if(pthread_sigmask(SIG_BLOCK, &mask_block, NULL) != 0)
	{
		printf("Error while masking signal in Main\n");
	}

	printf("Waiting for signal in Thread 4\n");
	sleep(5);
	sigsuspend(&mask_block);
	printf("Received signal in Thread 4\n");
	printf("Thread 4 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	// else
	// {
	// 	printf("Thread 4 is not registered so it won't recieve any signal");
	// }
	flag=1;
	
	return NULL;
}

void* thread_func5(void* arg)
{
	
	struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);

	printf("Waiting for signal in Thread 5\n");
	sleep(5);
	sigsuspend(&mask_block);
	printf("Received signal in Thread 5\n");
	printf("Thread 5 PID : %u\n", (unsigned int)pthread_self());
	if(t_flag==0)
	{
		t_flag=1;
		signal_tid(pthread_self());
	}
	flag=1;
	
	return NULL;
}



int main(int argc,char* argv[])
{
	
   printf("Registered threads are 1,2,3 and unregistered threads are 4,5 \n");

	pthread_create(&pthread_id[0],NULL,&thread_func1,NULL);
	pthread_create(&pthread_id[1],NULL,&thread_func2,NULL);
	pthread_create(&pthread_id[2],NULL,&thread_func3,NULL);
	pthread_create(&pthread_id[3],NULL,&thread_func4,NULL);
	pthread_create(&pthread_id[4],NULL,&thread_func5,NULL);
    

	register_tid(pthread_id[0]);
	register_tid(pthread_id[1]);
	register_tid(pthread_id[2]);
	sleep(5);

	
    struct sigaction my_sig_act;

	my_sig_act.sa_handler = &sig_chk_func;

	sigemptyset(&my_sig_act.sa_mask);

	sigaction(SIGIO, &my_sig_act, NULL);

	sigemptyset(&mask_block);
	sigaddset(&mask_block, SIGIO);
	
	sigfillset(&mask_unblock);
	sigdelset(&mask_unblock, SIGIO);

	
	if(pthread_sigmask(SIG_BLOCK, &mask_block, NULL) != 0)
	{
		printf("Error while masking signal in Main\n");
	}
	
	kill(getpid(),SIGIO);
	while(flag == 0);
	sleep(5);
	printf("Thus, only the registered threads receive the signal\n");
	return 0;
}



