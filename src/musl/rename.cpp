#include "common.hpp"
#include <stdio.h>

static long sys_rename(const char* /*oldpath*/, const char* /*newpath*/)
{
  // currently makes no sense, especially since we're read-only
  return -EROFS;
}

extern "C"
long syscall_SYS_rename(const char* oldpath, const char* newpath)
{
  return strace(sys_rename, "rename", oldpath, newpath);
}

// TODO: !!!
#warning "TODO: stub sys_renameat() symbol so I can use x264 encoder :p"
# if 1
static long sys_renameat(const char* /*oldpath*/, const char* /*newpath*/)
{
  // currently makes no sense, especially since we're read-only
  return -EROFS;
}

extern "C"
long syscall_SYS_renameat(const char* oldpath, const char* newpath)
{
  return strace(sys_renameat, "renameat", oldpath, newpath);
}
#endif

