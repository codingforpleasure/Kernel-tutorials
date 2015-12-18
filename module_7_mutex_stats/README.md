I definitely like to explore and delve into intriguing subjects.
I'll explain soon the module which I wrote.

The mutex is a simple mechanisem for making sure who holds the lock gets access to the critical section,
all other who try to acquire the lock are sent to the waiting list.
The moment the lock is released by the owner the next thread in the waiting list is given the lock and now he becomes the owner.

The Kernel implements it via spinlock and a list of waiters, the code below is from mutex.h:

>struct mutex {
	atomic_t		count;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
	struct task_struct	*owner;
};


>struct mutex_waiter {
	struct list_head	list;
	struct task_struct	*task;
};"
