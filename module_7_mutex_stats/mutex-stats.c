#include <linux/module.h>  /*for all modules*/
#include <linux/init.h>    /*for entry/exit macros*/
#include <linux/kernel.h>  /*for printk priority macros*/
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define MAX_THREADS 64

/* Parameters for the module */
static int num_threads = 1; 
static int elapse = 5;      /*elapse (run) time in seconds*/  
static int load_in_cs = 5;  /*default value of load in cs in us*/ 
static int do_sched = 0;      /*flag: schedule in NCS*/ 
static int do_sched_in_cs = 0;/*flag: schedule in CS*/ 
static int load_in_ncs = 5; /*default value of load in NCS in us*/ 
static int verbose = 0;     /*verbosity of output*/

static struct mutex mutex;
static struct timer_list timer;
//static int currentlyInsideCS;

static struct task_struct  *threadsCollection[MAX_THREADS];
static int thread_hold_mutex[MAX_THREADS];
static unsigned int histogram_Qlen[MAX_THREADS];
static atomic_t working = ATOMIC_INIT(0);

module_param_named(v, verbose, int,0);
MODULE_PARM_DESC(verbose, "Verbosity");

module_param_named(nthr, num_threads, int, 0);
MODULE_PARM_DESC(num_threads, "Number of threads");

module_param(elapse, int, 0);
MODULE_PARM_DESC(elapse, "Number of seconds to run for");

module_param(load_in_cs, int, 0);
MODULE_PARM_DESC(load_in_cs, "Length of load in uS in CS");

module_param(load_in_ncs, int, 0);
MODULE_PARM_DESC(load_in_ncs, "Length of interval in uS before re-getting lock (NCS)");

module_param(do_sched, int, 0);
MODULE_PARM_DESC(do_sched, "True if each thread schedules in NCS");

module_param(do_sched_in_cs, int, 0);
MODULE_PARM_DESC(do_sched_in_cs, "True if each thread schedules in CS");


static void stop_running_threads(unsigned long junk)
{
	pr_debug("\nstop running threads arrived.\n");
	atomic_set(&working,0);
}

static void show_parameters(void)
{
	pr_debug("\nModule mutex-test: parameters\n"
	"# of online CPUs      :%d\n"
	"# of threads          :%d\n"
	"scheduling in NCS     :%s\n"
	"scheduling in CS      :%s\n"
	"load in CS in usec    :%d\n"
	"load in NCS in usec   :%d\n"
	"elapsed time in sec   :%d\n",
	num_online_cpus(), num_threads,
	do_sched ? "yes" : "no", do_sched_in_cs ? "yes" : "no",
	load_in_cs, load_in_ncs, elapse);
}

static unsigned int getQlen(void)
{
	unsigned int Qlen = 0;
	struct list_head *ptr;

//	pr_debug("\nThread has arrived getQlen.\n");
//	spinlock
	list_for_each(ptr,&mutex.wait_list)
			Qlen++;

	return Qlen;
}

static int thread_func(void * data)
{
		unsigned long thread_number = (unsigned long )data;

		pr_debug("\nThread #%lu has arrived.\n",thread_number);
		while (!kthread_should_stop())
		{
			mutex_lock(&mutex);
			thread_hold_mutex[thread_number]++;
			histogram_Qlen[getQlen()]++;
			mutex_unlock(&mutex);
		}
		pr_debug("\nThread #%lu has stopped.\n",thread_number);
		return 0;
}

static void print_statistics(void)
{
	unsigned long loop;	
	unsigned long total = 0;
	unsigned int ratio;
	pr_debug("\n****** statistics billboard ******\n");

	for (loop = 0; loop < MAX_THREADS ;loop++)
	{
		total+= thread_hold_mutex[loop]; 
	}

	for (loop = 0; loop < MAX_THREADS ;loop++)
	{
		if (thread_hold_mutex[loop] != 0)
		{
			ratio = thread_hold_mutex[loop] * 100 /total;
			pr_debug("\n Thread #%lu has caught the mutex %d times (%d percent)\n",loop,thread_hold_mutex[loop],ratio) ;
		}
		if (histogram_Qlen[loop]!=0)
				pr_debug("\n Queue len %d appeared %d times\n",loop,histogram_Qlen[loop]) ;
	}


}
static int __init hello_init(void)
{
	unsigned long loop;
	verbose =1;
	pr_debug("Gilc: The moudle has been loaded (Module name: %s).\n",KBUILD_MODNAME);
	
	for (loop = 0; loop < MAX_THREADS ;loop++)
	{
		thread_hold_mutex[loop] = 0;
		histogram_Qlen[loop] = 0;
	}

	pr_debug("\nGilc: Conducting sanity check for the parameters\n");

	if (num_threads < 0 || num_threads > MAX_THREADS || elapse < 1 || load_in_cs < 0 || load_in_cs > 999 ||
	load_in_ncs < 0 || load_in_ncs > 999) {
	printk("Parameter out of range\n");
	return -ERANGE;
	}

	show_parameters();

/*	printk("random value test:");
	for (loop = 0; loop < 10; loop++) {
		printk(" %d",random_val(load_in_cs));
	}
	printk("\n");
*/

	pr_debug("\nStarting synchronization primitive tests!\n");
	mutex_init(&mutex);
	init_timer(&timer);
	timer.function = stop_running_threads;
	timer.expires = jiffies + elapse*HZ;
	add_timer(&timer);
	atomic_set(&working,1);

	for (loop = 0; loop < num_threads ; loop++)
	{
		threadsCollection[loop] = kthread_run(thread_func,(void *)loop,"thread %lu",loop);
	}

	while (atomic_read(&working))
	{
		if (verbose)
				printk("\nTimer is running\n");
	
		msleep(500);
	}

	if (verbose)
		pr_debug("\nStopping kthreads\n");

	for (loop = 0; loop < num_threads ;  loop++)
	{

		kthread_stop(threadsCollection[loop]);
	}

	print_statistics();
	pr_debug("\n Exiting...\n");
	return 0;
}


static void __exit hello_cleanup(void)
{
	pr_debug("Gilc: module being unloaded.\n");
}

module_init(hello_init); /*What's called upon loading*/
module_exit(hello_cleanup);  /*What's called upon unloading*/

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
