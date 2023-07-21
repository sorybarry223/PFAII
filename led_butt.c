//irq-gpio-linux.c:
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
//#include <time.h>
#define BUFSIZE  50


#define GPIO_IN 123	//144 //GPIO4_27 formul
#define GPIO_OUT 112 //GPIO4_16 formul (4-1)*32+16

char buf[BUFSIZE];
static struct proc_dir_entry *ent;

uint64_t 	T, tn,to;
static int numero_interruption = 0;


static ssize_t myread(struct file *file, char __user *ubuf,size_t count, loff_t *ppos) 
{

	int len=0;

	len = sprintf(buf,"%ld\r",(long int)T);
	
	if(copy_to_user(ubuf,buf,len))return -EFAULT;
	
	return len;
}
 
static struct proc_ops myops = {

	.proc_read = myread,
};

irqreturn_t handler_irq_gpio(int irq, void * ident)
{
  static int value = 0;
  gpio_set_value(GPIO_OUT, value);
  value = 1 - value;
  tn = ktime_get_real_ns();
	T = tn-to;
	to=tn;
	//t0 = ktime_get_real_ns();
  
  return IRQ_HANDLED;
}

int __init exemple_init (void)
{
  int err;

  numero_interruption = gpio_to_irq(GPIO_IN);

  if ((err = gpio_request(GPIO_OUT, THIS_MODULE->name)) != 0)  return err;
  if ((err = gpio_direction_output(GPIO_OUT, 0)) != 0) {gpio_free(GPIO_OUT);return err;}
  if ((err = gpio_request(GPIO_IN, THIS_MODULE->name)) != 0) {gpio_free(GPIO_OUT);return err;}
  if ((err = gpio_direction_input(GPIO_IN)) != 0) {gpio_free(GPIO_IN);gpio_free(GPIO_OUT);return err;}
  if ((err = request_irq(numero_interruption, handler_irq_gpio, 0, THIS_MODULE->name, THIS_MODULE->name)) != 0) 
     { gpio_free(GPIO_IN); gpio_free(GPIO_OUT); return err;}
  irq_set_irq_type(numero_interruption, IRQF_TRIGGER_RISING);
  ent=proc_create("mydev",0660,NULL,&myops);
	printk(KERN_ALERT "hello...\n");

  return 0;
}

void __exit exemple_exit (void)
{
  free_irq(numero_interruption, THIS_MODULE->name);
  gpio_free(GPIO_IN);
  gpio_free(GPIO_OUT);
  proc_remove(ent);
	printk(KERN_WARNING "bye ...\n");
}

module_init(exemple_init);
module_exit(exemple_exit);
MODULE_LICENSE("GPL"); 
