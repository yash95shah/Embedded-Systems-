#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <asm/errno.h>
#include <linux/math64.h>
#define DRIVER_NAME 		"usensor"
#define DEVICE_NAME 		"usensor"
#define TRIGGER_IO 13
#define ECHO_IO 14
#define TRIGGER_IO_MUX 77
#define ECHO_IO_MUX 76
#define L_GPIO 0
#define H_GPIO 1
#define Det_R 0
#define Det_F 1
static dev_t dev_p_number;      /* Allotted Device Number */
static struct class *pulse_class;   /* Device class */
static unsigned char Edge = Det_R;


typedef struct Cdev_dev_p
{
	struct cdev cdev;               /* The cdev structure */
	char name[20];                  /* Name of device */
	unsigned int BUSY;		  	/* Busy Flag Status */
	unsigned long long Rise_Time;		/* TimeStamp to record Start Time */
	unsigned long long Fall_Time;		/* TimeStamp to record End Time */
	int irq;
} dev_pulse;

dev_pulse *dev_p;


static __inline__ unsigned long long rdtsc(void)
{
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long) lo) | ((unsigned long long) hi)<<32;
}

static irqreturn_t change_state_interrupt(int irq, void *dev_id)
{
	//printk("pulse.c change_state_interrupt Start\n");
	if(Edge==Det_R)
	{
		dev_p->Rise_Time = rdtsc();
	    irq_set_irq_type(irq, IRQF_TRIGGER_FALLING);
	    Edge=Det_F;
	}
	else
	{
		dev_p->Fall_Time = rdtsc();
	    irq_set_irq_type(irq, IRQF_TRIGGER_RISING);
	    Edge=Det_R;
		dev_p->BUSY = 0;
	}
	//printk("pulse.c change_state_interrupt End\n");
	return IRQ_HANDLED;
}


int pulse_open(struct inode *inode, struct file *filp)
{
	int irq_line;
	int irq_req_res_rising;
	int retValue;

	//printk("pulse.c pulse_open() Start \n");
	dev_p->BUSY = 0;

	/* Get the per-device structure that contains this cdev */
	dev_p = container_of(inode->i_cdev, dev_pulse, cdev);

	/* Easy access to cmos_devp from rest of the entry points */
	filp->private_data = dev_p;

	//Free the GPIO Pins
	gpio_free(TRIGGER_IO);
	gpio_free(ECHO_IO);
	gpio_free(TRIGGER_IO_MUX);
	gpio_free(ECHO_IO_MUX);

	//Set GPIO pins directions and values
	gpio_request_one(TRIGGER_IO_MUX, GPIOF_OUT_INIT_LOW , "gpio77");
	gpio_request_one(ECHO_IO_MUX, GPIOF_OUT_INIT_LOW , "gpio76");
	gpio_request_one(TRIGGER_IO, GPIOF_OUT_INIT_LOW , "gpio13");
	gpio_request_one(ECHO_IO, GPIOF_OUT_INIT_LOW , "gpio14");

	//Set GPIO pins values
	gpio_set_value_cansleep(TRIGGER_IO, L_GPIO);
	gpio_set_value_cansleep(ECHO_IO, L_GPIO);
	gpio_set_value_cansleep(TRIGGER_IO_MUX, L_GPIO);
	gpio_set_value_cansleep(ECHO_IO_MUX, L_GPIO);

	//printk("%s has opened\n", dev_p->name);
	gpio_free(ECHO_IO);
	gpio_request_one(ECHO_IO, GPIOF_IN , "gpio14");

	/*install interrupt handler*/
	irq_line = gpio_to_irq(ECHO_IO);

	if(irq_line < 0)
	{
		printk("Gpio %d cannot be used as interrupt",ECHO_IO);
		retValue=-EINVAL;
	}
	dev_p->irq = irq_line;

	dev_p->Rise_Time=0;
	dev_p->Fall_Time=0;

	irq_req_res_rising = request_irq(irq_line, change_state_interrupt, IRQF_TRIGGER_RISING, "gpio_change_state", dev_p);
	if(irq_req_res_rising)
	{
		printk("Unable to claim irq %d; error %d\n ", irq_line, irq_req_res_rising);
		return 0;
	}

	//printk("pulse.c pulse_open() End \n");
	return 0;
}

int pulse_release(struct inode *inode, struct file *filp)
{
	dev_pulse *local_dev_p;
	//printk("pulse.c pulse_release() Start\n");

	dev_p->BUSY = 0;
	local_dev_p = filp->private_data;
	free_irq(dev_p->irq,dev_p);

	gpio_free(TRIGGER_IO);
	gpio_free(ECHO_IO);
	gpio_free(TRIGGER_IO_MUX);
	gpio_free(ECHO_IO_MUX);

	printk("pulse_release -- %s is closing\n", local_dev_p->name);
	//printk("pulse.c pulse_release() End\n");
	return 0;
}


static ssize_t pulse_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	int retValue = 0;
	//printk("pulse.c pulse_write() Start\n");

	if(dev_p->BUSY == 1)
	{
		retValue = -EBUSY;
		return -EBUSY;
	}

	//Generate a trigger pulse
	gpio_set_value_cansleep(TRIGGER_IO, H_GPIO);
	udelay(18);
	gpio_set_value_cansleep(TRIGGER_IO, L_GPIO);
	dev_p->BUSY = 1;
	//printk("pulse.c pulse_write() End\n");
	return retValue;
}


static ssize_t pulse_read(struct file *file, char *buf, size_t count, loff_t *ptr)
{
	int retValue=0;
	unsigned int c;
	unsigned long long tempBuffer;
	//printk("pulse.c pulse_read() Start\n");
	if(dev_p->BUSY == 1)
	{
		return -EBUSY;
	}
	else
	{
		if(dev_p->Rise_Time == 0 && dev_p->Fall_Time == 0)
		{
			printk("Please Trigger the measure first\n");
		}
		else
		{
			tempBuffer = dev_p->Fall_Time - dev_p->Rise_Time;
			c = div_u64(tempBuffer,400);
			retValue = copy_to_user((void *)buf, (const void *)&c, sizeof(c));
		}
	}
	//printk("pulse.c pulse_read() End\n");
	return retValue;
}

/**
 * File operations structure. Defined in linux/fs.h
 */
static struct file_operations pulse_fops =
{
		.owner = THIS_MODULE,			/* Owner */
		.open = pulse_open,             /* Open method */
		.release = pulse_release,       /* Release method */
		.write = pulse_write,           /* Write method */
		.read = pulse_read				/* Read method */
};


static int __init pulse_init(void)
{
	int retValue;
	//printk("pulse.c pulse_init() Start \n");

	/* Request dynamic allocation of a device major number */
	if(alloc_chrdev_region(&dev_p_number, 0, 0, DRIVER_NAME) < 0)
	{
		printk("Can't register device\n");
		return -1;
	}

	/* Populate sysfs entries */
	pulse_class = class_create(THIS_MODULE, DRIVER_NAME);

	/* Allocate memory for the per-device structure dev_p */
	dev_p = kmalloc(sizeof(dev_pulse), GFP_KERNEL);
	if(!dev_p)
	{
		printk("Bad Kmalloc dev_p\n");
		return -ENOMEM;
	}

	/* Request I/O Region */
	sprintf(dev_p->name, DRIVER_NAME);

	/* Connect the file operations with the cdev */
	cdev_init(&dev_p->cdev, &pulse_fops);
	dev_p->cdev.owner = THIS_MODULE;

	/* Connect the major/minor number to the cdev */
	retValue = cdev_add(&dev_p->cdev, MKDEV(MAJOR(dev_p_number), 0), 1);
	if(retValue)
	{
		printk("Bad cdev for dev_p\n");
		return retValue;
	}

	/* A struct device will be created in sysfs, registered to the specified class.*/
	device_create(pulse_class, NULL, MKDEV(MAJOR(dev_p_number), 0), NULL, DEVICE_NAME);

	printk("Pulse Driver = %s Initialized.\n", DRIVER_NAME);
	//printk("pulse.c pulse_init() Ends \n");
	return 0;
}

static void __exit pulse_exit(void)
{
	//printk("pulse_exit() Start\n");

	/* Destroy device with Minor Number 0*/
	device_destroy(pulse_class, MKDEV(MAJOR(dev_p_number), 0));
	cdev_del(&dev_p->cdev);
	kfree(dev_p);

	/* Destroy driver_class */
	class_destroy(pulse_class);

	/* Release the major number */
	unregister_chrdev_region(dev_p_number, 1);
	printk("Pulse Driver = %s Uninitialized.\n", DRIVER_NAME);
	//printk("pulse_exit() End\n");
}

module_init(pulse_init);
module_exit(pulse_exit);

MODULE_DESCRIPTION("PULSE Driver");
MODULE_LICENSE("GPL");
