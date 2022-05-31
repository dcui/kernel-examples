// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Demo usage of atomic for a counter
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/sched/signal.h>

static int __init list_processes_init(void)
{
	struct task_struct *task, *self;
	struct list_head *begin, *list;

	rcu_read_lock();

	self = current;
	pr_info("task pid =%d, comm=%s\n", self->pid, self->comm);

	begin = &self->tasks;
	
	/*
	 * Note: container_of() works because there is not a special dummy
	 * "head node" in the doubly-linked list: check init/init_task.c and
	 * see how the 'tasks' field of "struct task_struct init_task" is
	 * initialized!
	 */
	for (list = begin->next; list != begin; list = list->next) {
		task = container_of(list, struct task_struct, tasks);
		
		task_lock(task);

		pr_info("task pid =%d, comm=%s\n", task->pid, task->comm);
		/* do whatever you want to do to the task... */

		task_unlock(task);
	}

	rcu_read_unlock();

	return 0;
}

static void __exit list_processes_exit(void)
{
	pr_info("%s: exited\n", __FUNCTION__);
}

module_init(list_processes_init);
module_exit(list_processes_exit);

MODULE_LICENSE("GPL");
