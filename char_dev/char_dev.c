#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/types.h>

#define MEM_SIZE 1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev my_cdev;
uint8_t *kernel_buffer;

static int __init chr_driver_init(void);
static void __exit chr_driver_exit(void);
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_release,
};

static int my_open(struct inode *inode, struct file *file) {
    kernel_buffer = kmalloc(MEM_SIZE, GFP_KERNEL);
    if (!kernel_buffer) {
        pr_err("Failed to allocate memory in kernel\n");
        return -ENOMEM;
    }
    pr_info("Device file opened\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file) {
    kfree(kernel_buffer);
    pr_info("Device file closed\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    if (copy_to_user(buf, kernel_buffer, MEM_SIZE)) {
        pr_err("Failed to copy data to user space\n");
        return -EFAULT;
    }
    pr_info("Data read: DONE\n");
    return MEM_SIZE;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
    if (len > MEM_SIZE) {
        pr_err("Write size exceeds buffer capacity\n");
        return -ENOMEM;
    }
    if (copy_from_user(kernel_buffer, buf, len)) {
        pr_err("Failed to copy data from user space\n");
        return -EFAULT;
    }
    pr_info("Data written successfully\n");
    return len;
}

static int __init chr_driver_init(void) {
    if ((alloc_chrdev_region(&dev, 0, 1, "my_Dev")) < 0) {
        pr_err("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&my_cdev, &fops);
    if ((cdev_add(&my_cdev, dev, 1)) < 0) {
        pr_err("Cannot add device to the system\n");
        goto r_class;
    }

    if ((dev_class = class_create(THIS_MODULE, "my_class")) == NULL) {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    if ((device_create(dev_class, NULL, dev, NULL, "my_device")) == NULL) {
        pr_err("Cannot create device\n");
        goto r_device;
    }

    pr_info("Device driver inserted successfully\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

void __exit chr_driver_exit(void) {
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device driver removed successfully\n");
}

module_init(chr_driver_init);
module_exit(chr_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rupert Zoone, zoone.rupert@gmail.com");
MODULE_DESCRIPTION("A Character Device Driver");
