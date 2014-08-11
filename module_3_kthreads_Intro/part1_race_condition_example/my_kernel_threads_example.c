/* 
 * Copyright (C) 2013 Gil Cohen http://codingforpleasure.blogspot.com
 *
 * Tutorial subject : Kernels Threads  ( Lesson #3 )
 *
 * Tutorial purpose : Getting familiar with the Kernel threads API
 * 										
 * Sample usage: This example which I wrote, demonstrates the need for 
 * 							 synchronization methods for avoiding race condition.
 * 							 As we may know the linux kernel is preemptible, so the kernel thread
 * 							 can lose the CPU at any time.
 * 							 As you can see below, the actual shared resource is glob_var.
 * 							 the thread function kthread_fct() is executed by the two threads
 * 							 each thread increments the the glob_var in a loop.
 * 							 but since the incrementations is not atomic (three instructions: load -> increment -> store)
 * 							 there is a quite good chance that the cpu would be taken in the middle
 * 							 of the increment operation.
 * 							 
 * 							 We can predict after the two threads have finished runnig,
 * 							 the final value of glob_var would be 1000000, but you can see for yourself
 * 							 the value varies on each execution.
 * 							 
 * 							 look for the actual solution of this problemtic scenerio the on other kernel modules I wrote.
 * 
 * For more thrilling and intresting subjects regarding linux, visit my blog.
 * Happy coding!
 */

#include <linux/module.h>   /* For all modules            */
#include <linux/init.h>     /* For entry/exit macros      */
#include <linux/kernel.h>   /* For printk priority macros */
#include <linux/delay.h>    /* For using mdeley           */
#include <linux/kthread.h>  /* For using the kthread_run  */  
#include <linux/sched.h>    /* For using the task_struct  */
#include <linux/jiffies.h>  /* For measuring time         */

struct task_struct *ts1;
struct task_struct *ts2;
unsigned long glob_var;

int kthread_fct(void *data)
{
	
	unsigned int thread_number = (int)(data);
	unsigned long i;
	unsigned long time_stamp_before;

	printk(KERN_INFO "\n%s(): Thread number (%d) was invoked.\n",__func__,thread_number);
	
	time_stamp_before = jiffies;
	
	for (i = 0; i < 500000; i++) {

		glob_var++;
		if ((glob_var % 10000) == 0) 
				printk(KERN_INFO "\nThread number(%d):Look at me!",thread_number);
	}

	printk(KERN_INFO "\nThread number (%d):he glob_res is: %lu.\n",thread_number,glob_var);
	printk(KERN_INFO "\nThread number (%d):Finished the loop. now I'm waiting for kthread_stop()\n",thread_number);
	
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
	thread_id = 1;
	glob_var = 0;
	ts1 = kthread_run(kthread_fct,(void *)thread_id,"codingpleaure_1");
	printk(KERN_INFO "\ntask1: pid = %d, on cpu number: %d.",ts1->pid,ts1->on_cpu);
	thread_id++;
	ts2 = kthread_run(kthread_fct,(void *)thread_id,"codingpleaure_2");
	printk(KERN_INFO "\ntask2: pid = %d, on cpu number: %d.",ts2->pid,ts2->on_cpu);
	printk(KERN_WARNING "Gilc: The moudle has been loaded!\n");
	return 0;
}

static void __exit kthr_exit(void)
{
	printk(KERN_WARNING "Gilc: module being unloaded.\n");
	kthread_stop(ts1);
	kthread_stop(ts2);
}

module_init(kthr_init);      /* What's called upon loading */
module_exit(kthr_exit);      /* What's called upon unloading */

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
