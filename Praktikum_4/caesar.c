#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#define DEVICE_NAME_0 "encrypt"
#define DEVICE_NAME_1 "decrypt"
#define CLASS_NAME "ebb"
#define BUFFER_SIZE sizeof(char) * 40

MODULE_LICENSE("Dual BSD/GPL");

static int majorNumber;
static char translate_shift = 3;
module_param(translate_shift, int, S_IRUGO);
static struct class*  devClass  = NULL;
static struct device* encryptDevice = NULL;
static struct device* decryptDevice = NULL;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

char* enc_buf;
char* dec_buf;



static struct file_operations fops =
{
  .open = dev_open,
  .read = dev_read,
  .write = dev_write,
  .release = dev_release,
};

static int dev_open(struct inode *inodep, struct file *filep){
  unsigned int minor_num = iminor(inodep);
  enc_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
  dec_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);

  return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){

  // verschlüsselten oder entschlüsselten Text ausgeben
  copy_to_user(enc_buf, buffer, len);
  return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  unsigned int minor_num = iminor(filep->f_inode);
  copy_from_user(enc_buf, buffer, len);
  if (minor_num == 0) {
    // Text verschlüsseln und auf den puffer legen
    for(int i = 0; i<len; i++){
      enc_buf[i] = enc_buf[i] + translate_shift;
    }
    printk(KERN_INFO "verschluesseln...");
  } else if (minor_num == 1) {
    // Text entschlüsseln und auf den puffer legen
    printk(KERN_INFO "entschluesseln...");
  } else {
    printk(KERN_INFO "write %d...", minor_num);
  }

  return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
  unsigned int minor_num = iminor(inodep);
  kfree(enc_buf);
  kfree(dec_buf);
  return 0;
}

static int caesar_init(void)
{
  majorNumber = register_chrdev(0, DEVICE_NAME_0, &fops);
  if (majorNumber<0){
    printk(KERN_ALERT "caesar: failed to register a major number\n");
    return majorNumber;
  }
  printk(KERN_INFO "caesar: registered correctly with major number %d\n", majorNumber);

  devClass = class_create(CLASS_NAME);
  if (IS_ERR(devClass)){                // Check for error and clean up if there is
    unregister_chrdev(majorNumber, DEVICE_NAME_0);
    printk(KERN_ALERT "Failed to register device class\n");
    return PTR_ERR(devClass);          // Correct way to return an error on a pointer
  }
  printk(KERN_INFO "caesar: device class registered correctly\n");

  // Register the device driver
  encryptDevice = device_create(devClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME_0);
  if (IS_ERR(encryptDevice)){               // Clean up if there is an error
    class_destroy(devClass);           // Repeated code but the alternative is goto statements
    unregister_chrdev(majorNumber, DEVICE_NAME_0);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(encryptDevice);
  }
  printk(KERN_INFO "caesar: device class (encrypt) created correctly\n"); // Made it! device was initialized

  decryptDevice = device_create(devClass, NULL, MKDEV(majorNumber, 1), NULL, DEVICE_NAME_1);
  if (IS_ERR(decryptDevice)){               // Clean up if there is an error
    class_destroy(devClass);           // Repeated code but the alternative is goto statements
    unregister_chrdev(majorNumber, DEVICE_NAME_1);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(decryptDevice);
  }
  printk(KERN_INFO "caesar: device class (decrypt) created correctly\n");

  return 0;
}

static void caesar_exit(void) {
  device_destroy(devClass, MKDEV(majorNumber, 0));    // remove the device
  device_destroy(devClass, MKDEV(majorNumber, 1));
  class_unregister(devClass);                          // unregister the device class
  class_destroy(devClass);                             // remove the device class
  unregister_chrdev(majorNumber, DEVICE_NAME_0);             // unregister the major number
  unregister_chrdev(majorNumber, DEVICE_NAME_1);             // unregister the major number
  printk(KERN_INFO "caesar: Goodbye from the LKM!\n");
}

module_init(caesar_init);
module_exit(caesar_exit);