/* 
 * Copyright (C) 2013 Gil Cohen http://codingforpleasure.blogspot.com
 *
 * Tutorial subject : Writing a basic character device driver ( Lesson #4 )
 *
 * Tutorial purpose : For experincing and delving into a character written from scratch.
 * 										moreover registering & and associating your functions to the device
 * 										operations such as (open/release/read/write).
 *
 * Sample usage			: After compiling and inserting the module, you should compile
 * 										the application which runs on userspace. the application
 * 										would invoke the corresponding operations which we registred
 * 										during the initialization of the module.
 * 
 * 
 * For more thrilling and intresting subjects regarding linux, visit my blog.
 * Happy coding!
 */

#include <linux/module.h>    /* for all modules                                                     */
#include <linux/init.h>      /* for entry/exit macros                                               */
#include <linux/kernel.h>    /* for printk priority macros*/
#include <linux/fs.h>        /* file operation structure - which of course allows use to open/close 
                                                         read/write to device                       */
#include <linux/cdev.h>      /* This is a char driver: makes cdev avaliable                         */
#include <linux/semaphore.h> /* For synchronization                                                 */ 
#include <asm/uaccess.h>     /* For using the functions: copy_to_user/copy_from_user                */

#define BUF_SIZE 100
/* (1) Create a structure for our fake device */
struct fake_device{
	char data[BUF_SIZE];
	struct semaphore sem;
} virtual_device;

/* (2) To later register our device we need a cdev object and some other*/
struct cdev *my_cdev;
int major_number;
int ret;
dev_t dev_num;

#define DEVICE_NAME "Gil_device"


int device_open(struct inode *inode, struct file *filp)
{
	if (down_interruptible(&virtual_device.sem) != 0)
	{
		printk(KERN_WARNING "Gilc: Could not lock device during open\n");
		return -1;
	}
		printk(KERN_WARNING "Gilc: Open device\n");
		return 0;
}

/*
 * While trying to follow a pointer from kernel space into user space, you should make sure:
 * A) The pointer points to a region of memory in user-space.
 * B) The pointer points to a region of memory in the process's address space.
 * C) The process must not be able to bypass memory access restriction.
 *  
 * As said before since kernel never blindly follow a pointer into user-space
 * we use two basics kernel functions: copy_to_user(), copy_from_user().
 * in those functions you first check if the pointer is a valid pointer
 * who actually points to a userspace region via the function access_ok()
 */
ssize_t device_read(struct file * filp, char * bufStoreData, size_t bufCount, loff_t * curOffset)
{
		printk(KERN_WARNING "Gilc: reading from  device\n");
		ret = copy_to_user(bufStoreData,virtual_device.data, bufCount);
		return ret;
}


ssize_t device_write(struct file * filp, const char * bufSourceData, size_t bufCount, loff_t * curOffset)
{
		printk(KERN_WARNING "Gilc: writing into device\n");
		ret = copy_from_user(virtual_device.data,bufSourceData, bufCount);
		return ret;
}

int device_close(struct inode * node, struct file *filp)
{
	up(&virtual_device.sem);
	printk(KERN_WARNING "Gilc: closed device\n");
	return 0;
}

	/*assigning callbacks operations*/
struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,  
	.write = device_write,   /* Points to the method to call when writing into the device */
	.read = device_read      /* Points to the method to call when reading from the device */
};


static int __init driver_entry_init(void)
{
	printk(KERN_WARNING "Gilc: The moudle has been loaded!\n");
	ret = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
	if (ret < 0){
		printk(KERN_WARNING "Gilc: Failed to allocate a major number\n");
		return ret;
	}
	major_number = MAJOR(dev_num);
	printk(KERN_WARNING "Gilc: major number is %d", major_number);
	printk(KERN_WARNING "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME,major_number);

	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev,dev_num,1);
	if (ret < 0){
		printk(KERN_WARNING "Gilc: unable to add cdev to kernel\n");
		return ret;
	}

	sema_init(&virtual_device.sem , 1);

	return 0;
}


static void __exit driver_entry_cleanup(void)
{
	cdev_del(my_cdev);
	unregister_chrdev_region(dev_num,1);
	printk(KERN_WARNING "Gilc: module being unloaded.\n");
}


module_init(driver_entry_init); /*What's called upon loading*/
module_exit(driver_entry_cleanup);  /*What's called upon unloading*/

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
