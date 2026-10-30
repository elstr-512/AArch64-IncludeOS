cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(includeos_x264_encoder LANGUAGES C CXX)

# Default build type
set(CMAKE_BUILD_TYPE Release)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Disable FP contraction (needed for reproducibility)
add_compile_options(-ffp-contract=off)

# Static linking (Linux build matches IncludeOS)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# INCLUDEOS
find_package(IncludeOS)

# =============================
# Shared code sources
# =============================
set(SHARED_SOURCES

  src/memfile.cpp
  src/memfile.h

  src/get_cpu_cycles.h

  src/video-conf.cpp
  src/video-conf.h

  src/x264wrapper.cpp
  src/x264wrapper.h
)

if(IncludeOS_FOUND)
  # =============================
  # IncludeOS service
  # =============================

  include(os)

  # Shared code
  add_library(service STATIC ${SHARED_SOURCES})

  target_compile_options(service PUBLIC -DINCLUDEOS)

  os_add_executable(includeos_x264_encoder " x264 Benchmark - OS included" src/main-includeos.cpp)

  target_link_libraries(includeos_x264_encoder.elf.bin service x264)

  if("${ARCH}" STREQUAL "aarch64")
    os_add_drivers(includeos_x264_encoder boot_logger)
    os_add_stdout(includeos_x264_encoder default_stdout)

  else()
    os_add_drivers(includeos_x264_encoder virtionet vmxnet3 boot_logger)
    os_add_stdout(includeos_x264_encoder default_stdout)
  endif()

  # TODO: remove or hide the ELF_POSTFIX.
  # currently os.cmake expects the target name to be TARGET.elf.bin
  # where TARGET is the first param to os_add_executable.
  # I added it here becuase nix expects an install target.
  set(BINARY_NAME includeos_x264_encoder)
  set(ELF_TARGET ${BINARY_NAME}${ELF_POSTFIX})

  install(TARGETS ${ELF_TARGET} DESTINATION bin)

else()
  # =============================
  # Linux executable
  # =============================

  # Shared code
  add_library(service STATIC ${SHARED_SOURCES})

  target_compile_options(service PUBLIC -DLINUX_NATIVE)

  add_executable(linux_x264_native.out src/main-linux.cpp)

  target_link_libraries(linux_x264_native.out service x264)

  install(TARGETS linux_x264_native.out DESTINATION bin)



  add_library(linux_vm_s STATIC ${SHARED_SOURCES})

  target_compile_options(linux_vm_s PUBLIC -DLINUX_VM)

  add_executable(linux_VM src/main-linux.cpp)

  target_link_libraries(linux_VM linux_vm_s x264)

  install(TARGETS linux_VM DESTINATION bin)

endif()

