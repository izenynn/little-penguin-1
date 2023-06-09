#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("izenynn");
MODULE_DESCRIPTION("A simple Hello world kernel module");
MODULE_VERSION("0.1");

static int __init hello_init(void)
{
	//printk(KERN_INFO "Hello world !\n");
	pr_info(KERN_INFO "Hello world !\n");
	return 0;
}

static void __exit hello_exit(void)
{
	//printk(KERN_INFO "Cleaning up module.\n");
	pr_info("Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
