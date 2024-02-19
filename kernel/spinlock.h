#ifndef __SPINLOCK_H
#define __SPINLOCK_H

typedef int spinLock_t;

void spin_init(spinLock_t * lock);
void spin_lock(spinLock_t * lock);
void spin_unlock(spinLock_t * lock);

uint8_t try_lock(lock);

#endif