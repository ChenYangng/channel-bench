#include <autoconf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sel4/sel4.h>

#include "low.h"
#include "l1.h"
#include "bench_common.h"
#include "bench_types.h"



#define TS_THRESHOLD 10000

static void  newTimeSlice(){
  asm("");
  uint32_t prev = rdtscp();
  for (;;) {
    uint32_t cur = rdtscp();
    if (cur - prev > TS_THRESHOLD)
      return;
    prev = cur;
  }
}

static void access_buffer(char *buffer, uint32_t sets) {

    uint32_t offset; 

    for (int i = 0; i < sets; i++) {
        for (int j = 0; j < L1_ASSOCIATIVITY; j++) { 
            offset = i * L1_CACHELINE + j * L1_STRIDE; 
            access(buffer + offset);
        }
    }
}

int l1_trojan(bench_env_t *env) {

    seL4_MessageInfo_t info;


    /*buffer size 32K L1 cache size
      512 cache lines*/
    char *data = malloc(L1_PROBE_BUFFER);
    int secret = 0; 

    bench_args_t *args = env->args; 

    uint32_t volatile *share_vaddr = args->shared_vaddr; 
    *share_vaddr = SYSTEM_TICK_SYN_FLAG; 

    /*manager: trojan is ready*/
    info = seL4_MessageInfo_new(seL4_Fault_NullFault, 0, 0, 1);
    seL4_SetMR(0, 0); 
    seL4_Send(args->r_ep, info);

    /*syn with spy*/
    seL4_Send(args->ep, info);


    for (int i = 0; i < CONFIG_BENCH_DATA_POINTS; i++) {

        /*waiting for a system tick*/
        newTimeSlice();
        secret = random() % (L1_SETS + 1);

        access_buffer(data, secret);

        /*update the secret read by low*/ 
        *share_vaddr = secret; 

    }
    while (1);

    return 0;
}

int l1_spy(bench_env_t *env) {
    seL4_Word badge;
    seL4_MessageInfo_t info;

#ifdef CONFIG_BENCH_COVERT_L1D 
    uint64_t monitored_mask[1] = {~0LLU};
#else 
    /*for the L2 cache attack, with larger cache set*/
    uint64_t monitored_mask[8] = {~0LLU, ~0LLU,~0LLU, ~0LLU,
        ~0LLU, ~0LLU,~0LLU, ~0LLU,};
#endif 

#ifdef CONFIG_x86_64
    uint64_t UNUSED pmu_start[BENCH_PMU_COUNTERS]; 
    uint64_t UNUSED pmu_end[BENCH_PMU_COUNTERS]; 
#else
    uint32_t UNUSED pmu_start[BENCH_PMU_COUNTERS]; 
    uint32_t UNUSED pmu_end[BENCH_PMU_COUNTERS]; 
#endif

    bench_args_t *args = env->args; 

    l1info_t l1_1 = l1_prepare(monitored_mask);
    uint16_t *results = malloc(l1_nsets(l1_1)*sizeof(uint16_t));


    /*the record address*/
    struct bench_l1 *r_addr = (struct bench_l1 *)args->record_vaddr;
    /*the shared address*/
    uint32_t volatile *secret = (uint32_t *)args->shared_vaddr; 

    /*syn with trojan*/
    info = seL4_Recv(args->ep, &badge);
    assert(seL4_MessageInfo_get_label(info) == seL4_Fault_NullFault);

    /*waiting for a start*/
    while (*secret == SYSTEM_TICK_SYN_FLAG) ;


    for (int i = 0; i < CONFIG_BENCH_DATA_POINTS; i++) {

        newTimeSlice();

#ifdef CONFIG_MANAGER_PMU_COUNTER
        sel4bench_get_counters(BENCH_PMU_BITS, pmu_start);  
#endif 

        l1_probe(l1_1, results);

#ifdef CONFIG_MANAGER_PMU_COUNTER 
        sel4bench_get_counters(BENCH_PMU_BITS, pmu_end);
        /*loading the pmu counter value */
        for (int counter = 0; counter < BENCH_PMU_COUNTERS; counter++ )
            r_addr->pmu[i][counter] = pmu_end[counter] - pmu_start[counter]; 

#endif

        /*result is the total probing cost
          secret is updated by trojan in the previous system tick*/
        r_addr->result[i] = 0; 
        r_addr->sec[i] = *secret; 

        for (int j = 0; j < l1_nsets(l1_1); j++) 
            r_addr->result[i] += results[j];

    }

    /*send result to manager, spy is done*/
    info = seL4_MessageInfo_new(seL4_Fault_NullFault, 0, 0, 1);
    seL4_SetMR(0, 0);
    seL4_Send(args->r_ep, info);

    while (1);

    return 0;
}
