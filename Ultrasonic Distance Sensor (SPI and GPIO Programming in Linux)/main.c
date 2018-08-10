#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <pthread.h>

#define GPIO_IO2 13
#define GPIO_IO3 14
#define GPIO_Mux2 77
#define GPIO_Mux3 76
#define GPIO_DIRECTION_IN 1
#define GPIO_DIRECTION_OUT 0
#define Set_Low 0
#define Set_High 1
#define Buffer_Max 64
#define TRIGGER 10
#define TRIGGER_OFF 50
#define POLL_TIMEOUT 3000
#define CLK_SPEED 400000000.00
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0])
#define SPI_DEVICE_NAME "/dev/spidev 1.0"
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define GPIO_SPI1_CS_IO12 	15
#define GPIO_SPI1_MOSI_IO11 5
#define GPIO_SPI1_MISO_IO12 15
#define GPIO_SPI1_SCK_IO13 	7
#define GPIO11_LS 24
#define GPIO12_LS 42
#define GPIO13_LS 30
#define GPIO11_MX 44
#define GPIO13_MX 46

pthread_mutex_t mutex;
double distance;

typedef unsigned long u64;
typedef unsigned long u32;

static uint8_t mode=0;
static uint8_t bits=8;
static uint32_t speed=500000;
static uint16_t delay;

typedef struct
{
	int threadId;
	int fd;
}ThreadParams;

int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[Buffer_Max];

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[Buffer_Max];

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd, len;
	char buf[Buffer_Max];

	len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/direction");
		return fd;
	}

	if (out_flag == GPIO_DIRECTION_OUT)
		write(fd, "out", 4);
	else
		write(fd, "in", 3);

	close(fd);
	return 0;
}

int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, len;
	char buf[Buffer_Max];

	len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/set-value");
		return fd;
	}

	if(value == Set_High)
		write(fd, "1", 2);
	else
		write(fd, "0", 2);

	close(fd);
	return 0;
}

int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, len;
	char buf[Buffer_Max];
	char ch;

	len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
		perror("gpio/get-value");
		return fd;
	}

	read(fd, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}

	close(fd);
	return 0;
}


int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd, len;
	char buf[Buffer_Max];

	len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge)+1);
	close(fd);
	return 0;
}


int gpio_fd_open(unsigned int gpio)
{
	int fd, len;
	char buf[Buffer_Max];

	len = snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

int gpio_fd_close(int fd)
{
	return close(fd);
}

static inline u64 rdtsc(void)
{
	if (sizeof(long) == sizeof(u64))
	{
		u32 lo, hi;
		asm volatile("rdtsc" : "=a" (lo), "=d" (hi));
		return ((u64)(hi) << 32) | lo;
	}
	else
	{
		u64 tsc;
		asm volatile("rdtsc" : "=A" (tsc));
		return tsc;
	}
}

void LED_transfer_func(int fd, uint8_t address, uint8_t data)
{

	int i;
    static long rt;

	uint8_t tx[2];
	tx[0] = address;
	tx[1] = data;


struct spi_ioc_transfer tr = 
	{
		.tx_buf = (unsigned long) tx,
		.rx_buf = 0,
		.len = 500,
		.cs_change = 1,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
    
    rt= ioctl(i, SPI_IOC_MESSAGE(1), &tr);

	if(rt < 0)
	{
		printf("error in sending message\n");
	}
	}

	void LED_clear(int fd)
{
	char i;
	for(i=1; i < 9; i++)
	{
		LED_transfer_func(fd, i, 0x00);
	}
}

void LED_init(int fd)
{
	usleep(100000);
	// Enable mode B
    LED_transfer_func(fd, 0x09, 0x00);
	usleep(100000);
	// Define Intensity
	LED_transfer_func(fd, 0x0A, 0x04);
	usleep(100000);
	// Only scan 7 digit
	LED_transfer_func(fd, 0x0B, 0x07);
	usleep(100000);
	// Turn on chip
	LED_transfer_func(fd, 0x0C, 0x01);
	usleep(100000);
	LED_clear(fd);
}

void *thread_transmit_spi(void *data)
{
	int retValue,fd;
	ThreadParams *tparams = (ThreadParams*)data;
	double distance_previous = 0, distance_current = 0, distance_diff = 0, distance_threshhold=0;
	char new_direction = 'L', old_direction = 'L';

	fd = tparams->fd;

	//Export all GPIO Pins
	gpio_export(GPIO_SPI1_MOSI_IO11);
	gpio_export(GPIO_SPI1_CS_IO12);
	gpio_export(GPIO_SPI1_SCK_IO13);
	gpio_export(GPIO11_LS);
	gpio_export(GPIO11_MX);
	gpio_export(GPIO12_LS);
	gpio_export(GPIO13_LS);
	gpio_export(GPIO13_MX);


	//Set Directions for all GPIO Pins
	gpio_set_dir(GPIO_SPI1_MOSI_IO11, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO_SPI1_CS_IO12, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO_SPI1_SCK_IO13, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO11_LS, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO11_MX, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO12_LS, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO13_LS, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO13_MX, GPIO_DIRECTION_OUT);

	//Set Values for all GPIO Pins
	gpio_set_value(GPIO_SPI1_MOSI_IO11, Set_Low);
	gpio_set_value(GPIO_SPI1_CS_IO12, Set_Low);
	gpio_set_value(GPIO_SPI1_SCK_IO13, Set_Low);
	gpio_set_value(GPIO11_LS, Set_Low);
	gpio_set_value(GPIO11_MX, Set_Low);
	gpio_set_value(GPIO12_LS, Set_Low);
	gpio_set_value(GPIO13_LS, Set_Low);
	gpio_set_value(GPIO13_MX, Set_Low);

	//Open the Device for File Operations
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

	/*
	* spi mode
	*/
	retValue = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if(retValue == -1)
	{
		printf("can't set spi mode\n");
	}
	retValue = ioctl(fd, SPI_IOC_RD_MODE, &mode);

	if(retValue == -1)
	{
		printf("can't get spi mode\n");
	}

	/*
	* bits per word
	*/
	retValue = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (retValue == -1)
	{
		printf("can't set bits per word\n");
	}
	retValue = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (retValue == -1)
	{
		printf("can't get bits per word\n");
	}

	/*
	* max speed hz
	*/
	retValue = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (retValue == -1)
	{
		printf("can't set max speed hz\n");
	}
	retValue = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (retValue == -1)
	{
		printf("can't get max speed hz\n");
	}


	LED_init(fd);

	usleep(100000);
	while(1)
	{
		//sleep(1);
		pthread_mutex_lock(&mutex);
		distance_current = distance;
		pthread_mutex_unlock(&mutex);
		distance_diff = distance_current - distance_previous;
		distance_threshhold = distance_current / 10.0;
		printf("Distance = %0.2f\n",distance_current);

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

		if(new_direction == 'R')
		{
			//printf("Moving Away... Move Right\n");
			LED_transfer_func(fd, 0x01, 0x08);
			LED_transfer_func(fd, 0x02, 0x90);
			LED_transfer_func(fd, 0x03, 0xf0);
			LED_transfer_func(fd, 0x04, 0x10);
			LED_transfer_func(fd, 0x05, 0x10);
			LED_transfer_func(fd, 0x06, 0x37);
			LED_transfer_func(fd, 0x07, 0xdf);
			LED_transfer_func(fd, 0x08, 0x98);
			//usleep(300000);
			if(distance_current < 200)
			{
				usleep(distance_current * 5000);
			}
			else
			{
				usleep(500 * 5000);
			}
			LED_transfer_func(fd, 0x01, 0x20);
			LED_transfer_func(fd, 0x02, 0x10);
			LED_transfer_func(fd, 0x03, 0x70);
			LED_transfer_func(fd, 0x04, 0xd0);
			LED_transfer_func(fd, 0x05, 0x10);
			LED_transfer_func(fd, 0x06, 0x97);
			LED_transfer_func(fd, 0x07, 0xff);
			LED_transfer_func(fd, 0x08, 0x18);

			//usleep(300000);
			if(distance_current < 200)
			{
				usleep(distance_current * 5000);
			}
			else
			{
				usleep(500 * 5000);
			}
		}
		else if(new_direction == 'L')
		{
			//printf("Moving Closer... Move Left\n");
			LED_transfer_func(fd, 0x01, 0x98);
			LED_transfer_func(fd, 0x02, 0xdf);
			LED_transfer_func(fd, 0x03, 0x37);
			LED_transfer_func(fd, 0x04, 0x10);
			LED_transfer_func(fd, 0x05, 0x10);
			LED_transfer_func(fd, 0x06, 0xf0);
			LED_transfer_func(fd, 0x07, 0x90);
			LED_transfer_func(fd, 0x08, 0x08);
			//usleep(300000);
			if(distance_current < 200)
			{
				usleep(distance_current * 5000);
			}
			else
			{
				usleep(500 * 5000);
			}
			LED_transfer_func(fd, 0x01, 0x18);
			LED_transfer_func(fd, 0x02, 0xff);
			LED_transfer_func(fd, 0x03, 0x97);
			LED_transfer_func(fd, 0x04, 0x10);
			LED_transfer_func(fd, 0x05, 0xd0);
			LED_transfer_func(fd, 0x06, 0x70);
			LED_transfer_func(fd, 0x07, 0x10);
			LED_transfer_func(fd, 0x08, 0x20);

			//usleep(300000);
			if(distance_current < 200)
			{
				usleep(distance_current * 5000);
			}
			else
			{
				usleep(500 * 5000);
			}
		}

		distance_previous = distance_current;
		old_direction = new_direction;
	}
	close(fd);
	//printf("thread_transmit_spi End\n");
	pthread_exit(0);
}



void *Ultrasonic_Distance(void *data)
{
    int retval,k;
    unsigned long long risetime, falltime;
    char *buf[Buffer_Max];
    struct pollfd poll_io3;

    gpio_unexport(GPIO_IO2);
	gpio_unexport(GPIO_IO3);
	gpio_unexport(GPIO_Mux2);
	gpio_unexport(GPIO_Mux3);

	gpio_export(GPIO_IO2);
	gpio_export(GPIO_IO3);
	gpio_export(GPIO_Mux2);
	gpio_export(GPIO_Mux3);

	gpio_set_dir(GPIO_IO2, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO_IO3, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO_Mux2, GPIO_DIRECTION_OUT);
	gpio_set_dir(GPIO_Mux3, GPIO_DIRECTION_OUT);

	gpio_set_value(GPIO_IO2,Set_Low);
	gpio_set_value(GPIO_IO3,Set_Low);
	gpio_set_value(GPIO_Mux2,Set_Low);
	gpio_set_value(GPIO_Mux3,Set_Low);

	gpio_set_dir(GPIO_IO3, GPIO_DIRECTION_IN);

	poll_io3.fd = gpio_fd_open(GPIO_IO3);
	poll_io3.events = POLLPRI|POLLERR;

   while(1)
{


	lseek(poll_io3.fd,0,SEEK_SET);
	gpio_set_edge(GPIO_IO3,"rising");
	gpio_set_value(GPIO_IO2,Set_Low);
	usleep(TRIGGER);
	gpio_set_value(GPIO_IO2,Set_High);
	usleep(TRIGGER_OFF);
	gpio_set_value(GPIO_IO2,Set_Low);
	usleep(TRIGGER);


	k=poll(&poll_io3,1,POLL_TIMEOUT);
	if(k<0)
    {

        printf("The polling failed \n");
    }
    else if(k==0)
    {

        printf("The polling timed out \n");

    }
    else
    {

        if(poll_io3.revents & POLLPRI)
        {
            risetime=rdtsc();
            retval=read(poll_io3.fd,buf,1);
            if (retval>0)
            {
            }
            else
            {
                printf("Unable to detect rise time \n");
            }

        }
    }

    lseek(poll_io3.fd,0,SEEK_SET);
	gpio_set_edge(GPIO_IO3,"falling");
	gpio_set_value(GPIO_IO2,Set_Low);
	usleep(TRIGGER);
	gpio_set_value(GPIO_IO2,Set_High);
	usleep(TRIGGER_OFF);
	gpio_set_value(GPIO_IO2,Set_Low);
	usleep(TRIGGER);


	k=poll(&poll_io3,1,POLL_TIMEOUT);
	if(k<0)
    {

        printf("The polling failed \n");
    }
    else if(k==0)
    {

        printf("The polling timed out \n");

    }
    else
    {

        if(poll_io3.revents & POLLPRI)
        {
            falltime=rdtsc();
            retval=read(poll_io3.fd,buf,1);
        }
            if (retval>0)
            {
            }
            else
            {
                printf("Unable to detect falling time \n");
            }

            }

        usleep(30000);


pthread_mutex_lock(&mutex);

distance=((falltime-risetime)*340.00)/(2.0* CLK_SPEED);
pthread_mutex_unlock(&mutex);
}

gpio_unexport(GPIO_IO2);
gpio_unexport(GPIO_IO3);
gpio_unexport(GPIO_Mux2);
gpio_unexport(GPIO_Mux3);

pthread_exit (0);

}

int main(int argc, char **argv, char **envp)
{
	int return_value;
	pthread_t thread_id_spi, thread_id_dist;
	ThreadParams *tp_spi, *tp_dist;

	/* Mutex Initialization*/
	pthread_mutex_init(&mutex, NULL);

	tp_spi = malloc(sizeof(ThreadParams));
	tp_spi -> threadId = 100;
	//tp_spi -> fd = fd_spi;
	return_value = pthread_create(&thread_id_spi, NULL, &thread_transmit_spi, (void*)tp_spi);
	if(return_value)
	{
		printf("ERROR; return code from pthread_create() is %d\n", return_value);
		exit(-1);
	}

	tp_dist = malloc(sizeof(ThreadParams));
	tp_dist -> threadId = 101;
	return_value = pthread_create(&thread_id_dist, NULL, &Ultrasonic_Distance, (void*)tp_dist);

	pthread_join(thread_id_spi, NULL);
	pthread_join(thread_id_dist, NULL);

	//close(fd_spi);
	return 0;
}



