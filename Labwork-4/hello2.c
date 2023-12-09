// SPDX-License-Identifier: GPL-2.0
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Gadiev Renat");
MODULE_DESCRIPTION("Hello, this is second module for labwork 4!");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_count = 1;

module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of times to print 'Hello, world!'");

static int __init hello_init(void)
{
	print_hello(hello_count);
	return 0;
}


static void __exit hello_exit(void)
{
	pr_info("Module hello2 out!\n");
}
module_init(hello_init);
module_exit(hello_exit);
