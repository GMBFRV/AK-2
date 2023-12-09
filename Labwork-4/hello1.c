// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Gadiev Renat");
MODULE_DESCRIPTION("Hello, this is first module for labwork-4!!");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
	struct list_head list;
	ktime_t timestamp_before;
	ktime_t timestamp_after;
	ktime_t print_duration;
};

static LIST_HEAD(hello_list);
static ktime_t unload_timestamp_before;

void print_hello(unsigned int hello_count)
{
	struct hello_event *event;
	int i;

	if (hello_count == 0 || (hello_count > 4 && hello_count <= 10)) {
	pr_warn("Warning: hello_count is 0 or between 5 and 10. Continuing...\n");
	} else if (hello_count > 10) {
	pr_err("Error: hello_count is greater than 10. Module cannot load.\n");
	return;
	}

	for (i = 0; i < hello_count; ++i) {
	event = kmalloc(sizeof(*event), GFP_KERNEL);
	if (!event) {
		pr_err("Failed to allocate memory for hello_event\n");
		return;
		}

	event->timestamp_before = ktime_get();
	pr_info("Iteration %d: Hello world!\n", i + 1);
	event->timestamp_after = ktime_get();
	event->print_duration = ktime_sub(event->timestamp_after,
	event->timestamp_before);
	list_add_tail(&event->list, &hello_list);
	}
}
EXPORT_SYMBOL(print_hello);


static void __exit hello_exit(void)
{
	struct hello_event *event, *tmp;
	int i = 1;

	pr_info("Module hello1 out!\n");

	list_for_each_entry_safe(event, tmp, &hello_list, list) {
		pr_info("Привітання #%d:\n", i);
		pr_info("   Час події: %lld ns\n",
		ktime_to_ns(event->timestamp_before));
		pr_info("   Час, який пішов на друк: %lld ns\n",
		ktime_to_ns(event->print_duration));
		list_del(&event->list);
		kfree(event);
		i++;
	}

	unload_timestamp_before = ktime_get();
}

module_exit(hello_exit);
