#include <cstdio>
#include <os>
#include <service>
#include <cstdint>

#include "x264wrapper.h"
#include "video-conf.h"

static void dump_head(const uint8_t arr[], size_t size) {
  size_t i = 0;
  for ( ;
  i != size && i < (10 * 16);
  ++i
) {
    if ( i % 16 == 0) { printf("%08zx  ", i); }
    else if ( i % 8 == 0) { printf(" "); }
    printf("%02x%c", arr[i], (i + 1) % 16 ? ' ' : '\n');
  }

  if( i % 16 != 0) {
    puts("");
  }
}

static void dump_tail(const uint8_t arr[], size_t size) {

  size_t i = size - (10 * 16);
  while ( i % 16 != 0) {
    i++;
  }

  for ( ;
  i != size;
  ++i
) {
    if ( i % 16 == 0) { printf("%08zx  ", i); }
    else if ( i % 8 == 0) { printf(" "); }
    printf("%02x%c", arr[i], (i + 1) % 16 ? ' ' : '\n');
  }

  if( i % 16 != 0) {
    puts("");
  }
}

static void embedded_test() {
  printf("Embedded data size: %zu bytes\n", video->size);
  dump_head(video->pmem_pos , video->size);
  puts("");
  dump_tail(video->pmem_pos, video->size);
  puts("");
}

static int service_entry() {
  // embedded_test();
  return x264wrapper();
}

void Service::start(const std::string& args){
  service_entry();
  __arch_poweroff();
  os::shutdown();
}

