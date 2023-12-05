#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Gadiev Renat");
MODULE_DESCRIPTION("Hello, this is labwork 3!");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_event {
    struct list_head list;
    ktime_t timestamp;
};

static LIST_HEAD(hello_list);

static unsigned int hello_count = 1;

module_param(hello_count, uint, S_IRUGO);
MODULE_PARM_DESC(hello_count, "Number of times to print 'Hello, world!'");

static int __init hello_init(void)
{
    struct hello_event *event;
    int i;

    if (hello_count == 0 || (hello_count > 4 && hello_count <= 10)) {
        pr_warn("Warning: hello_count is 0 or between 5 and 10. Continuing...\n");
    } else if (hello_count > 10) {
        pr_err("Error: hello_count is greater than 10. Module cannot load.\n");
        return -EINVAL;
    }

    for (i = 0; i < hello_count; ++i) {
        printk(KERN_EMERG "Hello, world!\n");

        event = kmalloc(sizeof(*event), GFP_KERNEL);
        if (!event) {
            pr_err("Failed to allocate memory for hello_event\n");
            return -ENOMEM;
        }

        event->timestamp = ktime_get();
        list_add_tail(&event->list, &hello_list);
    }

    return 0;
}


static void __exit hello_exit(void)
{
    struct hello_event *event, *tmp;

    list_for_each_entry_safe(event, tmp, &hello_list, list) {
        printk(KERN_EMERG "Goodbye, world! Event time: %lld ns\n", ktime_to_ns(event->timestamp));
        list_del(&event->list);
        kfree(event);
    }
}

module_init(hello_init);
module_exit(hello_exit);


