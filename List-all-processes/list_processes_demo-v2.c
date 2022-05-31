// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Demo usage of atomic for a counter
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/sched/signal.h>

static int __init list_processes_init(void)
{
	struct task_struct *task;

	rcu_read_lock();

	for_each_process(task) {
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
