#include <linux/module.h>  /*for all modules*/
#include <linux/init.h>    /*for entry/exit macros*/
#include <linux/kernel.h>    /*for printk priority macros*/
#include <linux/netfilter.h>  /**/
#include <linux/netfilter_ipv4.h> /**/


struct nf_hook_ops my_nf_hookups;


unsigned int my_hook_func(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *))
{

	printk(KERN_WARNING "\nGilc: you are in %s\n",__func__);
	return NF_DROP;
}

static int __init hello_init(void)
{
	printk(KERN_WARNING "Gilc: The moudle has been loaded!\n");
	my_nf_hookups.priority = NF_IP_PRI_FIRST;
	my_nf_hookups.hook = my_hook_func;
	my_nf_hookups.hooknum = NF_INET_PRE_ROUTING;	
	my_nf_hookups.pf = PF_INET;
	
	nf_register_hook(&my_nf_hookups);
	return 0;
}


static void __exit hello_cleanup(void)
{
	printk(KERN_WARNING "Gilc: module being unloaded.\n");
	nf_unregister_hook(&my_nf_hookups);
}


module_init(hello_init); /*What's called upon loading*/
module_exit(hello_cleanup);  /*What's called upon unloading*/

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");

