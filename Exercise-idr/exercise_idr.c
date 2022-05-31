// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Demo usage of atomic for a counter
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>

#define NAME_MAX_LEN 32

struct my_node_t {
	char name[NAME_MAX_LEN];

	/* Add something more... */
};

static struct idr *my_idr;

#define NUM_NODES 5
static struct my_node_t *my_nodes;

static int __init exercise_idr_init(void)
{
	struct my_node_t *node;
	int id;
	int i;

	my_idr = kmalloc(sizeof(*my_idr), GFP_KERNEL);
	if (!my_idr)
		return -ENOMEM;
	idr_init(my_idr);

	my_nodes = kcalloc(NUM_NODES, sizeof(struct my_node_t), GFP_KERNEL);
	if (!my_nodes)
		goto err;

	for (i = 0; i < NUM_NODES; i++) {
		snprintf(my_nodes[i].name, NAME_MAX_LEN, "My Node %d", i);

		id = idr_alloc(my_idr, &my_nodes[i], 10 * i, 10 * i + 1,
			       GFP_KERNEL);
		if (id < 0)
			pr_err("Failed to allocate id for node %d\n", i);
	}

	idr_for_each_entry(my_idr, node, id)
		pr_info("node 0x%px (%s)'s ID is %d\n", node, node->name, id);

	return 0;
err:
	idr_destroy(my_idr);
	kfree(my_idr);
	my_idr = NULL;
	return -ENOMEM;
}

static void __exit exercise_idr_exit(void)
{
	kfree(my_nodes);

	idr_destroy(my_idr);
	kfree(my_idr);

	pr_info("%s: exited\n", __FUNCTION__);
}

module_init(exercise_idr_init);
module_exit(exercise_idr_exit);

MODULE_LICENSE("GPL");
