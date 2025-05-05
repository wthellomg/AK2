#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>

MODULE_AUTHOR("Sviatoslav Zakharov, IO-24 <9818138@gmail.com>");
MODULE_DESCRIPTION("AK2, lab 4");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat = 1;
module_param(repeat, uint, 0644);
MODULE_PARM_DESC(repeat, "Number of times to print 'Hello, world!'");

struct hello_time {
	ktime_t time;
	struct list_head list;
};

static LIST_HEAD(hello_list);

static int __init hello_init(void)
{
	int i;
	struct hello_time *entry;

	if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
		printk(KERN_WARNING "Warning: unusual repeat value (%d)\n", repeat);
	} else if (repeat > 10) {
		printk(KERN_ERR "Error: repeat value too large (%d)\n", repeat);
		return -EINVAL;
	}

	for (i = 0; i < repeat; i++) {
		entry = kmalloc(sizeof(*entry), GFP_KERNEL);
		if (!entry) {
			printk(KERN_ERR "Failed to allocate memory!\n");
			return -ENOMEM;
		}
		entry->time = ktime_get();
		list_add(&entry->list, &hello_list);
		printk(KERN_INFO "Hello, world! (iteration %d)\n", i+1);
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_time *pos, *tmp;
	printk(KERN_INFO "--- Timestamps (ns) ---\n");
	list_for_each_entry_safe(pos, tmp, &hello_list, list) {
		printk(KERN_INFO "%lld\n", ktime_to_ns(pos->time));
		list_del(&pos->list);
		kfree(pos);
	}
}

module_init(hello_init);
module_exit(hello_exit);
