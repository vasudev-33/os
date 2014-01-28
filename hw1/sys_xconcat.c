#include <linux/linkage.h>
#include <linux/moduleloader.h>

asmlinkage extern long (*sysptr)(void *arg);

asmlinkage long xconcat(void *arg)
{
	/* dummy syscall: returns 0 for non null, -EINVAL for NULL */
	if (arg == NULL)
		return -EINVAL;
	else
		return 0;
}

static int __init init_sys_xconcat(void)
{
	printk("installed new sys_xconcat module\n");
	if (sysptr == NULL)
		sysptr = xconcat;
	return 0;
}
static void  __exit exit_sys_xconcat(void)
{
	if (sysptr != NULL)
		sysptr = NULL;
	printk("removed sys_xconcat module\n");
}
module_init(init_sys_xconcat);
module_exit(exit_sys_xconcat);
MODULE_LICENSE("GPL");
