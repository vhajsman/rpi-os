#include "spinlock.c"

void spin_init(spinLock_t* lock) {
    *lock = 1;
}

void spin_lock(spinLock_t* lock) {
    while (!try_lock(lock));
}

void spin_unlock(spinLock_t* lock) {
    *lock = 1;
}