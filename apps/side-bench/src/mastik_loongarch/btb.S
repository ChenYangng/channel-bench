#include <autoconf.h>
#include <manager/gen_config.h>
#include <side-bench/gen_config.h>

/*
            ret
       |=== block 1
   spy ||   block 2   /\
       \/   ...       || trojan
            block s ===|
            ...
            ret
*/

/* number of half words per block */
#define JUMP_W_ALIGN CONFIG_BENCH_BTB_ALIGN/4

.global btb_probe
.type btb_probe, @function
.global btb_prime
.type btb_prime, @function

.align 4
/* end of prime */
    jirl $r0, $ra, 0
/* align. ret already takes one HW */
.rept JUMP_W_ALIGN - 1
    addi.w $r0, $r0, 0
.endr
btb_probe:
.rept CONFIG_BENCH_BTB_ENTRIES 
  /* a1 should either be +BTB_ALIGN (probe) or -BTB_ALIGN (prime)
     a0 points to the next (probe) or previous (prime) block */
    add.d $a0, $a0, $a1  // 
    jirl $r0, $a0, 0     // 
  /* align. add and jirl already take two HWs */
    .rept JUMP_W_ALIGN - 2
        addi.w $r0, $r0, 0
    .endr
.endr
btb_prime:
    jirl $r0, $ra, 0
/* end of probe */
