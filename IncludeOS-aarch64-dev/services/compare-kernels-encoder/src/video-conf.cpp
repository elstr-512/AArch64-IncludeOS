#include "video-conf.h"

#include <stdint.h>

static uint8_t* memory_position_video = reinterpret_cast<uint8_t*>(0x80000000);

static struct _video_conf tractor_yuv = {
  .input_file = "tractor.yuv",
  .output_file = "output.c63",

  .height = 1080,
  .width = 1920,

  .size = 0x7FFFE000UL,

  .pmem_pos = memory_position_video
};

static struct _video_conf foreman_yuv = {
  .input_file = "foreman.yuv",
  .output_file = "output.c63",

  .height = 288,
  .width = 352,

  .size = 0x2B81800,

  .pmem_pos = memory_position_video
};

/*struct _video_conf *video = &foreman_yuv;*/
struct _video_conf *video = &tractor_yuv;
