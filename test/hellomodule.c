#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define DEV_NAME "hellomodule"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("helloworld module");

static int major;

static unsigned long ADDR = 0x20200000;
static unsigned long RANGE = 0x40;
static void *addr;

static int dev_open(struct inode *ip, struct file *fp)
{
	printk(KERN_INFO "dev_open\n");
	return 0;
}

static ssize_t dev_write(struct file *fp, const char *buffer, size_t len, loff_t *offset)
{
	printk(KERN_INFO "dev_write %d characters\n", len);

	//*((unsigned int*)0x20200000) = 1<<15;
	//*((unsigned int*)0x2020001c) = 1<<5;
	writel(1<<15, addr);
	writel(1<<5, (addr+0x28));
	if(len==4)
		writel(1<<5, (addr+0x1c));

	return len;
}
	
static int dev_release(struct inode *ip, struct file *fp)
{
	printk(KERN_INFO "dev_released\n");
	return 0;
}

static struct file_operations fops =
{
	.open = dev_open,
	.write = dev_write,
	.release = dev_release,
};

static int mod_init(void)
{
	major = register_chrdev(0, DEV_NAME, &fops);
	printk(KERN_INFO "mod_init %d\n", major);
	//if(check_mem_region(ADDR, RANGE))
		request_mem_region(ADDR, RANGE, DEV_NAME);
	//else
		//printk(KERN_WARNING "Cannot get I/O region\n");
	addr = ioremap(ADDR, RANGE);

	return 0;
}

static void mod_exit(void)
{
	printk(KERN_INFO "mod_exit\n");
	iounmap(addr);
	release_mem_region(ADDR, RANGE);
	unregister_chrdev(major, DEV_NAME);
}

module_init(mod_init);
module_exit(mod_exit);


