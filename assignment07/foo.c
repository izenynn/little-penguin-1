#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

static char foo_data[PAGE_SIZE];
static int foo_len;
static DEFINE_MUTEX(foo_mutex);

static ssize_t foo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	ssize_t ret;

	mutex_lock(&foo_mutex);
	ret = simple_read_from_buffer(buf, count, ppos, foo_data, foo_len);
	mutex_unlock(&foo_mutex);

	return ret;
}

static ssize_t foo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	if (count > PAGE_SIZE)
		return -EINVAL;

	mutex_lock(&foo_mutex);
	if (copy_from_user(foo_data, buf, count)) {
		mutex_unlock(&foo_mutex);
		return -EFAULT;
	}

	foo_len = count;
	mutex_unlock(&foo_mutex);

	return count;
}

const struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.write = foo_write,
	.read = foo_read,
	.llseek = no_llseek,
};
EXPORT_SYMBOL(foo_fops);
