#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/poll.h>
#include <pthread.h>
#include <unistd.h>


void* calculation_funct(void *id);
void* mouse_double_click(void *id);//This is a mouse double click detection function to demonstrate the use of setjmp and longjmp
long long int result;
static void sig_usr(int);
jmp_buf buffer_jump;
int tmp_flag, temp;
struct timeval cur,end;
// For execution type in sudo ./Task1 /dev/input/eventN where eventN is the corresponding mouse event in your computer.
int main(int argc, char* argv[])
{
	int fd,count, i, ret, nfd;
	pthread_t p1,p2;//This indicates the ids of the respective pthreads
	struct sigaction actions;
	//Here we need to give the event number of the mouse event for eg in my machine it is /dev/input/event3//
	if(argc<2)
	{
		printf("\n Specify mouse event action to perform this task\n");//This will be displayed when there is no mention of the argument //
		return 0;
	}
	nfd= open(argv[1],O_RDONLY);//This variable will open the file whose location is mentioned as a argument //
	if (nfd<0)
	{
		printf("\n Open failed \n");

	}

	actions.sa_handler=sig_usr;
	sigemptyset (&actions.sa_mask);

	if(sigaction(SIGUSR1,&actions,NULL)==-1)
	{
		printf("\n Sigaction error \n");

	}

tmp_flag=0;

gettimeofday(&cur,NULL);

ret= pthread_create(&p1,NULL,mouse_double_click,(void*) &nfd);
if (ret<0){
	printf("\n Pthread creation error \n");
	}

ret = pthread_create(&p2,NULL,calculation_funct,NULL);
if (ret<0){
	printf("\n Pthread creation error \n");
	}

	if (setjmp(buffer_jump)==-1)
	{
		printf("Imprecise Computation Result %lld \n",result);
		exit(0);
	}

	pthread_join(p1,NULL);
	pthread_join(p2,NULL);

	printf("Final result of computation is %lld \n",result);
	

return 0;
}

// Signal Handler Function //

static void sig_usr(int signo)
{
	tmp_flag =1;

}

// Computation Function //

void* calculation_funct(void* id)
{
	int i,j,k;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			for(k=0;k<1000;k++)
			{
				result +=(k-j-i);	
			if (tmp_flag)
			{
				longjmp(buffer_jump,-1);
			}

		}
	}
}
	pthread_exit(NULL);
}


void* mouse_double_click(void *id)
{
	int fd,ret;

	fd=*(int*)id;
	struct input_event ie;
	struct pollfd poll_val=
	{
		.fd=fd,
		.events=POLLIN,
		.revents=0,
	};

	while(1)
	{
		ret =poll(&poll_val,1,10000);
		if(poll_val.revents &POLLIN)
		{
			poll_val.revents=0;
			ret=read(fd,&ie,sizeof(struct input_event));
			if((ie.type==EV_KEY) && (ie.code==BTN_RIGHT) && ie.value==0)
			{
				gettimeofday(&end,NULL);
				temp=((end.tv_sec - cur.tv_sec)*1000000+ (end.tv_usec- cur.tv_usec))/1000;
				if (temp<250)
				{
					printf("\n Right double click detected \n");
					kill(getpid(),SIGUSR1);
					break;
				}
				else
				{
					cur=end;
                  
				}
			}
		
			}	
			else
			{
				printf("No mouse click detected\n");
				kill(getpid(),SIGUSR1);
				break;

			}
			
		}
		
	

}