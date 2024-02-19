.section ".text"

.global try_lock

try_lock:
    mov     r1, #0
    swp     r2, r1, [r0]
    mov     r0, r2
    blx     lr 