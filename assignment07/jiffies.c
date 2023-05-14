#include <linux/module.h>
#include <linux/fs.h>
#include <linux/jiffies.h>


static ssize_t jiffies_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char jiffies_value[64];
	int len = snprintf(jiffies_value, sizeof(jiffies_value), "%lu\n", jiffies);
	return simple_read_from_buffer(buf, count, ppos, jiffies_value, len);
}

const struct file_operations jiffies_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_read,
	.llseek = no_llseek,
};
EXPORT_SYMBOL(jiffies_fops);
