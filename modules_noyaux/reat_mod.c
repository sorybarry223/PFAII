    /*ln -s /usr/src/linux-headers-4.4.54-ti-r93/ /lib/modules/4.4.54-ti-r93/build
      and if you already have folder "build" in lib/modules/4.4.54-ti-r93
       you have to delete it before*/



#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define BUFSIZE  50
 
 
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Liran B.H");
 

unsigned long int i=0;

char buf[BUFSIZE];
 
static struct proc_dir_entry *ent;
 
static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{

	int len=0;
		if (i>=100000000) i=0;
	len = sprintf(buf,"%ld\r",i++);
	
	if(copy_to_user(ubuf,buf,len))return -EFAULT;
	
	return len;
}
 
static struct proc_ops myops = {

	.proc_read = myread,
};
 
static int simple_init(void)
{
	ent=proc_create("mydev",0660,NULL,&myops);
	printk(KERN_ALERT "hello...\n");
	return 0;
}
 
static void simple_cleanup(void)
{
	proc_remove(ent);
	printk(KERN_WARNING "bye ...\n");
}
 
module_init(simple_init);
module_exit(simple_cleanup);

