#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

typedef void* (*thread_func_t)(void*);

typedef struct thread_t {
    pthread_attr_t attr;
    pthread_t handle;
} thread_t;

void launch_thread(thread_t* thread, thread_func_t func, void* thread_args);

void join_thread(thread_t* thread);

#endif //THREAD_H
