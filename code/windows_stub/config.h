// config.h



#ifndef _CONFIG_H
#define _CONFIG_H

#include <cstdio>


#include <SDL.h>

#ifndef BYTE_ORDER
#include <SDL_endian.h>
#endif

#ifndef BYTE_ORDER
#define LITTLE_ENDIAN 1234
#define BIG_ENDIAN    4321

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define BYTE_ORDER   LITTLE_ENDIAN
#elif SDL_BYTEORDER == SDL_BIG_ENDIAN
#define BYTE_ORDER   BIG_ENDIAN
#else
#error unknown byte order
#endif
#endif  // BYTE_ORDER

#if defined _WIN32

#ifndef filelength
#define filelength _filelength
#endif

#define STUB_FUNCTION nprintf(( "Warning", "STUB: %s in " __FILE__ " at line %d\n", __FUNCTION__, __LINE__))

#define SOCKLEN_T int

#define NETCALL_WOULDBLOCK(err) (err == WSAEWOULDBLOCK)

#if (_WIN32_WINNT < 0x0600)
#define AI_NUMERICSERV              0x00000008
#define AI_ALL                      0x00000100
#define AI_ADDRCONFIG               0x00000400
#define AI_V4MAPPED                 0x00000800
#define AI_NON_AUTHORITATIVE        0x00004000
#define AI_SECURE                   0x00008000
#define AI_RETURN_PREFERRED_NAMES   0x00010000

extern const char *inet_ntop(int af, const void *src, char *dst, int size);
#endif

#else  // ! Win32


#include <unistd.h>
#include <cmath>
#include <cstdint>


// don't verbose stub funtions unless we're debugging
#define STUB_FUNCTION nprintf(( "Warning", "STUB: %s in " __FILE__ " at line %d, thread %d\n", __FUNCTION__, __LINE__, getpid() ))
#define DEBUGME(d1) nprintf(( "Warning", "DEBUGME: %s in " __FILE__ " at line %d, msg \"%s\", thread %d\n", __FUNCTION__, __LINE__, d1, getpid() ))

// networking/socket stuff
#define SOCKET			int
#define SOCKADDR		struct sockaddr
#define SOCKADDR_IN		struct sockaddr_in
#define LPSOCKADDR		struct sockaddr*
#define HOSTENT			struct hostent
#define SERVENT			struct servent
#define closesocket(x)	close(x)
#define WSAEALREADY     EALREADY
#define WSAEINVAL       EINVAL
#define WSAEWOULDBLOCK  EINPROGRESS
#define WSAEISCONN      EISCONN
#define WSAENOTSOCK     ENOTSOCK
#define WSAECONNRESET   ECONNRESET
#define WSAECONNABORTED ECONNABORTED
#define WSAESHUTDOWN    ESHUTDOWN
#define WSAEADDRINUSE	EADDRINUSE
#define SOCKET_ERROR	(-1)
#define ioctlsocket(x, y, z)	ioctl(x, y, z)

#define NETCALL_WOULDBLOCK(err) (err == EAGAIN || err == EINPROGRESS)

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ((SOCKET) -1)
#endif

#define SOCKLEN_T socklen_t


#define _unlink(s)					unlink(s)

int filelength(int fd);
int _chdir(const char *path);
int _getcwd(char *buffer, unsigned int len);
int _mkdir(const char *path);
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);

// other stuff
#define _hypot(x, y)  hypot(x, y)

int MulDiv(int number, int numerator, int denominator);

#endif  // if !defined (WINDOWS)

#endif // ifndef _CONFIG_H
