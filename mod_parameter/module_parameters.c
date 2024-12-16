#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static char *whom = "Family";
static int howMany = 1;

module_param(howMany, int, S_IRUGO);
module_param(whom,  charp, S_IRUGO);

static int __init m_init(void){
    pr_debug("Parameter test module is loaded...\n");

    for(int i = 0; i < howMany; i++){
        pr_info("#%d Hello, %s\n", i, whom);
    }
    return 0;
}

static void __exit m_exit(void){
    pr_debug("Parameters test module is unloaded...\n");
}

module_init(m_init);
module_exit(m_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rupert Zoone");
MODULE_DESCRIPTION("Module parameter test program");
