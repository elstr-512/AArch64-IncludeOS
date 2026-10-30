/* memfile.cpp */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memfile.h"
#include "video-conf.h"

#define PANIC_ON_OVERFLOW 1

struct MemFile memfile_in = {
  video->pmem_pos,
  video->size,
  0,
  false,
  false,
};

static const size_t MEGABYTE = 1024 * 1024;

// foreman.yuv output = ? MB
// tractor.yuv output = 22 MB

static uint8_t output_buf[32 * MEGABYTE];

struct MemFile memfile_out = {
  output_buf,
  sizeof(output_buf),
  0,
  false,
  false,
};

struct MemFile* const MEMFILE_IN = &memfile_in;
struct MemFile* const MEMFILE_OUT = &memfile_out;

// MemFile ---- Write functions. Bounded, no realloc.

int mf_fputc(int byte, MemFile* f) {
  if (f->pos >= f->size) {
    f->error = true;
    return EOF;
  }
  f->buf[f->pos++] = (uint8_t)byte;
  return byte;
}

size_t mf_fwrite(const void* data, size_t size, size_t len, MemFile* f) {
  size_t avail = f->size - f->pos;
  size_t total = size * len;

  size_t to_write = (total <= avail) ? total : avail;

  memcpy(f->buf + f->pos, data, to_write);
  f->pos += to_write;

  if (total > avail) {
#if PANIC_ON_OVERFLOW
    puts("Output buffer overflow");
    exit(44);
#endif
    f->error = true;
  }

  return to_write / size;
}

// MemFile ---- Read functions.

int mf_fgetc(MemFile* f) {
  if (f->pos >= f->size) {
    f->eof = true;
    return EOF;
  }
  return f->buf[f->pos++];
}

size_t mf_fread(void* data, size_t size, size_t len, MemFile* f) {
  if (f->pos >= f->size) {
    f->eof = true;
    return 0;
  }

  size_t avail = f->size - f->pos;
  size_t total = size * len;

  size_t to_read = (total <= avail) ? total : avail;

  memcpy(data, f->buf + f->pos, to_read);
  f->pos += to_read;

  if (to_read < total)
    f->eof = true;

  return to_read / size;
}

// MemFile ---- Error checks.

int mf_feof(const MemFile* f) {
  return f->eof ? 1 : 0;
}

int mf_ferror(const MemFile* f) {
  return f->error ? 1 : 0;
}

