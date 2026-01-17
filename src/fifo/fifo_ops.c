#include <linux/slab.h> 
#include <linux/kfifo.h> 

#include "fifo_ops.h"

/* private structs */

typedef struct fifo_device fifo_device;
typedef struct fifo_entry fifo_entry;

struct fifo_entry {
    int data;
};

struct fifo_device {
    DECLARE_KFIFO_PTR(fifo, fifo_entry);
    int max_size;
};

/* private structs end */

static fifo_device *fifo_dev = NULL;

extern fifo_result 
fifo_init(int size) {
    int ret;

    if (fifo_dev != NULL)
        return FIFO_INCOR_STATE;

    if (size <= 0)
        return FIFO_INVALID;

    fifo_dev = kmalloc(sizeof(fifo_device), GFP_KERNEL);
    if (!fifo_dev)
        return FIFO_NOMEM;

    ret = kfifo_alloc(&(fifo_dev->fifo), size, GFP_KERNEL);
    if (ret != 0) {
        kfree(fifo_dev);
        fifo_dev = NULL;
        return FIFO_NOMEM;
    }

    fifo_dev->max_size = size;
    return FIFO_OK;       
}

extern bool
fifo_is_full(void) {
    if (!fifo_dev)
        return false;
    return kfifo_is_full(&(fifo_dev->fifo));
}

extern bool
fifo_is_empty(void) {
    if (!fifo_dev)
        return true;

    return kfifo_is_empty(&(fifo_dev->fifo));
}

extern fifo_result 
fifo_enqueue(int value) {

    if (!fifo_dev)
        return FIFO_INCOR_STATE;

    if (fifo_is_full())
        return FIFO_FULL;

    fifo_entry new_elem;
    new_elem.data = value;

    if (!kfifo_put(&(fifo_dev->fifo), new_elem))
        return FIFO_INTERNAL_ERROR;

    return FIFO_OK;
}

extern fifo_result 
fifo_dequeue(int *result) {

    if (!fifo_dev)
        return FIFO_INCOR_STATE;

    if(fifo_is_empty())
        return FIFO_EMPTY;

    fifo_entry entry;
    if (!kfifo_get(&(fifo_dev->fifo), &entry))
        return FIFO_INTERNAL_ERROR;

    *result = entry.data;

    return FIFO_OK;
}

extern fifo_result 
fifo_peek(int *result) {
    if (!fifo_dev)
        return FIFO_INCOR_STATE;

    if(fifo_is_empty())
        return FIFO_EMPTY;

    fifo_entry entry;
    if (!kfifo_peek(&(fifo_dev->fifo), &entry))
        return FIFO_INTERNAL_ERROR;

    *result = entry.data;

    return FIFO_OK;
}

extern int 
fifo_size(void) {
    if (!fifo_dev)
        return 0;

    return kfifo_len(&(fifo_dev->fifo));
}

#define kfifo_avail_simple(fifo) \
({ \
    typeof((fifo) + 1) __tmpq = (fifo); \
    kfifo_size(__tmpq) - kfifo_len(__tmpq) ; \
})

extern int 
fifo_available(void) {
    if (!fifo_dev)
        return 0;

    /* in our case kfifo->recsize is equal 0*/
    return kfifo_avail_simple(&(fifo_dev->fifo));
}

extern void 
fifo_clear(bool full_clear) {

    if (full_clear && !fifo_dev)
        return;

    kfifo_reset(&fifo_dev->fifo);

    if (full_clear) {
        kfifo_free(&(fifo_dev->fifo));
        kfree(fifo_dev);
        fifo_dev = NULL;
    }
}

extern const char* 
fifo_error_string(fifo_result err) {
    switch (err) {
        case FIFO_OK:
            return "Success";
        case FIFO_EMPTY:
            return "Queue is empty";
        case FIFO_FULL:
            return "Queue is full";
        case FIFO_NOMEM:
            return "Memory allocation failed";
        case FIFO_INVALID:
            return "Invalid parameter";
        case FIFO_INCOR_STATE:
            return "Incorrect queue state";
        case FIFO_INTERNAL_ERROR:
            return "Internal error";
        default:
            return "Unknown error";
    }
}
