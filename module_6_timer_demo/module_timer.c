#include <linux/module.h>       /* for all modules                     */
#include <linux/init.h>         /* for entry/exit macros               */
#include <linux/kernel.h>       /* for printk priority macros          */
#include <linux/moduleparam.h>  /* for recieving paramters into module */
#include <linux/delay.h>
#include <linux/random.h>

#define NUM_STATIONS 5

static struct timer_list timer;
static int elapse = 5;

module_param(elapse,int,0);
MODULE_PARM_DESC(elapse, "Time duration in power saving mode");

static void wakeup_from_Power_managment(unsigned long no_need)
{
	pr_debug("Gilc: The associated client station #1 was awakend by timer \n");
	pr_debug("Gilc: Now station #1 checks for beacons sent from AP, if the TIM (Traffic Indication Map) was set\n");
	pr_debug("it should empty the buffer of packets that the AP kept for it. \n");

}

static int __init demo_init(void)
{
	unsigned int num;
	init_timer(&timer);
	pr_debug("Gilc: The moudle has been loaded (MODNAME = %s).\n",KBUILD_MODNAME);
	pr_debug("Gilc: Client station #1 is entering right now to Power Saving Mode \n");
	pr_debug("Gilc: Client station #1 informs the AP by sending packet with the Power Managment is turned on.\n");
	pr_debug("Gilc: Client station #1 sets timer for exiting Power Saving Mode in the future\n");
	get_random_bytes(&num,sizeof(num)); /* stores nbytes worth of random data in num */
	pr_debug("num = %d",num % NUM_STATIONS);

	timer.function = wakeup_from_Power_managment;
	timer.expires = jiffies + elapse * HZ;
	add_timer(&timer);
	

	return 0;
}


static void __exit demo_cleanup(void)
{

	int ret;
	ret = del_timer(&timer);
	if (ret)
		pr_debug("Gilc: The timer is still in use.");

	pr_debug("Gilc: module being unloaded.\n");
}


module_init(demo_init);     /* What's called upon loading   */
module_exit(demo_cleanup);  /* What's called upon unloading */

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
