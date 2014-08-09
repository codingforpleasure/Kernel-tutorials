/* 
 * Copyright (C) 2013 Gil Cohen http://codingforpleasure.blogspot.com
 *
 * Tutorial subject : Linked list in the Kernel ( Lesson #2)
 *
 * Tutorial purpose : For Getting familiar with the built-in API of the data-structure klist (linked-list)
 *
 * Sample usage: For demostarting issues after you have inserted the module into the kernel. 
 *               check in the dmesg, that the a list was indeed created via the invoked subroutine: birthdayList_init
 *
 * 
 * For more thrilling and intresting subjects regarding linux, visit my blog.
 * Happy coding!
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

#define NUM_OF_PERSON    5

/* Example struct we will use. */
struct birthday_node {
    int day;
    int month;
    int year;
    struct list_head list;
};

/* Declare and init the head of the linked list. */
struct birthday_node birthday_list_head = {
	.day = -1,
	.month = -1,
	.year = -1,
};

/* This function is called when the module is loaded. */
int birthdayList_init(void)
{
    struct birthday_node *new_person;
    struct birthday_node *ptr;

    int i = 0;
    printk(KERN_WARNING "Loading Module\n");

		INIT_LIST_HEAD(&birthday_list_head.list);

    /* Allocate 5 birthdays from kernel */

    for(i = 0; i < NUM_OF_PERSON; i++)
    {
        /* Request malloc to the kernel. */
        new_person = (struct birthday_node * )kmalloc(sizeof(*new_person), GFP_KERNEL);

        /* Assign value to the struct. */
        new_person->day = i+1;
        new_person->month = i+2;
        new_person->year = 2013;
        /* Add this struct to the tail of the list. */
        list_add_tail(&new_person->list, &birthday_list_head.list);
    }

    printk(KERN_WARNING "So let's display the list in reverse order:\n");

    /* I'm here traversing thru the list in reverse order and printing the data. */
    
		list_for_each_entry_reverse(ptr, &birthday_list_head.list, list)
    {
     printk(KERN_WARNING "day: %d, month: %d, year: %d\n",
                ptr->day,
                ptr->month,
                ptr->year);
    }

    printk(KERN_WARNING "Display done\n");

    return 0;
}

/* This function is called when the module is removed. */
void birthdayList_exit(void)
{
    struct birthday_node *ptr, *next;

    /* Go thru the list and free the memory. */

    list_for_each_entry_safe(ptr, next, &birthday_list_head.list, list)
    {
        printk(KERN_WARNING "Removing - day: %d, month: %d, year: %d\n",
                ptr->day,
                ptr->month,
                ptr->year);
        list_del(&ptr->list);
        kfree(ptr);
    }
		printk(KERN_WARNING "Memory free done\n");
		printk(KERN_WARNING "Module has been removed.\n");

}

/* Macros for registering module entry and exit points. */
module_init( birthdayList_init );
module_exit( birthdayList_exit );

MODULE_AUTHOR("Gil Cohen");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("You have to start somewhere.");

