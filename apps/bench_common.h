/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */




/*The common header file shared by benchmark application 
 and root task (manager process). Mainly containing: 
 data format and hardware configurations for testing.*/


#ifndef _BENCH_COMMON_H 
#define _BENCH_COMMON_H 


#include <stdint.h>
#include <sel4bench/sel4bench.h>
#include <autoconf.h>

/*elf file name of benchmark thread*/ 
#define CONFIG_BENCH_THREAD_NAME    "side-bench"

/*name, ep, option*/
/*num of arguments passed to benchmark tests*/
#define CONFIG_BENCH_ARGS    1

/*num of digits used for maximum unsigned int*/
#define CONFIG_BENCH_MAX_UNIT   10 

/*benchmark thread priority*/
#define CONFIG_BENCH_PRIORITY    100 


/*benchmark communication protocol*/ 
#define BENCH_INIT_MSG      0x11

/*number of shared frames for benchmark recording*/

/*TODO: currently only suits the L1 data benchmark
 which is 16(msg length) * 256 (maximum 8 bits integer) 
 * 64 (number of cache sets) * 8 (timing measurement) = 0x200000 */
#define CONFIG_BENCH_RECORD_PAGES 0x200 

#define BENCH_FAILURE (-1)
#define BENCH_SUCCESS 0

/*cache line size*/
#define CL_SIZE    64 
/**************************************************/
/*prime + probe attack on L1 D cache, in dcache.c*/
/*AES attack, synchrous*/

/*number of probes*/
#define N_D_TESTS   900000
/*all zero bits for aes key value*/
#define AES_KEY_ALL_ZERO 

/*L1 d cache size 32K*/
#define L1D_SIZE  0x8000

/*number of l1 d cache sets: 64*/
#define N_L1D_SETS   64
/*number of l1 d cache ways*/
#define N_L1D_WAYS   8 
/*bit masks used for recording connected cache lines*/
#define N_L1D_WAY_BITMASK 0xff 

/*size of an entire L1 D set: 4096 */
#define L1D_SET_SIZE  0x1000
/*AES msg size, 128 bits 
 key size 128 bits*/
#define AES_MSG_SIZE  16
#define AES_KEY_SIZE  16
/*aes service size 128 bits block */
#define AES_BITS      128

/*number of possible plaintext values in each byte*/
#define N_PT_B 256   

/*for cache flushing benchmark*/
#define BENCH_CACHE_FLUSH_RUNS    10

/*ipc benchmark, same as sel4bench*/

#define NLATENCY  16 
#define IPC_RUNS    32
#define IPC_WARMUPS 32
#define IPC_OVERHEAD_RETRIES  4 

#define IPC_PROCESS_PRIO_LOW 50 
#define IPC_PROCESS_PRIO_HIGH 100 

#define IPC_PROCESS_PRIO 100 

/*maximum number of benchmarking thread in the system*/ 
#define MAX_BENCH_THREADS  2


/*dividing cache colours into security domains*/
#define CC_NUM_DOMAINS     2

#ifdef CONFIG_ARCH_X86 
/*spliting according to the L2 cache colours, total 8 colours*/
#define CC_DIV             4
#endif  /*x86*/

#ifdef CONFIG_PLAT_HIKEY
#define CC_DIV             4
#endif

#ifdef CONFIG_PLAT_IMX6
#define CC_DIV             8
#endif
#ifdef CONFIG_PLAT_EXYNOS4
#define CC_DIV             8
#endif
#ifdef CONFIG_PLAT_EXYNOS54XX
#define CC_DIV             8
#endif


/*running the capacity benchmark*/ 
#define BENCH_CAP_SINGLE  1  /*running the benchmark on single core*/
#define BENCH_CAP_SLAVE 1

/*ipc bench serial number*/
enum ipc_funs{
    IPC_CALL, 
    IPC_CALL2, 
    IPC_CALL_10, 
    IPC_CALL2_10, 
    IPC_REPLY_WAIT, 
    IPC_REPLY_WAIT2, 
    IPC_REPLY_WAIT_10, 
    IPC_REPLY_WAIT2_10, 
    IPC_WAIT, 
    IPC_SEND, 
    IPC_RT_CALL, 
    IPC_RT_REPLY_WAIT,
    IPC_LATENCY_CALL, 
    IPC_LATENCY_REPLY_WAIT,
    IPC_OVERHEAD, 
    IPC_ALL
};

#ifdef CONFIG_ARCH_X86 
#define BENCH_PMU_BITS 0x1
#define BENCH_PMU_COUNTERS 1
#endif 

#ifdef CONFIG_ARM_CORTEX_A9
#define BENCH_PMU_BITS 0x1
#define BENCH_PMU_COUNTERS 1
#endif 

#ifdef CONFIG_ARM_CORTEX_A15
#define BENCH_PMU_BITS 0x3
#define BENCH_PMU_COUNTERS 2 
#endif 

#ifdef CONFIG_ARM_CORTEX_A53
#define BENCH_PMU_BITS  0x1
#define BENCH_PMU_COUNTERS 1 
#endif 


#define BENCH_RECORD_PAGES    1
#define BENCH_COVERT_BUF_PAGES  4096 /*trojan/probe buffers*/

/*the page used for functional test correctness*/
#define BENCH_FUNC_TEST_PAGES    1

#define BENCH_MORECORE_HUGE_SIZE  (16 * 1024 * 1024) /* huge pages created by master for benchmarking thread */



#define BENCH_PAGE_SIZE  4096

/*opt for running covert channel bench*/
#define BENCH_COVERT_L2_TROJAN  1 
#define BENCH_COVERT_L2_SPY   2
#define BENCH_MASTIK_TEST    3
#define BENCH_MASTIK_VICTIM  4
#define BENCH_MPI_VICTIM     5
#define BENCH_MASTIK_SPY     6
#define BENCH_COVERT_L1D_TROJAN    7 
#define BENCH_COVERT_L1D_SPY       8 
#define BENCH_COVERT_L1I_TROJAN    9 
#define BENCH_COVERT_L1I_SPY       10
#define BENCH_COVERT_LLC_KERNEL_TROJAN   11 
#define BENCH_COVERT_LLC_KERNEL_SPY      12 
#define BENCH_COVERT_LLC_KD_TROJAN       13   /*kernel determinisitic scheduling*/
#define BENCH_COVERT_LLC_KD_SPY          14 
#define BENCH_COVERT_TLB_TROJAN          15 
#define BENCH_COVERT_TLB_SPY             16
#define BENCH_COVERT_BRANCH_TROJAN       17 
#define BENCH_COVERT_BRANCH_SPY          18
#define BENCH_COVERT_LLC_SINGLE_TROJAN   19 
#define BENCH_COVERT_LLC_SINGLE_SPY      20

#define BENCH_COVERT_BP_TROJAN		 21
#define BENCH_COVERT_BP_SPY              22
#define BENCH_COVERT_TIMER_HIGH          23 
#define BENCH_COVERT_TIMER_LOW           24
#define BENCH_COVERT_FUNS                25


#define BENCH_CACHE_FLUSH_FUN_START      100
#define BENCH_CACHE_FLUSH_L1D            100 
#define BENCH_CACHE_FLUSH_L1I            101 
#define BENCH_CACHE_FLUSH_LLC            102
#define BENCH_CACHE_FLUSH_IDLE           103 
#define BENCH_CACHE_FLUSH_FUNS           4

#ifdef CONFIG_BENCH_CACHE_FLUSH_L1_CACHES 
#define BENCH_FLUSH_THREAD_NUM        BENCH_CACHE_FLUSH_L1D 
#endif 

#ifdef CONFIG_BENCH_CACHE_FLUSH_L1_CACHES_INSTRUCTION 
#define BENCH_FLUSH_THREAD_NUM       BENCH_CACHE_FLUSH_L1I 
#endif 

#ifdef CONFIG_BENCH_CACHE_FLUSH_ALL_CACHES 
#define BENCH_FLUSH_THREAD_NUM      BENCH_CACHE_FLUSH_LLC 
#endif 

#ifdef CONFIG_MANAGER_CACHE_FLUSH 
#define BENCH_IDLE_THREAD_NUM        BENCH_CACHE_FLUSH_IDLE
#endif 


/*ipc round trip performance*/
#ifdef CONFIG_MANAGER_IPC
#define BENCH_FLUSH_THREAD_NUM          IPC_RT_CALL 
#define BENCH_IDLE_THREAD_NUM           IPC_RT_REPLY_WAIT 
#endif 


/*the benchmarking tets for the function correctness */
#define BENCH_FUNC_RECEIVER              80 
#define BENCH_FUNC_SENDER                81 

#define BENCH_COVERT_MSG_LEN  3 /*msg len for init env*/
/*matching the test number according to the config*/ 
#ifdef CONFIG_BENCH_COVERT_L1D 
#define BENCH_COVERT_TROJAN    BENCH_COVERT_L1D_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_L1D_SPY 
#endif 
#ifdef CONFIG_BENCH_COVERT_L1I
#define BENCH_COVERT_TROJAN    BENCH_COVERT_L1I_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_L1I_SPY 
#endif 
#ifdef CONFIG_BENCH_COVERT_L2 
#define BENCH_COVERT_TROJAN    BENCH_COVERT_L2_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_L2_SPY 
#endif 
#ifdef CONFIG_BENCH_COVERT_LLC_KERNEL  /*LLC channel through shared kernel*/
#define BENCH_COVERT_TROJAN    BENCH_COVERT_LLC_KERNEL_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_LLC_KERNEL_SPY 
#endif 




/*LLC covert channel, single core*/
#ifdef CONFIG_BENCH_COVERT_LLC 
#define BENCH_COVERT_TROJAN    BENCH_COVERT_LLC_SINGLE_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_LLC_SINGLE_SPY 
#endif 


/*the tlb covert channel*/
#ifdef CONFIG_BENCH_COVERT_TLB
#define BENCH_COVERT_TROJAN    BENCH_COVERT_TLB_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_TLB_SPY 
#endif 

/*the branch covert channel*/
#ifdef CONFIG_BENCH_COVERT_BTB
#define BENCH_COVERT_TROJAN    BENCH_COVERT_BRANCH_TROJAN 
#define BENCH_COVERT_SPY       BENCH_COVERT_BRANCH_SPY 
#endif 


/*LLC channel through kernel scheduling*/
#ifdef CONFIG_BENCH_COVERT_LLC_KERNEL_SCHEDULE 
#define BENCH_COVERT_TROJAN      BENCH_COVERT_LLC_KD_TROJAN
#define BENCH_COVERT_SPY         BENCH_COVERT_LLC_KD_SPY
#endif

#ifdef CONFIG_MASTIK_ATTACK_COVERT
#define BENCH_COVERT_SPY       BENCH_MASTIK_TEST
#define BENCH_COVERT_TROJAN    BENCH_MASTIK_VICTIM 
#endif
    /*the demo*/
#ifdef CONFIG_MASTIK_ATTACK_SIDE 
#define BENCH_COVERT_SPY       BENCH_MASTIK_SPY
#define BENCH_COVERT_TROJAN    BENCH_MPI_VICTIM
#endif 

#ifdef CONFIG_BENCH_COVERT_BP
#define BENCH_COVERT_TROJAN      BENCH_COVERT_BP_TROJAN
#define BENCH_COVERT_SPY         BENCH_COVERT_BP_SPY
#endif

#ifdef CONFIG_BENCH_COVERT_TIMER 
#define BENCH_COVERT_TROJAN     BENCH_COVERT_TIMER_HIGH 
#define BENCH_COVERT_SPY        BENCH_COVERT_TIMER_LOW 
#endif 


/*used by kernel determinsitic scheduling benchmark*/
#ifdef CONFIG_LIB_SEL4_CACHECOLOURING
/*if cache colouring enabled, the number of cache sets that high uses doubles*/
#define NUM_LLC_CACHE_SETS   4096
#else
#define NUM_LLC_CACHE_SETS   2048 
#endif
/*number of cache sets visits for trojan to warm up the platform
 total 4096 cache sets, trying 0x16 rounds on each sets*/
#define NUM_KD_WARMUP_ROUNDS   0x16000

#ifndef CONFIG_BENCH_DATA_POINTS 
#define CONFIG_BENCH_DATA_POINTS  1
#endif 

#define SYSTEM_TICK_SYN_FLAG   0xffff
#define SPY_SYN_FLAG           0x87654321
#define TROJAN_SYN_FLAG        0x12345678


#define NUM_L1D_SHARED_PAGE  1

/*one system tick is 1ms, 3400000 cycles, 3.4GHZ sandybridge machine*/
/*ARM V7 tick length 1ms 800 MHZ*/
#define KERNEL_SCHEDULE_TICK_LENGTH   100000

#ifdef CONFIG_ARCH_X86
static inline uint64_t rdtscp_64(void) {
    uint32_t low, high;

    asm volatile ( 
            "rdtscp          \n"
            "movl %%edx, %0 \n"
            "movl %%eax, %1 \n"
            : "=r" (high), "=r" (low)
            :
            : "eax", "ecx", "edx");

    return ((uint64_t) high) << 32llu | (uint64_t) low;
}

#endif

static inline int wait_msg(seL4_CPtr ep) {
    seL4_MessageInfo_t info;

    info = seL4_Recv(ep, NULL);
    if (seL4_MessageInfo_get_label(info) != seL4_Fault_NullFault)
       return BENCH_FAILURE;

    return BENCH_SUCCESS;

}

static inline seL4_CPtr wait_msg_from(seL4_CPtr endpoint)
{
    /* wait for a message */
    seL4_Word badge;
    seL4_MessageInfo_t info;

    info = seL4_Recv(endpoint, &badge);

    /* check the label and length*/
    assert(seL4_MessageInfo_get_label(info) == seL4_Fault_NullFault);
    assert(seL4_MessageInfo_get_length(info) == 1);

    return (seL4_CPtr)seL4_GetMR(0);
}



/*sandy bridge machine frequency 3.4GHZ*/
#define MASTIK_FEQ  (3400000000ull)

/*the sandy bridge frequency in microsecond 3.4MHZ*/
#define CPU_FEQ_MICROSEC (3400000ull) 

#define COMPILER_BARRIER do { asm volatile ("" ::: "memory"); } while(0);

/*the warmup rounds for the timing channel benchmarks*/
#define BENCH_TIMING_WARMUPS    10 

#endif 



