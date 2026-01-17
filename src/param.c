#include <linux/stat.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>  
#include <linux/module.h> 

#include "./fifo/fifo_ops.h"

static int param_set_enqueue(const char *val, const struct kernel_param *kp)
{
    int value;
    if (kstrtoint(val, 10, &value) != 0)
        return - EINVAL;

    fifo_result res = fifo_enqueue(value);
    if (res != FIFO_OK)
         pr_info("enqueue error: %s \n", fifo_error_string(res));
    return 0;
}

static int param_set_clear(const char *val, const struct kernel_param *kp)
{
    fifo_clear(FIFO_ENTRIES_CLEAR);
    return 0;
}

static int param_get_dequeue(char *buf, const struct kernel_param *kp)
{
    int value;
    fifo_result res = fifo_dequeue(&value);
    if (res != FIFO_OK)
        return sprintf(buf, "dequeue error: %s \n", fifo_error_string(res)); 
    return sprintf(buf, "%d\n", value);
}

static int param_get_peek(char *buf, const struct kernel_param *kp)
{
    int value;
    fifo_result res = fifo_peek(&value);
    if (res != FIFO_OK)
        return sprintf(buf, "dequeue error: %s \n", fifo_error_string(res)); 
    return sprintf(buf, "%d\n", value);
}

static int param_get_size(char *buf, const struct kernel_param *kp)
{
    int value = fifo_size();
    return sprintf(buf, "%d\n", value);
}

static int param_get_avail(char *buf, const struct kernel_param *kp)
{
    int value = fifo_available();
    return sprintf(buf, "%d\n", value);
}

static int param_get_is_empty(char *buf, const struct kernel_param *kp)
{
    bool value = fifo_is_empty();
    return sprintf(buf, "%d\n", value);
}

static int param_get_is_full(char *buf, const struct kernel_param *kp)
{
    bool value = fifo_is_full();
    return sprintf(buf, "%d\n", value);
}

static const struct kernel_param_ops enqueue_params_ops = {
    .set = param_set_enqueue,
};

static const struct kernel_param_ops clear_params_ops = {
    .set = param_set_clear,
};

static const struct kernel_param_ops dequeue_params_ops = {
    .get = param_get_dequeue,
};

static const struct kernel_param_ops peek_params_ops = {
    .get = param_get_peek,
};

static const struct kernel_param_ops size_params_ops = {
    .get = param_get_size,
};

static const struct kernel_param_ops avail_params_ops = {
    .get = param_get_avail,
};

static const struct kernel_param_ops is_empty_params_ops = {
    .get = param_get_is_empty,
};

static const struct kernel_param_ops is_full_params_ops = {
    .get = param_get_is_full,
};

module_param_cb(enqueue, &enqueue_params_ops, NULL, S_IWUSR);
module_param_cb(clear, &clear_params_ops, NULL, S_IWUSR);

module_param_cb(dequeue, &dequeue_params_ops, NULL, S_IRUSR);
module_param_cb(peek, &peek_params_ops, NULL, S_IRUSR);
module_param_cb(size, &size_params_ops, NULL, S_IRUSR);
module_param_cb(avail, &avail_params_ops, NULL, S_IRUSR);
module_param_cb(is_empty, &is_empty_params_ops, NULL, S_IRUSR);
module_param_cb(is_full, &is_full_params_ops, NULL, S_IRUSR);