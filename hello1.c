// SPDX-License-Identifier: UNLICENSED
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <hello1.h>

MODULE_AUTHOR("Khinochek Vladislav IO-21 <Vladhinochek@gmail.com>");
MODULE_DESCRIPTION("Part of structure for printing 'Hello, world!'");
MODULE_LICENSE("Dual BSD/GPL");

struct timestamp_event {
	struct list_head list;
	ktime_t start_Time;
	ktime_t end_Time;
};

static LIST_HEAD(eventList);

void print_hello(void)
{
	//Working with list and memory
	struct timestamp_event *event = kmalloc(sizeof(*event),
	 	GFP_KERNEL);
	event->start_Time = ktime_get();
	pr_info("Hello, world!\n");
	event->end_Time = ktime_get();
	list_add_tail(&event->list, &eventList);
}
EXPORT_SYMBOL(print_hello);

static void __exit hello1_exit(void)
{
	struct timestamp_event *event;
	struct list_head *pos,  *tmp;

	// Travel the list
	list_for_each_safe(pos, tmp, &eventList) {
		event = list_entry(pos, struct timestamp_event, list);
		pr_info("Print time: %lld ns\n",
		        ktime_to_ns(event->end_Time) - ktime_to_ns(event->start_Time));
		list_del(pos);
		kfree(event);
	}
}
module_exit(hello1_exit);
