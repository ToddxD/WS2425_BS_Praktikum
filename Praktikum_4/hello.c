#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

static int translate_shift = 3;
module_param(translate_shift, int, S_IRUGO);

static int hello_init(void)
{
  printk(KERN_INFO "Die Zahlt lautet: %d\n", translate_shift);
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_INFO "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);