// config.h



#ifndef _CONFIG_H
#define _CONFIG_H

#include <cstdio>


#if defined _WIN32

#define STUB_FUNCTION core::nprintf( "Warning", "STUB: %s in " __FILE__ " at line %d\n", __FUNCTION__, __LINE__)

#else  // ! Win32


#include <unistd.h>
#include <cmath>
#include <cstdint>


// don't verbose stub funtions unless we're debugging
#define STUB_FUNCTION core::nprintf( "Warning", "STUB: %s in " __FILE__ " at line %d, thread %d\n", __FUNCTION__, __LINE__, getpid() )
#define DEBUGME(d1) core::nprintf( "Warning", "DEBUGME: %s in " __FILE__ " at line %d, msg \"%s\", thread %d\n", __FUNCTION__, __LINE__, d1, getpid() )

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

#ifndef INVALID_SOCKET
#define INVALID_SOCKET ((SOCKET) -1)
#endif

// file related items
#define _MAX_FNAME					255
#define _MAX_PATH					255
#define MAX_PATH					255
#define _unlink(s)					unlink(s)

int _chdir(const char *path);
int _getcwd(char *buffer, unsigned int len);
int _mkdir(const char *path);
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);

// other stuff
#define _hypot(x, y)  hypot(x, y)

int MulDiv(int number, int numerator, int denominator);

#endif  // if !defined (WINDOWS)

#endif // ifndef _CONFIG_H
