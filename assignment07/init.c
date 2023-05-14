#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/debugfs.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("izenynn");
MODULE_DESCRIPTION("A simple debugfs char device driver");
MODULE_VERSION("0.1");

extern const struct file_operations id_fops;
extern const struct file_operations jiffies_fops;
extern const struct file_operations foo_fops;

static struct dentry *dir, *jiffies_file, *id_file, *foo_file;

static int __init fortytwo_init(void)
{
	pr_info("Hello world !\n");

	dir = debugfs_create_dir("fortytwo", NULL);
	if (!dir) {
		pr_err("Could not create directory\n");
		return -ENOMEM;
	}

	id_file = debugfs_create_file("id", 0666, dir, NULL, &id_fops);
	if (!id_file) {
		pr_err("Could not create id file\n");
		goto fail;
	}

	jiffies_file = debugfs_create_file("jiffies", 0444, dir, NULL, &jiffies_fops);
	if (!jiffies_file) {
		pr_err("Could not create jiffies file\n");
		goto fail;
	}
	// This other approach is cooler but it won't let us check for errors or
	// implement custom behavior, but it will work for the assignment
	// debugfs_create_u64("jiffies", 0444, dir, (u64 *)&jiffies);


	foo_file = debugfs_create_file("foo", 0644, dir, NULL, &foo_fops);
	if (!foo_file) {
		pr_err("Could not create foo file\n");
		goto fail;
	}

	return 0;

fail:
	debugfs_remove_recursive(dir);
	return -ENOMEM;
}

static void __exit fortytwo_exit(void)
{
	debugfs_remove_recursive(dir);
	pr_info("Cleaning up module.\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);
