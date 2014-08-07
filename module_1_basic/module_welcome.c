#include <linux/module.h>  /*for all modules*/
#include <linux/init.h>    /*for entry/exit macros*/
#include <linux/kernel.h>    /*for printk priority macros*/


static int __init hello_init(void)
{
	printk(KERN_WARNING "Gilc: The moudle has been loaded (MODNAME = %s).\n",KBUILD_MODNAME);
	return 0;
}


static void __exit hello_cleanup(void)
{
	printk(KERN_WARNING "Gilc: module being unloaded.\n");
}


module_init(hello_init); /*What's called upon loading*/
module_exit(hello_cleanup);  /*What's called upon unloading*/

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
