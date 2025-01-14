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
#define M_ENCRYPT 0
#define M_DECRYPT 1

MODULE_LICENSE("Dual BSD/GPL");

static int majorNumber;
static int translate_shift = 3;
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

void encrypt(char *buffer, size_t len) {
  for(int i = 0; i<len; i++){
    if(buffer[i] == 32){
      // Leerzeichen als ursprung behandeln
      buffer[i] = 96 + translate_shift;
      continue;
    }
    // nicht behandelte Zeichen
    if((buffer[i] < 65) || (buffer[i] > 90 && buffer[i] < 97) || (buffer[i] > 122)) continue;

    if (buffer[i] + translate_shift == 91) {
      // Leerzeichen als ergebnis behandeln
      buffer[i] = 32;
      continue;
    } else if (buffer[i] + translate_shift > 122) {
      // Kleinbuchstaben overflow
      buffer[i] = buffer[i] + translate_shift - 58;
      continue;
    } else if (buffer[i] + translate_shift > 90 && buffer[i] <= 90) {
      // Großbuchstaben overflow
      buffer[i] = buffer[i] + translate_shift + 5;
      continue;
    }

    buffer[i] = buffer[i] + translate_shift;
  }
}

void decrypt(char* buffer, size_t len){
  for(int i = 0; i<len; i++){
    if(buffer[i] == 32){
      // Leerzeichen als ursprung behandeln
      buffer[i] = 91 - translate_shift;
      continue;
    }
    // nicht behandelte Zeichen
    if((buffer[i] < 65) || (buffer[i] > 90 && buffer[i] < 97) || (buffer[i] > 122)) continue;

    if (buffer[i] - translate_shift == 96) {
      // Leerzeichen als ergebnis behandeln
      buffer[i] = 32;
      continue;
    } else if (buffer[i] - translate_shift < 65) {
      // Großbuchstaben underflow
      buffer[i] = buffer[i] - translate_shift + 58;
      continue;
    } else if (buffer[i] - translate_shift < 97  && buffer[i] >= 97) {
      // Kleinbuchstaben underflow
      buffer[i] = buffer[i] - translate_shift - 5;
      continue;
    }
    buffer[i] = buffer[i] - translate_shift;
  }
}

static int dev_open(struct inode *inodep, struct file *filep){
  unsigned int minor_num = iminor(inodep);
  enc_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);
  dec_buf = kmalloc(BUFFER_SIZE, GFP_KERNEL);

  return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
  unsigned int minor_num = iminor(filep->f_inode);

  if (minor_num == M_ENCRYPT) {
    copy_to_user(buffer, enc_buf, len);
  } else if (minor_num == M_DECRYPT) {
    copy_to_user(buffer, dec_buf, len);
  }

  return 0;
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
  unsigned int minor_num = iminor(filep->f_inode);
    // TODO len > 40
  if (minor_num == M_ENCRYPT) {
    copy_from_user(enc_buf, buffer, len);
    encrypt(enc_buf, len);
    printk(KERN_INFO "verschluesseln...");

  } else if (minor_num == M_DECRYPT) {
    copy_from_user(dec_buf, buffer, len);
    decrypt(dec_buf, len);
    printk(KERN_INFO "entschluesseln...");

  } else {
    printk(KERN_INFO "nicht untersteutzte minor number: %d", minor_num);
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
  encryptDevice = device_create(devClass, NULL, MKDEV(majorNumber, M_ENCRYPT), NULL, DEVICE_NAME_0);
  if (IS_ERR(encryptDevice)){               // Clean up if there is an error
    class_destroy(devClass);           // Repeated code but the alternative is goto statements
    unregister_chrdev(majorNumber, DEVICE_NAME_0);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(encryptDevice);
  }
  printk(KERN_INFO "caesar: device class (encrypt) created correctly\n"); // Made it! device was initialized

  decryptDevice = device_create(devClass, NULL, MKDEV(majorNumber, M_DECRYPT), NULL, DEVICE_NAME_1);
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
  device_destroy(devClass, MKDEV(majorNumber, M_ENCRYPT));    // remove the device
  device_destroy(devClass, MKDEV(majorNumber, M_DECRYPT));
  class_unregister(devClass);                          // unregister the device class
  class_destroy(devClass);                             // remove the device class
  unregister_chrdev(majorNumber, DEVICE_NAME_0);             // unregister the major number
  unregister_chrdev(majorNumber, DEVICE_NAME_1);             // unregister the major number
  printk(KERN_INFO "caesar: Goodbye from the LKM!\n");
}

module_init(caesar_init);
module_exit(caesar_exit);