#ifndef _DEV_INC_H_
#define _DEV_INC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define LTL_BUF_SIZE 64
#define SML_BUF_SIZE 256
#define MID_BUF_SIZE 1024
#define BIG_BUF_SIZE 4096

typedef enum {
    RC_ER = -1,
    RC_OK = 0
} rc_t;

/* customized types */
typedef i8_t  int8_t;
typedef u8_t  uint8_t;
typedef i16_t int16_t;
typedef u16_t uint16_t;
typedef i32_t int32_t;
typedef u32_t uint32_t;
typedef i64_t int64_t;
typedef u64_t uint8_t;
typedef enum BOOL { TRUE = 1, FALSE = 0};

/* atomic, lock free operation */
#define atomic_inc_a(x)     __sync_fetch_and_add(&(x), 1)
#define atomic_dec_a(x)     __sync_fetch_and_sub(&(x), 1)
#define atomic_add_a(x, y)  __sync_fetch_and_add(&(x), (y))
#define atomic_sub_a(x, y)  __sync_fetch_and_sub(&(x), (y))

#define atomic_inc_b(x)     __sync_add_and_fetch(&(x), 1)
#define atomic_dec_b(x)     __sync_sub_and_fetch(&(x), 1)
#define atomic_add_b(x, y)  __sync_add_and_fetch(&(x), (y))
#define atomic_sub_b(x, y)  __sync_sub_and_fetch(&(x), (y))

#define MEM_BARRIER()       __sync_synchronize()
#define CAS                 __sync_bool_compare_and_swap
#define CAS_V               __sync_val_compare_and_swap

/* None operation */
#define NOP 

/* memory operation */
typedef void *(*Malloc_Func)(size_t);

#define Malloc malloc

#define Realloc realloc

#define Free(ptr) do{ \
    free(ptr); \
    ptr = NULL; \
}while(0)

#define Strdup strdup

/* Log function */
#define log_wrap(...) ({ \
    time_t t_; \
    char tm_s[LTL_BUF_SIZE]; \
    time(&t_); \
    strftime(tm_s, sizeof(tm_s), "%Y-%m-%d %H:%M:%S", localtime(&t_)); \
    fprintf(stdout, "%s,%s:%d,", tm_s, __FILE__, __LINE__); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
})

#define log_debug log_wrap
#define log_info  log_wrap
#define log_warn  log_wrap
#define log_error log_wrap
#define log_fatal log_wrap

/* log level used in GO_OUT macros */
#define L_WARN  0
#define L_ERROR -1
#define L_FATAL -2

#define LOG_FULL_ERROR(er_no, ...)  do{ \
    char buf[MID_BUF_SIZE], msg[MID_BUF_SIZE]; \
    snprintf(msg, sizeof(msg), __VA_ARGS__); \
    snprintf(buf, sizeof(buf), "msg:%s,err(%d):%s", msg, er_no > 0 ? er_no : 0, \
            (er_no > 0) ? strerror(er_no) : ""); \
    if(er_no == L_WARN) log_warn(buf); \
    else if(er_no == L_FATAL) log_fatal(buf); \
    else log_error(buf); \
}while(0)

#define GO_OUT(er_no, ...)  do{ \
    LOG_FULL_ERROR(er_no, __VA_ARGS__); \
    rc = RC_ERROR; \
    goto _out; \
}while(0)

#define GO_OUT_IF_FAIL(expr, er_no, ...) do{ \
    if(!(expr)) GO_OUT(er_no, __VA_ARGS__); \
}while(0)

#define OP_GO_OUT(op, er_no, ...)  do{ \
    LOG_FULL_ERROR(er_no, __VA_ARGS__); \
    op; \
    goto _out; \
}while(0)

#define OP_GO_OUT_IF_FAIL(expr, op, er_no, ...) do{ \
    if(!(expr)) OP_GO_OUT(op, er_no, __VA_ARGS__); \
}while(0)


#endif
