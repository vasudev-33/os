#include <linux/linkage.h>
#include <linux/moduleloader.h>

asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long xintegrity(void *arg)
{
	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	if (arg == NULL)
		return -EINVAL;
	else
		return 0;
}

static int __init init_sys_xintegrity(void)
{
	printk("installed new sys_xintegrity module\n");
	if (sysptr == NULL)
		sysptr = xintegrity;
	return 0;
}
static void  __exit exit_sys_xintegrity(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	printk("removed sys_xintegrity module\n");
}
module_init(init_sys_xintegrity);
module_exit(exit_sys_xintegrity);
MODULE_LICENSE("GPL");
