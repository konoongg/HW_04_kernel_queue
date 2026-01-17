#ifndef FIFO_OPS_H
#define FIFO_OPS_H

typedef enum fifo_result fifo_result;


enum fifo_result {
    FIFO_OK = 0,
    FIFO_EMPTY = -1,
    FIFO_FULL = -2,
    FIFO_NOMEM = -3,
    FIFO_INVALID = -4,
    FIFO_INCOR_STATE = -5,
    FIFO_INTERNAL_ERROR = -6,
};

#define FIFO_FULL_CLEAR true
#define FIFO_ENTRIES_CLEAR false

extern fifo_result fifo_init(int size);
extern fifo_result fifo_enqueue(int value);
extern fifo_result fifo_dequeue(int *result);
extern fifo_result fifo_peek(int *result);
extern bool fifo_is_empty(void);
extern bool fifo_is_full(void);
extern int fifo_size(void);
extern int fifo_available(void);
extern void fifo_clear(bool full_clear);
extern const char* fifo_error_string(fifo_result err);

#endif