#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#define SPI_DEVICE_NAME "/dev/displayspidev"
#define PULSE_DEVICE_NAME "/dev/usensor"

typedef struct 
{
	int threadId;
	int fd;
}ThreadParams;


pthread_mutex_t mutex;

double distance;
// This is just a normal test function to determine if the patterns work or not. This will display the letters 'I','N','N','O','V'
'A','T','I','O','N'//

void *thread_transmit_spi_user(void *data)
{
	int retValue,fd;
	int i,j,k;
	ThreadParams *tparams = (ThreadParams*)data;
    char patternBuffer [10][8] = {
		{0x81,0x81,0x81,0xff,0x81,0x81,0x81,0x81}, // 'I'
		{0xff,0x02,0x04,0x08,0x10,0x20,0x40,0xff}, // 'N'
		{0xff,0x02,0x04,0x08,0x10,0x20,0x40,0xff}, // 'N'
		{0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff}, // 'O'
		{0x0f,0x10,0x60,0xc0,0x60,0x10,0x0f,0x00}, // 'V'
		{0xff,0x11,0x11,0x11,0x11,0x11,0x11,0xff}, // 'A'
		{0x01,0x01,0x01,0xff,0x01,0x01,0x01,0x01}, // 'T'
		{0x81,0x81,0x81,0xff,0x81,0x81,0x81,0x81}, // 'I'
		{0xff,0x81,0x81,0x81,0x81,0x81,0x81,0xff}, // 'O'
		{0xff,0x02,0x04,0x08,0x10,0x20,0x40,0xff}, // 'N'
	};
	unsigned int sequenceBuffer[20] = {0, 100, 1, 200, 3, 300, 4, 400, 5, 500, 6, 600, 7, 700, 8, 800, 0, 0};
	//printf("thread_transmit_spi Start\n");

	fd = open(SPI_DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("Can not open device file fd_spi.\n");
		return 0;
	}
	else
	{
		//printf("fd_spi device opened succcessfully.\n");
	}
	
	retValue = spi_led_ioctl(fd, patternBuffer, sizeof(patternBuffer));
	while(1)
	{
		retValue = spi_led_write(fd, sequenceBuffer, sizeof(sequenceBuffer));
		usleep(10000);
	}

	//printf("thread_transmit_spi fd = %d\n",fd);
	close(fd);
	pthread_exit(0);
}

char numberBuffer[10][4] = {
		{0x7e, 0x81, 0x81, 0x7e},
		{0x84, 0x82, 0xff, 0x80},
		{0x61, 0x91, 0x91, 0x8e},
		{0x81, 0x99, 0x99, 0x7e},
		{0x0f, 0x08, 0x08, 0xff},
		{0x8e, 0x91, 0x91, 0x61},
		{0x7e, 0x89, 0x89, 0x71},
		{0x01, 0x01, 0x01, 0xfe},
		{0x6e, 0x91, 0x91, 0x6e},
		{0x8e, 0x91, 0x91, 0x7e},
		};

void *distance_transmit_spi(void *data)
{
	int retValue,fd;
	int i,j,k;
	int leftNumber = 0, rightNumber = 0;
	ThreadParams *tparams = (ThreadParams*)data;
	double distance_previous = 0, distance_current = 0, distance_diff = 0;

	char tempBuffer[8];
	unsigned int sequenceBuffer[4];
	//printf("thread_transmit_spi Start\n");

	fd = open(SPI_DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("Can not open device file fd_spi.\n");
		return 0;
	}
	else
	{
		//printf("fd_spi device opened succcessfully.\n");
	}
	
	while(1)
	{
		for(i=0;i<100;i++)
		{
			pthread_mutex_lock(&mutex);
			distance_current = distance;
			pthread_mutex_unlock(&mutex);
			distance_diff = distance_current - distance_previous;
			printf("Distance = %0.2f cm\n",distance_current);
		
			if(distance_current > 100)
			{
				rightNumber = 9;
				leftNumber = 9;
			}
			else
			{
				rightNumber = (int)distance_current%10;
				leftNumber = (int)distance_current/10;
			}
			
			tempBuffer[0] = numberBuffer[leftNumber][0];
			tempBuffer[1] = numberBuffer[leftNumber][1];
			tempBuffer[2] = numberBuffer[leftNumber][2];
			tempBuffer[3] = numberBuffer[leftNumber][3];

			tempBuffer[4] = numberBuffer[rightNumber][0];
			tempBuffer[5] = numberBuffer[rightNumber][1];
			tempBuffer[6] = numberBuffer[rightNumber][2];
			tempBuffer[7] = numberBuffer[rightNumber][3];

			spi_led_ioctl(fd,tempBuffer);
			
			sequenceBuffer[0] = 0;
			sequenceBuffer[1] = 1000;
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);
			usleep(10000);
		}
	}
	
	close(fd);
	pthread_exit(0);
}



void *thread_Ultrasonic_distance(void *data)
{
	int fd;
	int pulseWidth;
	fd = open(PULSE_DEVICE_NAME, O_RDWR);
	while(1)
	{
		write_pulse(fd);
		pulseWidth = read_pulse(fd);
		pthread_mutex_lock(&mutex);
		distance = pulseWidth * 0.017;
		pthread_mutex_unlock(&mutex);
		usleep(100000);
	}
	close(fd);
	pthread_exit(0);
}

void *spi_number_transmit(void *data)
{
	int retValue,fd;
	int i,j,k;
	int leftNumber = 0, rightNumber = 0;
	ThreadParams *tparams = (ThreadParams*)data;
	double distance_previous = distance, distance_current = distance, distance_diff = 0;
	char tempBuffer[8];
	unsigned int sequenceBuffer[4];
	//printf("thread_transmit_spi Start\n");

	fd = open(SPI_DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("Can not open device file fd_spi.\n");
		return 0;
	}
	else
	{
		//printf("fd_spi device opened succcessfully.\n");
	}
	sleep(1);
	while(1)
	{
		for(i=0;i<100;i++)
		{
			pthread_mutex_lock(&mutex);
			distance_current = distance;
			pthread_mutex_unlock(&mutex);
			distance_diff = distance_current - distance_previous;
			printf("Distance = %0.2f cm \n",distance_current);
		
			rightNumber = i%10;
			leftNumber = i/10;
			
			tempBuffer[0] = numberBuffer[leftNumber][0];
			tempBuffer[1] = numberBuffer[leftNumber][1];
			tempBuffer[2] = numberBuffer[leftNumber][2];
			tempBuffer[3] = numberBuffer[leftNumber][3];

			tempBuffer[4] = numberBuffer[rightNumber][0];
			tempBuffer[5] = numberBuffer[rightNumber][1];
			tempBuffer[6] = numberBuffer[rightNumber][2];
			tempBuffer[7] = numberBuffer[rightNumber][3];

			spi_led_ioctl(fd,tempBuffer);
			
			sequenceBuffer[0] = 0;
			if(distance_current > 200)
			{
				sequenceBuffer[1] = 1000;
			}
			else if(distance_current < 2)
			{
				sequenceBuffer[1] = (int)(10 * 5);
			}
			else
			{
				sequenceBuffer[1] = (int)(distance_current * 5);
			}
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);
			usleep(10000);
		}
	}
	//printf("thread_transmit_spi fd = %d\n",fd);
	close(fd);
	pthread_exit(0);
}

char patternBuffer[4][8] = {
		{0x08, 0x90, 0xf0, 0x10, 0x10, 0x37, 0xdf, 0x98},
		{0x20, 0x10, 0x70, 0xd0, 0x10, 0x97, 0xff, 0x18},
		{0x98, 0xdf, 0x37, 0x10, 0x10, 0xf0, 0x90, 0x08},
		{0x18, 0xff, 0x97, 0x10, 0xd0, 0x70, 0x10, 0x20},
		};

void *spi_dog_transmit(void *data)
{
	int retValue,fd;
	ThreadParams *tparams = (ThreadParams*)data;
	double distance_previous = 0, distance_current = 0, distance_diff = 0, distance_threshhold=0;
	char new_direction = 'L', old_direction = 'L';
	unsigned int timeToDisplay = 0;
	unsigned int sequenceBuffer[4];

	fd = open(SPI_DEVICE_NAME, O_RDWR);
	if(fd < 0)
	{
		printf("Can not open device file fd_spi.\n");
		return 0;
	}
	else
	{
		//printf("fd_spi device opened succcessfully.\n");
	}
	
	spi_led_ioctl(fd,patternBuffer);
	while(1)
	{
		pthread_mutex_lock(&mutex);
		distance_current = distance;
		pthread_mutex_unlock(&mutex);
		distance_diff = distance_current - distance_previous;
		distance_threshhold = distance_current / 10.0;

		printf("Distance = %0.2f cm\n",distance_current);

		if((distance_diff > -distance_threshhold) && (distance_diff < distance_threshhold))
		{
			new_direction = old_direction;
		}
		else if(distance_diff > distance_threshhold)
		{
			new_direction = 'R';
		}
		else if(distance_diff < -distance_threshhold)
		{
			new_direction = 'L';
		}
		
		
		if(distance_current > 300)
		{
			timeToDisplay = 300 * 5;
		}
		else
		{
			timeToDisplay = distance_current * 5;
		}
		
		if(new_direction == 'R')
		{
			//printf("Moving Away... Move Right\n");
			sequenceBuffer[0] = 0;
			sequenceBuffer[1] = timeToDisplay;
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);

			sequenceBuffer[0] = 1;
			sequenceBuffer[1] = timeToDisplay;
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);
		}
		else if(new_direction == 'L')
		{
			//printf("Moving Closer... Move Left\n");
			sequenceBuffer[0] = 2;
			sequenceBuffer[1] = timeToDisplay;
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);
			
			sequenceBuffer[0] = 3;
			sequenceBuffer[1] = timeToDisplay;
			sequenceBuffer[2] = 0;
			sequenceBuffer[3] = 0;
			
			spi_led_write(fd, sequenceBuffer);
		}
		distance_previous = distance_current;
		old_direction = new_direction;
		usleep(10000);
	}
	//printf("thread_transmit_spi fd = %d\n",fd);
	close(fd);
	pthread_exit(0);
}














int main(int argc, char **argv, char **envp)
{
	int retValue, input;
	pthread_t thread_id_spi, thread_id_dist;
	ThreadParams *tp_spi, *tp_dist;
	
	printf("Enter \n1. To See dog controlled by sensor\n2. Number counter controlled by sensor\n3. Display distance on Sensor\n4. User Defined Pattern\n");
	scanf("%d",&input);
	
	/* Mutex Initialization*/
	pthread_mutex_init(&mutex, NULL);
	
	tp_spi = malloc(sizeof(ThreadParams));
	tp_spi -> threadId = 100;

	if (input == 2)
	{
		retValue = pthread_create(&thread_id_spi, NULL, &spi_number_transmit, (void*)tp_spi);
	}
	else if (input == 3)
	{
		retValue = pthread_create(&thread_id_spi, NULL, &distance_transmit_spi, (void*)tp_spi);
	}
	else if(input == 4)
	{
		retValue = pthread_create(&thread_id_spi, NULL, &thread_transmit_spi_user, (void*)tp_spi);
	}
	else
	{
		retValue = pthread_create(&thread_id_spi, NULL, &spi_dog_transmit, (void*)tp_spi);
	}
		
		
	if(retValue)
	{
		printf("ERROR; return code from pthread_create() is %d\n", retValue);
		exit(-1);
	}
	
	tp_dist = malloc(sizeof(ThreadParams));
	tp_dist -> threadId = 101;
	
	retValue = pthread_create(&thread_id_dist, NULL, &thread_Ultrasonic_distance, (void*)tp_dist);
	
	pthread_join(thread_id_spi, NULL);
	pthread_join(thread_id_dist, NULL);
	
	free(tp_spi);
	free(tp_dist);
	
	return 0;
} 


/***********************************************************************
* write_pulse - Function to send trigger pulse to sensor
* @fd: File Descriptor
*
* Returns negative errno, or else the number of success of trigger
* 
* Description: Function to send trigger pulse to sensor. It indirectly
* calls write method of the pulse.c to send a trigger pulse to sensor.
***********************************************************************/
int write_pulse(int fd)
{
	int retValue=0, count=0;
    char* writeBuffer;
	
	writeBuffer = (char *)malloc(10);
	while(1)
	{
		retValue = write(fd, writeBuffer, 10);
		if(retValue < 0)
		{
			//printf("Trigger Failure\n");
			//perror("PULSE Write ERROR is : ");
		}
		else
		{
			//printf("Trigger Successful\n");
			break;
		}
		usleep(100000);
	}
	free(writeBuffer);
	return retValue;
}

/***********************************************************************
* read_pulse - Function to read pulsewidth measured from sensor.
* @fd: File Descriptor
*
* Returns negative errno, or else the number of success of read.
* 
* Description: Function to read pulsewidth measured from sensor. This
* function makes a system call to read function of pulse.c
***********************************************************************/
int read_pulse(int fd)
{
	int retValue=0, count=0;
    unsigned int writeBuffer =0;
	while(1)
	{
		retValue = read(fd, &writeBuffer, sizeof(writeBuffer));
		
		if(retValue < 0)
		{
			//printf("Read Failure\n");
			//perror("PULSE Read ERROR is : ");
		}
		else
		{
			//printf("Read Successful\n");
			break;
		}
		usleep(100000);
	}
	return writeBuffer;
}


/***********************************************************************
* spi_led_write - Function to write a sequence of pattern onto LED.
* @fd: File Descriptor
* @sequenceBuffer: Order of pattern and time for each pattern to be 
* 					passed.
*
* Returns success or failure of the write.
* 
* Description: Function to write a sequence of pattern onto LED. This
* function makes a system call to write function of spi_led.c
***********************************************************************/
int spi_led_write(int fd, unsigned int sequenceBuffer[20])
{
	int retValue=0, count=0;
	while(1)
	{
		retValue = write(fd, sequenceBuffer, sizeof(sequenceBuffer));
		if(retValue < 0)
		{
			//printf("SPI LED Write Failure\n");
			//perror("SPI LED ERROR is : ");
		}
		else
		{
			//printf("SPI LED Write Successful\n");
			break;
		}
		usleep(100000);
	}
	return retValue;
}

/***********************************************************************
* spi_led_ioctl - Function to send pattern to buffer of driver.
* @fd: File Descriptor
* @patternBuffer: Entire pattern is passed to driver, so it can
* 					select the pattern and display based on write calls
* 					to driver.
*
* Returns success or failure of the ioctl.
* 
* Description: Function to send pattern to buffer of driver. This
* function makes a system call to ioctl function of spi_led.c
***********************************************************************/
int spi_led_ioctl(int fd, char patternBuffer[10][8])
{
	int retValue=0, count=0;
	while(1)
	{
		retValue = ioctl(fd, patternBuffer, sizeof(patternBuffer));
		if(retValue < 0)
		{
			printf("SPI LED IOCTL Failure\n");
		}
		else
		{
			//printf("spi_led_ioctl SPI LED IOCTL Successful\n");
			break;
		}
		usleep(100000);
	}
	return retValue;
}

