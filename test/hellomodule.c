#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>


static int mod_init(void)
{
	printk(KERN_INFO "Hello World!\n");
	return 0;
}

static void  mod_exit(void)
{
	printk(KERN_INFO "Module exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_AUTHOR("WHOWHO");
MODULE_DESCRIPTION("hwlloworld module");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:helloworld");
