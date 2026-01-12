#include <assert.h>
#include "clib_threads.h"

bool threadCreate(Thread *thread, ThreadRoutine routine, void *arg) {
    HANDLE handle = CreateThread(NULL, 0, routine, arg, 0, NULL);
    if (handle == NULL) {
        return false;
    }
    *thread = handle;
    return true;
}

ThreadID threadGetCurrID(void) {
    return GetCurrentThreadId();
}

bool threadIDEq(ThreadID id1, ThreadID id2) {
    return id1 == id2;
}

bool threadJoin(Thread thread, ThreadRet *status) {
    // The only possible return values are WAIT_FAILED and WAIT_OBJECT_0,
    // WAIT_ABANDONED is impossible because it is not a mutex and
    // WAIT_TIMEOUT is impossible because an infinite time is allowed
    if (WaitForSingleObject(thread, INFINITE) == WAIT_FAILED) {
        return false;
    }

    if (status != NULL && GetExitCodeThread(thread, status) == 0) {
        CloseHandle(thread);
        return false;
    }
    CloseHandle(thread);
    return true;
}

void threadExit(ThreadRet status) {
    ExitThread(status);
}

bool threadMutexInit(ThreadMutex *mutex) {
    InitializeSRWLock(mutex);
    return true;
}

bool threadMutexDestroy(ThreadMutex *mutex) {
    return true; // no destructor for SRWLOCK
}

bool threadMutexLock(ThreadMutex *mutex) {
    AcquireSRWLockExclusive(mutex);
    return true;
}

ThreadLockResult threadMutexTryLock(ThreadMutex *mutex) {
    if (TryAcquireSRWLockExclusive(mutex)) {
        return ThreadLockResult_success;
    } else {
        return ThreadLockResult_busy;
    }
}

bool threadMutexUnlock(ThreadMutex *mutex) {
    ReleaseSRWLockExclusive(mutex);
    return true;
}
