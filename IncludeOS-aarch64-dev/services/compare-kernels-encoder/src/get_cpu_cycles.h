#pragma once

#include <stdint.h>

static inline uint64_t read_cntvct() {
#if defined(__x86_64__)
    uint32_t lo, hi;

    asm volatile(
        "lfence\n\t"
        "rdtsc\n\t"
        : "=a"(lo), "=d"(hi)
        :
        : "memory");

    return ((uint64_t)hi << 32) | lo;

#elif defined(__aarch64__)
    uint64_t cycles;

    asm volatile(
        "isb\n\t"
        "mrs %0, CNTVCT_EL0"
        : "=r"(cycles)
        :
        : "memory");

    return cycles;
#endif
}

static inline uint64_t read_cntfrq() {
  uint64_t freq;

  #if defined(__aarch64__)
  asm volatile("isb");
  asm volatile("mrs %0, CNTFRQ_EL0" : "=r"(freq) :: "memory");
  #endif

  return freq;
}
