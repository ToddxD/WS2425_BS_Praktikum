#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define DEVICE_NAME "caesar"

MODULE_LICENSE("Dual BSD/GPL");

static int majorNumber;
static int translate_shift = 3;
module_param(translate_shift, int, S_IRUGO);

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
  .open = dev_open,
  .read = dev_read,
  .write = dev_write,
  .release = dev_release,
};

static int dev_open(struct inode *inodep, struct file *filep){
  return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
  return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
  return 0;
}

static int caesar_init(void)
{
  majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
  if (majorNumber<0){
    printk(KERN_ALERT "caesar: failed to register a major number\n");
    return majorNumber;
  }
  printk(KERN_INFO "caesar: registered correctly with major number %d\n", majorNumber);
  return 0;
}

static void caesar_exit(void)
{
  printk(KERN_INFO "Goodbye, cruel world\n");
}

module_init(caesar_init);
module_exit(caesar_exit);