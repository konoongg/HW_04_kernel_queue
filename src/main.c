#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "fifo/fifo_ops.h"

#define QUEUE_SIZE 5

static int __init mod_init(void)
{
    pr_info("hello, from kernel \n");
    fifo_result res = fifo_init(QUEUE_SIZE);
    if (res != FIFO_OK)
        pr_err("can't init queue: %s \n", fifo_error_string(res));
    return 0;
}

static void __exit mod_exit(void)
{
    fifo_clear(FIFO_FULL_CLEAR);
    pr_info("bye, from kernel \n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vladislav Akhmedov");
MODULE_DESCRIPTION("Kernel module with queue exmple");
