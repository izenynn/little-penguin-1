#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define STUDENT_LOGIN "dpoveda"

static ssize_t id_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char input[sizeof(STUDENT_LOGIN)];
	size_t len = strlen(STUDENT_LOGIN);

	if (count != len)
		return -EINVAL;

	if (copy_from_user(input, buf, count))
		return -EFAULT;

	if (strncmp(input, STUDENT_LOGIN, len) != 0)
		return -EINVAL;

	return count;
}

static ssize_t id_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, STUDENT_LOGIN, strlen(STUDENT_LOGIN));
	/*char	*login = STUDENT_LOGIN;
	size_t	len = strlen(login);

	if (*ppos >= len)
		return 0;

	if (count > len - *ppos)
		count = len - *ppos;

	if (copy_to_user(buf, login + *ppos, count))
		return -EFAULT;

	*ppos += count;
	return count;*/
}

static loff_t id_llseek(struct file *file, loff_t offset, int whence)
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
	default: /* Can't happen */
		return -EINVAL;
	}

	if (newpos < 0 || newpos > len)
		return -EINVAL;
	
	file->f_pos = newpos;
	return newpos;
}

const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.write = id_write,
	.read = id_read,
	.llseek = id_llseek,
};
EXPORT_SYMBOL(id_fops);
