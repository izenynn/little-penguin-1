#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

// Dont have a license, LOL
MODULE_LICENSE("LICENSE");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static char myfd_data[PAGE_SIZE];
static DEFINE_MUTEX(myfd_mutex);

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size,
			 loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
			  loff_t *offs);
static loff_t myfd_llseek(struct file *fp, loff_t offs, int whence);

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write,
	.llseek = &myfd_llseek
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops,
	.mode = 0666
};

static int __init myfd_init(void)
{
	int ret;

	ret = misc_register(&myfd_device);
	if (ret)
		pr_err("Could not register misc device\n");

	return ret;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

ssize_t myfd_read(struct file *fp, char __user *user, size_t size,
		  loff_t *offs)
{
	size_t t, i;
	ssize_t ret;
	char *aux;

	aux = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	if (aux == NULL)
		return -ENOMEM;

	mutex_lock(&myfd_mutex);
	for (t = strlen(myfd_data), i = 0; t > 0; --t, ++i)
		aux[i] = myfd_data[t - 1];
	aux[i] = '\0';
	ret = simple_read_from_buffer(user, size, offs, aux, i);
	mutex_unlock(&myfd_mutex);

	kfree(aux);

	return ret;
}

ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		   loff_t *offs)
{
	ssize_t ret;

	if (*offs + size > PAGE_SIZE - 1)
		return -EINVAL;

	mutex_lock(&myfd_mutex);
	ret = simple_write_to_buffer(myfd_data, PAGE_SIZE - 1, offs, user, size);
	if (ret < 0) {
		mutex_unlock(&myfd_mutex);
		myfd_data[0] = '\0';
		return ret;
	}
	myfd_data[*offs + ret] = '\0';
	mutex_unlock(&myfd_mutex);

	return ret;
}

static loff_t myfd_llseek(struct file *fp, loff_t offset, int whence)
{
	loff_t newpos = 0;
	loff_t size;

	size = strnlen(myfd_data, PAGE_SIZE);

	switch (whence) {
	case SEEK_SET:
		if (offset < 0)
			return -EINVAL;
		newpos = offset;
		break;
	case SEEK_CUR:
		newpos = fp->f_pos + offset;
		break;
	case SEEK_END:
		newpos = size + offset;
		break;
	default: /* Should never happen */
		return -EINVAL;
	}

	if (newpos < 0 || newpos > size)
		return -EINVAL;

	fp->f_pos = newpos;
	return newpos;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
