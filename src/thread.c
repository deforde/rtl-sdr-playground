#include "thread.h"

void launch_thread(thread_t* thread, thread_func_t func, void* thread_args)
{
    pthread_attr_init(&thread->attr);
    pthread_attr_setdetachstate(&thread->attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread->handle, &thread->attr, func, thread_args);
    pthread_attr_destroy(&thread->attr);
}

void join_thread(thread_t* thread)
{
    void* thread_status;
    pthread_join(thread->handle, &thread_status);
}
