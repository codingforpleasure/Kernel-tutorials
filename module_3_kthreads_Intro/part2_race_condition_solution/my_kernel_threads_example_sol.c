/* 
 * Copyright (C) 2013 Gil Cohen http://codingforpleasure.blogspot.com
 *
 * Tutorial subject : Kernels Threads  ( Lesson #3 - part 2)
 *
 * Tutorial purpose : Getting familiar with the Kernel threads API
 * 										
 * Sample usage : After I have clarified the problematic scenerio (race condition) which
 * 							  the varible glob_var suffers from (part 1).
 * 							  I have used in the code below  atomic operations which supported by the kernel.
 * 							  So now after compilation our prediction becomes true,
 * 							  indeed the final value of glob_var is 1000000.
 * 
 * For more thrilling and intresting subjects regarding linux, visit my blog.
 * Happy coding!
 */

#include <linux/module.h>   /* For all modules                 */
#include <linux/init.h>     /* For entry/exit macros           */
#include <linux/kernel.h>   /* For printk priority macros      */
#include <linux/delay.h>    /* For using mdeley                */
#include <linux/kthread.h>  /* For using the kthread_run       */  
#include <linux/sched.h>    /* For using the task_struct       */
#include <linux/jiffies.h>  /* For measuring time              */
#include <linux/types.h>    /* For declaring an atomic varible */


struct task_struct *ts1;
struct task_struct *ts2;
atomic_t glob_var;


int kthread_fct(void *data)
{
	
	unsigned int thread_number = (int)(data);
	unsigned long i;
	unsigned long time_stamp_before;

	printk(KERN_INFO "\n%s(): Thread number (%d) was invoked.\n",__func__,thread_number);
	
	time_stamp_before = jiffies;
	
	for (i = 0; i < 500000; i++) {
		
		atomic_inc(&glob_var);
		if ((atomic_read(&glob_var) % 10000) == 0) 
				printk(KERN_INFO "\nThread number(%d):Look at me!",thread_number);
	}

	printk(KERN_INFO "\nThread number (%d):The glob_res is: %u .\n",thread_number,atomic_read(&glob_var));
	printk(KERN_INFO "\nThread number (%d):Finished the loop. now I'm waiting for kthread_stop().\n",thread_number);
	
	while (!kthread_should_stop())
	{
		mdelay(1); 
		msleep(499);
	}
			
	printk(KERN_INFO "\nThread number (%d) has spent %lu seconds since was created.\n",thread_number,(jiffies-time_stamp_before)/HZ); /*In seconds*/

	return 0;
}

static int __init kthr_init(void)
{
	unsigned int thread_id;

	printk(KERN_WARNING "Gilc: The moudle has been loaded!\n");
	thread_id = 1;
	atomic_set(&glob_var,0);
	ts1 = kthread_run(kthread_fct,(void *)thread_id,"codingpleaure_1");
	printk(KERN_INFO "\ntask1: pid = %d, on cpu number: %d.",ts1->pid,ts1->on_cpu);
	thread_id++;
	ts2 = kthread_run(kthread_fct,(void *)thread_id,"codingpleaure_2");
	printk(KERN_INFO "\ntask2: pid = %d, on cpu number: %d.",ts2->pid,ts2->on_cpu);
	
	return 0;
}

static void __exit kthr_exit(void)
{
	kthread_stop(ts1);
	kthread_stop(ts2);
	printk(KERN_WARNING "Gilc: module being unloaded.\n");
}


module_init(kthr_init);      /* What's called upon loading */
module_exit(kthr_exit);      /* What's called upon unloading */

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
