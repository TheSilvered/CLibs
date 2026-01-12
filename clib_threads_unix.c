#include <errno.h>
#include "clib_threads.h"

bool threadCreate(Thread *thread, ThreadRoutine routine, void *arg) {
    return (errno = pthread_create(thread, NULL, routine, arg)) == 0;
}

ThreadID threadGetCurrID(void) {
    return pthread_self();
}

bool threadIDEq(ThreadID id1, ThreadID id2) {
    return pthread_equal(id1, id2);
}

bool threadJoin(Thread thread, ThreadRet *status) {
    return (errno = pthread_join(thread, status)) == 0;
}

void threadExit(ThreadRet status) {
    pthread_exit(status);
}

bool threadMutexInit(ThreadMutex *mutex) {
    return (errno = pthread_mutex_init(mutex, NULL)) == 0;
}

bool threadMutexDestroy(ThreadMutex *mutex) {
    return (errno = pthread_mutex_destroy(mutex)) == 0;
}

bool threadMutexLock(ThreadMutex *mutex) {
    return (errno = pthread_mutex_lock(mutex)) == 0;
}

ThreadLockResult threadMutexTryLock(ThreadMutex *mutex) {
    int error = pthread_mutex_trylock(mutex);
    switch (error) {
    case 0:
        return ThreadLockResult_success;
    case EBUSY:
        return ThreadLockResult_busy;
    default:
        errno = error;
        return ThreadLockResult_error;
    }
}

bool threadMutexUnlock(ThreadMutex *mutex) {
    return (errno = pthread_mutex_unlock(mutex)) == 0;
}
