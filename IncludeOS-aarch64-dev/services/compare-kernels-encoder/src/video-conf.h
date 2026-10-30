#pragma once

#include <stdint.h>

struct _video_conf {
  const char *input_file;
  const char *output_file;
  const int height;
  const int width;

  const uint64_t size;

  uint8_t *pmem_pos;
};

extern struct _video_conf *video;
