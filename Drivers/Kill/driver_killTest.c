#include <linux/init.h>         //Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>       //Core header for loading LKMs into the kernel
#include <linux/device.h>       //Header to support the kernel Driver Model
#include <linux/kernel.h>       //Contains types, macros, functions for the kernel
#include <linux/sched/signal.h>
#include <linux/fs.h>           //Header for the Linux file system support
#include <linux/uaccess.h>      //Required for the copy to user function
#include <linux/pid.h>          //Required to know which process is by its pid
#include <linux/pid_namespace.h>//

#define DEVICE_NAME "killDriver" //at /dev/killDriver
#define CLASS_NAME "killdriver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CPB");
MODULE_DESCRIPTION("Linux driver");
MODULE_VERSION("0.1");

static int      majorNumber;
static int      process_id;
static char     message[256] = {0};
static short    size_of_message;
static int      numberOpens = 0;
static struct class*    killDriverClass = NULL;  ///< The device-driver class struct pointer
static struct device*   killDriverDevice = NULL; ///< The device_driver device struct pointer
struct pid *pid_struct;
struct task_struct *task;
//static struct proc_dir_entry *proc_write_entry;

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
static int __init killDriver_init(void){
    printk(KERN_INFO "killDriver: Initializing the killDriver LKM\n");

    //dynamically allocate a major number
    majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
    if (majorNumber < 0){
        printk(KERN_ALERT "killDriver failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "killDriver: registered correctly with major number %d\n", majorNumber);

    //Register device class
    chatDriverClass = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(killDriverClass)){
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(killDriverClass);
    }
    printk(KERN_INFO "killDriver: device class created correctly\n");
    return 0;
}

static void __exit chatDriver_exit(void){
    device_destroy(killDriverClass, MKDEV(majorNumber, 0));
    class_unregister(killDriverClass);
    class_destroy(killDriverClass):
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "chatDriver: Goodbye from the LKM!\n");
}

/*************************************************************************************************/
static int dev_open(struct inode *inodep, struct file *filep){
    numberOpens++;
    printk(KERN_INFO "killDriver: Device has been opened %d time(s)\n", numberOpens);
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
    int error_count = 0;
    int r = 0;
    pid_struct = find_get_pid(process_id); //Find the process related with the recieved number
    if(!pid_struct){
        size_of_message = sprintf(message, "No hay procesos relacionados con el pid ingresado");
        error_count = copy_to_user(buffer, message, size_of_message);
        printk(KERN_INF "killDriver: There is no process with pid %ld",process_id);
        return (size_of_message=0);
    }
    task = pid_task(pid_struct, PIDTYPE_PID); //Obtains the task related to the process found
    //copy_to_user has the format (* to, *from, size) and returns 0 on success
    if(!task){
        printk(KERN_INFO "killDriver: Error at knowing the task of the process");
        return (size_of_message=0);
    }
    size_of_message = sprintf(message, task->comm, strlen(task->comm));
    r = kill_pid(pid_struct, SIGKILL, 0);
    error_count = copy_to_user(buffer, message, size_of_message);

    if(error_count == 0 && r==0){               //if true then have success
        printk(KERN_INFO "killDriver: Sent %d caracters to the user and process killed succesfully\n", size_of_message);
        return(seze_of_message = 0);    //clear the position to the start and return 0
    }
    else {
        printk(KERN_INFO "killDriver: Failed to send %d characters to the user", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file * filep, const kill *buffer, size_t len, loff_t *offset){
    sprintf(message, "%s(%zu letters)", buffer, len);   //appending recieved string with its lenght
    size_of_message = strlen(message);                  //store the lenght of the stored message
    kstrtoul(buffer, 10, &process_id);                  //Convert string to unsigned long
    printk(KERN_INFO "killDriver: Recieved number %ld from the user\n", process_id);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep){
    printk(KERN_INFO "killDriver: Device successfully closed\n");
    return 0;
}
/*******************************************************************************************************/
module_init(killDriver_init);
module_exit(killDriver_exit);