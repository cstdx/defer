#ifndef DEFER_H
#define DEFER_H

#define GCDEFAULT 16

typedef void (*arg0_t)(void);
typedef void (*arg1_t)(void*);

typedef union {
    arg0_t arg0;
    arg1_t arg1;
} dtor_t;

typedef struct {
    void*  _ptr;
    dtor_t _del;
} __gc_node_t;

#define gcalloc(size)                                                          \
    __gc_node_t  __stack[(size)];                                              \
    __gc_node_t* __sp = __stack;

#define gcclean()                                                              \
    do {                                                                       \
        while (__sp != __stack) {                                              \
            --__sp;                                                            \
            if (__sp->_ptr) {                                                  \
                __sp->_del.arg1(__sp->_ptr);                                   \
            } else {                                                           \
                __sp->_del.arg0();                                             \
            }                                                                  \
        }                                                                      \
    } while (0)

#define gcdefer(del, ptr)                                                      \
    do {                                                                       \
        __sp->_ptr = (void*)(ptr);                                             \
        if (__sp->_ptr) {                                                      \
            __sp->_del.arg1 = (arg1_t)(del);                                   \
        } else {                                                               \
            __sp->_del.arg0 = (arg0_t)(del);                                   \
        }                                                                      \
        ++__sp;                                                                \
    } while (0)

#endif // !DEFER_H
