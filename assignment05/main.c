#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("izenynn");
MODULE_DESCRIPTION("A simple misc char device driver");
MODULE_VERSION("0.1");

#define STUDENT_LOGIN "dpoveda"

static ssize_t fortytwo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char	input[sizeof(STUDENT_LOGIN) / sizeof(char)];
	char	*login = STUDENT_LOGIN;
	size_t	len = strlen(login);

	if (count != len)
		return -EINVAL;

	if (copy_from_user(input, buf, count))
		return -EFAULT;

	if (strncmp(input, login, len) != 0)
		return -EINVAL;

	return count;
}

static ssize_t fortytwo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char	*login = STUDENT_LOGIN;
	size_t	len = strlen(login);

	if (*ppos >= len)
		return 0;

	if (count > len - *ppos)
		count = len - *ppos;

	if (copy_to_user(buf, login + *ppos, count))
		return -EFAULT;

	*ppos += count;
	return count;
}

static loff_t fortytwo_llseek(struct file *file, loff_t offset, int whence)
{
	loff_t newpos;
	size_t len = strlen(STUDENT_LOGIN);

	switch (whence) {
	case 0: /* SEEK_SET */
		newpos = offset;
		break;
	case 1: /* SEEK_CUR */
		newpos = file->f_pos + offset;
		break;
	case 2: /* SEEK_END */
		newpos = len + offset;
		break;
	default: /* can't happen */
		return -EINVAL;
	}

	if (newpos < 0 || newpos > len)
		return -EINVAL;
	
	file->f_pos = newpos;
	return newpos;
}


static const struct file_operations fortytwo_fops = {
	.owner		= THIS_MODULE,
	.write		= fortytwo_write,
	.read		= fortytwo_read,
	.open		= NULL,
	.release	= NULL,
	.llseek		= fortytwo_llseek,
};

static struct miscdevice fortytwo_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &fortytwo_fops,
};

static int __init fortytwo_init(void)
{
	int ret;

	pr_info("Hello world !\n");

	ret = misc_register(&fortytwo_misc_device);
	if (ret)
		pr_err("Could not register misc device\n");

	return ret;
}

static void __exit fortytwo_exit(void)
{
	misc_deregister(&fortytwo_misc_device);
	pr_info("Cleaning up module.\n");
}

module_init(fortytwo_init);
module_exit(fortytwo_exit);
