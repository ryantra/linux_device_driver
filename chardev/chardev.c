#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h> // For kmalloc and kfree

#define DEVICE_NAME "mychardev"

static char *device_buffer;
static size_t buffer_size = 1024; // Initial buffer size
static size_t data_size = 0;      // Amount of data written
static dev_t dev_number;
static struct cdev my_cdev;
static int open_count = 0;

/* Open device */
static int chardev_open(struct inode *inode, struct file *file) {
    open_count++;
    pr_info("%s: Device opened %d times\n", DEVICE_NAME, open_count);
    return 0;
}

/* Release device */
static int chardev_release(struct inode *inode, struct file *file) {
    pr_info("%s: Device closed\n", DEVICE_NAME);
    return 0;
}

/* Read from device */
static ssize_t chardev_read(struct file *file, char __user *buf, size_t len, loff_t *offset) {
    size_t bytes_to_read;

    if (*offset >= data_size) {
        pr_info("%s: Nothing to read\n", DEVICE_NAME);
        return 0;
    }

    bytes_to_read = min(len, data_size - *offset);

    if (copy_to_user(buf, device_buffer + *offset, bytes_to_read)) {
        pr_err("%s: Failed to copy to user\n", DEVICE_NAME);
        return -EFAULT;
    }

    *offset += bytes_to_read;
    pr_info("%s: Read %zu bytes\n", DEVICE_NAME, bytes_to_read);
    return bytes_to_read;
}

/* Write to device */
static ssize_t chardev_write(struct file *file, const char __user *buf, size_t len, loff_t *offset) {
    size_t new_size;

    // Calculate the new size of the buffer if necessary
    new_size = *offset + len;
    if (new_size > buffer_size) {
        char *new_buffer = krealloc(device_buffer, new_size, GFP_KERNEL);
        if (!new_buffer) {
            pr_err("%s: Failed to allocate memory\n", DEVICE_NAME);
            return -ENOMEM;
        }
        device_buffer = new_buffer;
        buffer_size = new_size;
    }

    if (copy_from_user(device_buffer + *offset, buf, len)) {
        pr_err("%s: Failed to copy from user\n", DEVICE_NAME);
        return -EFAULT;
    }

    *offset += len;
    data_size = max_t(size_t, data_size, *offset); // Update the amount of data written
    pr_info("%s: Written %zu bytes\n", DEVICE_NAME, len);
    return len;
}

/* File operations */
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = chardev_open,
    .release = chardev_release,
    .read = chardev_read,
    .write = chardev_write,
};

/* Module initialization */
static int __init chardev_init(void) {
    int ret;

    /* Allocate major and minor numbers dynamically */
    ret = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("%s: Failed to allocate major number\n", DEVICE_NAME);
        return ret;
    }

    /* Initialize character device */
    cdev_init(&my_cdev, &fops);
    my_cdev.owner = THIS_MODULE;

    /* Add the character device to the system */
    ret = cdev_add(&my_cdev, dev_number, 1);
    if (ret < 0) {
        pr_err("%s: Failed to add cdev\n", DEVICE_NAME);
        unregister_chrdev_region(dev_number, 1);
        return ret;
    }

    /* Allocate initial device buffer */
    device_buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!device_buffer) {
        pr_err("%s: Failed to allocate memory for buffer\n", DEVICE_NAME);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, 1);
        return -ENOMEM;
    }

    pr_info("%s: Device initialized with major %d and minor %d\n", DEVICE_NAME, MAJOR(dev_number), MINOR(dev_number));
    return 0;
}

/* Module cleanup */
static void __exit chardev_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number, 1);
    kfree(device_buffer); // Free allocated memory
    pr_info("%s: Device removed\n", DEVICE_NAME);
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rupesh Majhi");
MODULE_DESCRIPTION("A character device driver with dynamic buffer resizing");
