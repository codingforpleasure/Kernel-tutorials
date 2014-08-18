/* 
 * Copyright (C) 2013 Gil Cohen  -  http://codingforpleasure.blogspot.com
 *
 * Tutorial subject : Netfilter Basics ( Lesson #1 )
 *
 * Tutorial purpose : A closer look on Linux's kernel netfilter. 
 * 										I give here a simple & short example how to register a hook in the filter table.
 * 										I have retrieved the ip header from each packet, and have checked if 
 * 										It belongs to ICMP protocol, if so the action which would be taken is to drop the packet.
 *
 * Sample usage: After compiling the source file,
 * 							 insert the module (kernel object file) to the your kernel:
 * 							 sudo insmod <>.ko
 *
 *  						 You can easily notice the messages via dmesg.
 *
 *  						 Try to ping to any website, those packets get discarded since in this kernel
 *  						 module I have  blocked the icmp traffic. moreover you can see it quite easily via iptables app 
 *  						 iptables is a user space app which communicate with netfilter subsystem, and displays in a friendly
 *  						 matter the and the packet statistics. use the command: iptables -L -v
 *
 *  						 At the end of demonstration you should remove the module from the kernel via:
 *  						 sudo rmmod <>.ko , now the ICMP won't get blocked.
 * 
 * For more thrilling and intresting subjects regarding linux, visit my blog.
 * Happy coding!
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netfilter.h>       /* For netfilter - netfilter hooks operations*/
#include <linux/netfilter_ipv4.h> 
#include <linux/ip.h>              /* To access ip-header here */
#include <linux/inetdevice.h>      /* For printing the device name*/


static struct nf_hook_ops nfho;    /* struct holding set of hook function options */

/* My function to be called by hook */
unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
	struct iphdr *ip = NULL;
	struct tcphdr *tcp = NULL;
	char src_addr[16],dst_addr[16];


	ip = (struct iphdr * ) skb_network_header(skb);
	tcp = (struct tcphdr * ) skb_transport_header(skb + 20); /* The minimum IP header length is 20 Bytes */
	
	snprintf(src_addr,16,"%pI4",&ip->saddr);
	snprintf(dst_addr,16,"%pI4",&ip->daddr);

	if ((ip->protocol == IPPROTO_ICMP) || (ip->protocol = IPPROTO_ICMPV6))
	{
		printk(KERN_INFO "\nGilc: packet (src: %s, dst:%s) on inteface: %s \n",src_addr,dst_addr,in->name); 
		return NF_DROP;
	}
}

static int __init init_my_module(void)
{
	printk(KERN_INFO "Gilc: Init module! \n"); 
	nfho.hook = hook_func;                       /* function to call when conditions below met                  */
	nfho.hooknum = NF_INET_PRE_ROUTING;          /* called right after packet recieved, first hook in Netfilter */
	nfho.pf = PF_INET;                           /* IPV4 packets                                                */
	nfho.priority = NF_IP_PRI_FIRST;             /* set to highest priority over all other hook functions       */
	nf_register_hook(&nfho);                     /* register hook                                               */
	return 0;                                   
}

static void __exit clean_my_module(void)
{
	printk(KERN_INFO "\nExiting the module \n");
	nf_unregister_hook(&nfho);
}

module_init(init_my_module);   /* What's called upon loading   */
module_exit(clean_my_module);  /* What's called upon unloading */

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");
