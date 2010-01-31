/*
 * This file is part of GtkEveMon.
 *
 * GtkEveMon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with GtkEveMon. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OS_HEADER
#define OS_HEADER

#include <climits>

// XXX: namespace might be more fitting?
class OS
{
private:
  static short short_swap(short x);
  static int   int_swap(int x);
  static long  long_swap(long x);

public:
  static bool  dir_exists(char const* pathname);
  static bool  file_exists(char const* pathname);
  static char* get_default_home_path(void);
  static char* getcwd(char* buf, size_t size);
  // TODO: define modes for mkdir() for all platforms?
  static bool  mkdir(char const* pathname/*, mode_t mode*/);
  static bool  unlink(char const* pathname);

  static short letoh(short x);
  static int   letoh(int x);
  static long  letoh(long x);
  static short betoh(short x);
  static int   betoh(int x);
  static long  betoh(long x);
};

inline short OS::short_swap(short x)
{
  return (short)((x >> 8) | (x << 8));
}

inline int   OS::int_swap(int x)
{
  return ((x & 0x000000ff) << 24)
       | ((x & 0x0000ff00) <<  8)
       | ((x & 0x00ff0000) >>  8)
       | ((x & 0xff000000) >> 24);
}

inline long  OS::long_swap(long x)
{
  return ((x & 0x00000000000000ffULL) << 56)
       | ((x & 0x000000000000ff00ULL) << 40)
       | ((x & 0x0000000000ff0000ULL) << 24)
       | ((x & 0x00000000ff000000ULL) <<  8)
       | ((x & 0x000000ff00000000ULL) >>  8)
       | ((x & 0x0000ff0000000000ULL) >> 24)
       | ((x & 0x00ff000000000000ULL) >> 40)
       | ((x & 0xff00000000000000ULL) >> 56);
}

/* Determine host byte-order. */
#if defined(WIN32)
  /* "All versions of windows run little-endian, period."
   * http://social.msdn.microsoft.com/Forums/en-US/windowsmobiledev/thread/04c92ef9-e38e-415f-8958-ec9f7c196fd3
   */
# define HOST_BYTEORDER_LE
#endif

#if defined(MACOS_X)
# if defined(__ppc__)
#   define HOST_BYTEORDER_BE
# elif defined(__i386__) // XXX: What about __x86_64__?
#   define HOST_BYTEORDER_LE
# endif
#endif

#if defined(__linux__)
# include <endian.h> // part of glibc 2.9
# if __BYTE_ORDER == __LITTLE_ENDIAN
#   define HOST_BYTEORDER_LE
# else
#   define HOST_BYTEORDER_BE
# endif
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
# include <machine/endian.h>
# if BYTE_ORDER == LITTLE_ENDIAN
#   define HOST_BYTEORDER_LE
# else
#   define HOST_BYTEORDER_BE
# endif
#endif

#if defined(__SunOS)
# include <sys/byteorder.h>
# if defined(_LITTLE_ENDIAN)
#   define HOST_BYTEORDER_LE
# else
#   define HOST_BYTEORDER_BE
# endif
#endif

#if defined(HOST_BYTEORDER_LE) && defined(HOST_BYTEORDER_BE)
# error "Host endianess can't be both big and little!"
#elif defined(HOST_BYTEORDER_LE)
inline short OS::letoh(short x) { return x; }
inline int   OS::letoh(int x)   { return x; }
inline long  OS::letoh(long x)  { return x; }

inline short OS::betoh(short x) { return short_swap(x); }
inline int   OS::betoh(int x)   { return int_swap(x); }
inline long  OS::betoh(long x)  { return long_swap(x); }
#elif defined(HOST_BYTEORDER_BE)
inline short OS::letoh(short x) { return short_swap(x); }
inline int   OS::letoh(int x)   { return int_swap(x); }
inline long  OS::letoh(long x)  { return long_swap(x); }

inline short OS::betoh(short x) { return x; }
inline int   OS::betoh(int x)   { return x; }
inline long  OS::betoh(long x)  { return x; }
#else
# error "Couldn't determine host endianess!"
#endif

#endif /* OS_HEADER */