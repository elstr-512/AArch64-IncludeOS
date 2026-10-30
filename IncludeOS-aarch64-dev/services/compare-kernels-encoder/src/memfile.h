/* memfile.h */
#pragma once

#include <stdint.h>
#include <stdlib.h>

struct MemFile {
  uint8_t*  buf;
  size_t    size;
  size_t    pos;
  uint8_t   eof;
  uint8_t   error;
};

extern struct MemFile* const MEMFILE_IN;
extern struct MemFile* const MEMFILE_OUT;

int mf_fputc(int byte, struct MemFile* f);

size_t mf_fwrite(const void* data, size_t size, size_t len, struct MemFile* f);

int mf_fgetc(struct MemFile* f);

size_t mf_fread(void* data, size_t size, size_t len, struct MemFile* f);

int mf_feof(const struct MemFile* f);

int mf_ferror(const struct MemFile* f);

