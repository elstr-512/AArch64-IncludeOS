#include "x264wrapper.h"

static int service_entry() {
  return x264wrapper();
}

int main(int argc, char** argv) {
  return service_entry();
}
