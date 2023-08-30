#include <linux/module.h>
#include <linux/init.h>

/*Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Geek Majhi GNU/LINUX");
MODULE_DESCRIPTION("A Hello World! from LKM");

/*
 * @brief This function is called, when the module id loaded into the kernel
 */

static int __init ModuleInit(void){
    printk("Hello, Kernel!\n");
    return 0;
}

/*
 *@brief This function is called, when the module is removed from the Kernel
 */
static void __exit ModuleExit(void){
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
