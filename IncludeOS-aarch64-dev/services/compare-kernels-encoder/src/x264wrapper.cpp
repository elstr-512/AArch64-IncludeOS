
#include <cstdint>
#include <cstdio>
#include <stdio.h>

#ifndef INCLUDEOS
#include <sys/mman.h>
#include <fcntl.h>
#include <string>
#endif

#ifdef LINUX_VM
#include <cstddef>
#include <sys/mount.h>
#include <unistd.h>
#endif

extern "C" {
  #include <stdint.h>
  #include <x264.h>
}

#include "memfile.h"
#include "get_cpu_cycles.h"
#include "video-conf.h"

const int width = video->width;
const int height = video->height;

int x264wrapper() {

#ifdef LINUX_NATIVE
  printf("\n[x264 LINUX_NATIVE]\n\n");
#endif

#ifdef INCLUDEOS
  printf("\n[x264 INCLUDEOS]\n\n");
#endif

#ifdef LINUX_VM
  printf("\n [x264 LINUX_VM]\n\n");
#endif

  x264_param_t param;
  x264_picture_t pic;
  x264_picture_t pic_out;
  x264_t *h;
  int i_frame = 0;
  int i_frame_size;
  x264_nal_t *nal;
  int i_nal;

  /* Get default params for preset/tuning */
  if( x264_param_default_preset( &param, "medium", NULL ) < 0 )
    exit(1);

  /* Configure non-default params */
  param.i_csp = X264_CSP_I420;
  param.i_width  = width;
  param.i_height = height;

  param.i_threads = 1;          // single-threaded
  param.b_sliced_threads = 0;   // single-threaded, frame-based

  param.b_vfr_input = 0;
  param.b_repeat_headers = 1;
  param.b_annexb = 1;

  /* Apply profile restrictions. */
  if( x264_param_apply_profile( &param, "high" ) < 0 ) {
    exit(1);
  }

  if( x264_picture_alloc( &pic, param.i_csp, param.i_width, param.i_height ) < 0 ) {
    exit(1);
  }

  h = x264_encoder_open( &param );
  if( !h ) {
    exit(1);
  }

  struct MemFile *infile = MEMFILE_IN;
  struct MemFile *outfile = MEMFILE_OUT;

  infile->pos = 0;
  infile->eof = false;
  infile->error = false;

  outfile->pos = 0;
  outfile->eof = false;
  outfile->error = false;

  #ifndef INCLUDEOS

  #ifdef LINUX_VM
  int mount_err = mount("/dev/vda", "/mnt", "ext4", 0, NULL);
  while(mount_err != 0) {

    mount_err = mount("/dev/vda", "/mnt", "ext4", 0, NULL);
    umount("/mnt");

    printf("Error: Failed to mount Reason: [%d]\n", mount_err);
    sleep(2);
  }
  printf("MOUNTED\n");

  std::string pre_path = "/mnt/";

  #else
  std::string pre_path = "./videos-yuv/";
  #endif

  std::string path = pre_path.append(video->input_file);

  int fd = open(path.c_str(), O_RDONLY);

  if(fd == -1) {
    exit(123);
  }

  printf("[mmap] POPULATING\n");
  uint8_t *data =
    (uint8_t*) mmap(NULL, video->size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_POPULATE, fd, 0);

  infile->buf = data;

  printf("[mmap] done!\n");

  #else
  printf("ram buffer\n");
  #endif

  printf("[touch_buf] ensuring file is in mem\n");
  volatile uint8_t touch_buf;
  for (size_t i = 0; i < infile->size; i += 4096) {
    touch_buf = infile->buf[i];
  }
  printf("[touch_buf] done!\n");

  /* Encode frames */
  int luma_size = width * height;
  int chroma_size = luma_size / 4;

  int numframes = 0;

  volatile uint64_t start_c, end_c, total_c, per_byte_c, cpu_freq;
  volatile double time_seconds, per_frame_c;

  start_c = read_cntvct();

  for( ;; i_frame++ ) {
    /* Read input frame */
    if( mf_fread( pic.img.plane[0], 1, luma_size, infile) != luma_size ) {
      break;
    }
    if( mf_fread( pic.img.plane[1], 1, chroma_size, infile) != chroma_size ) {
      break;
    }
    if( mf_fread( pic.img.plane[2], 1, chroma_size, infile) != chroma_size ) {
      break;
    }

    pic.i_pts = i_frame;
    i_frame_size = x264_encoder_encode( h, &nal, &i_nal, &pic, &pic_out );

    ++numframes;
    printf("frame %d\n", numframes);

    if( i_frame_size < 0 ) {
      exit(2);
    }
    else if( i_frame_size ) {
      if( !mf_fwrite( nal->p_payload, i_frame_size, 1, outfile) ) {
        exit(3);
      }
    }
  }
  /* Flush delayed frames */
  while( x264_encoder_delayed_frames( h ) ) {
    i_frame_size = x264_encoder_encode( h, &nal, &i_nal, NULL, &pic_out );
    if( i_frame_size < 0 ) {
      exit(4);
    }
    else if( i_frame_size ) {
      if( !mf_fwrite( nal->p_payload, i_frame_size, 1, outfile) ) {
        exit(5);
      }
    }
  }

  end_c = read_cntvct();

  x264_encoder_close( h );
  x264_picture_clean( &pic );

  puts("");

  cpu_freq = read_cntfrq();

  total_c = (end_c - start_c);
  per_byte_c = total_c / video->size;
  per_frame_c = (double)total_c / (double)numframes;
  time_seconds = (double) (((double)end_c - (double)start_c) / (double)cpu_freq);

  double compression_percent = 100 - (100 * ((double)outfile->pos / (double)infile->size));

  printf("                                ");
  printf("[encoding], res:%lux%lu frames:%lu compression:%.1f%%\n",
         width, height, numframes, compression_percent);
  printf("                      clock frequency (Hz), %lu\n", cpu_freq);

#ifdef LINUX_NATIVE
  printf("[DATA-OUT][x264][enc.TIME    ][LINUX_NAT] , %f\n", time_seconds);
  printf("[DATA-OUT][x264][enc.CYCLES  ][LINUX_NAT] , %lu\n", total_c);
  printf("[DATA-OUT][x264][enc.PERFRAME][LINUX_NAT] , %f\n", per_frame_c);
  printf("[DATA-OUT][x264][enc.PERBYTE ][LINUX_NAT] , %lu\n", per_byte_c);
#endif

#ifdef INCLUDEOS
  printf("[DATA-OUT][x264][enc.TIME    ][INCLUDEOS] , %f\n", time_seconds);
  printf("[DATA-OUT][x264][enc.CYCLES  ][INCLUDEOS] , %lu\n", total_c);
  printf("[DATA-OUT][x264][enc.PERFRAME][INCLUDEOS] , %f\n", per_frame_c);
  printf("[DATA-OUT][x264][enc.PERBYTE ][INCLUDEOS] , %lu\n", per_byte_c);
#endif

#ifdef LINUX_VM
  printf("[DATA-OUT][x264][enc.TIME    ][LINUX_VM]  , %f\n", time_seconds);
  printf("[DATA-OUT][x264][enc.CYCLES  ][LINUX_VM]  , %lu\n", total_c);
  printf("[DATA-OUT][x264][enc.PERFRAME][LINUX_VM]  , %f\n", per_frame_c);
  printf("[DATA-OUT][x264][enc.PERBYTE ][LINUX_VM]  , %lu\n", per_byte_c);
#endif

  puts("");

#if 0
  printf("(start) MEMFILE_OUT->buf = %lu\n", MEMFILE_OUT->buf);
  printf("(len)   MEMFILE_OUT->pos = %lu\n", MEMFILE_OUT->pos);
  printf("Ctrl-a + c\n");
  printf("pmemsave %lu %lu x264.out\n", MEMFILE_OUT->buf, MEMFILE_OUT->pos);
#endif

  return 0;
}
