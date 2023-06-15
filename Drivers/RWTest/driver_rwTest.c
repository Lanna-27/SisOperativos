#include <linux/init.h>         //Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>       //Core header for loading LKMs into the kernel
#include <linux/device.h>       //Header to support the kernel Driver Model
#include <linux/kernel.h>       //Contains types, macros, functions for the kernel
#include <linux/fs.h>           //Header for the Linux file system support
#include <linux/uaccess.h>      //Required for the copy to user function

#define DEVICE_NAME "charDriver" //at /dev/charDriver
#define CLASS_NAME "chardriver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CPB");
MODULE_DESCRIPTION("Linux driver");
MODULE_VERSION("0.1");

static int      majorNumber;
static char     message[256] = {0};
static short    size_of_message;
static int      numberOpens = 0;
static struct class*    charDriverClass = NULL;  ///< The device-driver class struct pointer
static struct device*   charDriverDevice = NULL; ///< The device_driver device struct pointer

static int      dev_open(struct inode *, struct file *);
static int      dev_release(struct inode *, struct file *);
static ssize_t  dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t  dev_write(struct file *, const char *, size_t, loff_t *);

static struct static struct file_operations fops=
{
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

/********************************************************************************************/
static int __init charDriver_init(void){
    printk(KERN_INFO "charDriver: Initializing the charDriver LKM\n");

    //dynamically allocate a major number
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0){
        printk(KERN_ALERT "charDriver failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "charDriver: registered correctly with major number %d\n", majorNumber);

    //Register device class
    chatDriverClass = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(charDriverClass)){
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(charDriverClass);
    }
    printk(KERN_INFO "charDriver: device class created correctly\n");
    return 0;
}

static void __exit chatDriver_exit(void){
    device_destroy(charDriverClass, MKDEV(majorNumber, 0));
    class_unregister(charDriverClass);
    class_destroy(charDriverClass):
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "chatDriver: Goodbye from the LKM!\n");
}

/*************************************************************************************************/
static int dev_open(struct inode *inodep, struct file *filep){
    numberOpens++;
    printk(KERN_INFO "charDriver: Device has been opened %d time(s)\n", numberOpens);
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    int error_count = 0;
    //copy_to_user has the format (* to, *from, size) and returns 0 on success
    error_count = copy_to_user(buffer, message, size_of_message);

    if(error_count == 0){               //if true then have success
        printk(KERN_INFO "charDriver: Sent %d caracters to the user\n", size_of_message);
        return(seze_of_message = 0);    //clear the position to the start and return 0
    }
    else {
        printk(KERN_INFO "charDriver: Failed to send %d characters to the user", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file * filep, const char *buffer, size_t len, loff_t *offset){
    sprintf(message, "%s(%zu letters)", buffer, len);   //appending recieved string with its lenght
    size_of_message = strlen(message);                  //store the lenght of the stored message
    printk(KERN_INFO "charDriver: Recieved %zu characters from the user\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO "charDriver: Device successfully closed\n");
    return 0;
}
/*******************************************************************************************************/
module_init(charDriver_init);
module_exit(chatDriver_exit);