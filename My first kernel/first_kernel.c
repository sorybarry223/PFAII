#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init first_init(void)
{	
	pr_info("Hello Sory!\n");
	return 0;
}

static void __exit first_exit(void)
{
	pr_info("Bye\n");
}
module_init(first_init);
module_exit(first_exit);

MODULE_LICENCE("GPL");
MODULE_DESCRIPTION("My first module");
MODULE_AUTHOR("The Barry");
