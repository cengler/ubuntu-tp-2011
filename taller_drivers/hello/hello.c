#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void) {
	printk(KERN_ALERT "Aloha honua!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_ALERT "Adios mundo cruel...\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sabi");
MODULE_DESCRIPTION("'Aloha honua' desde el kernel.");
