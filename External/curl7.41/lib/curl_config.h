#ifndef HEADER_CURL_CONFIG_H
#define HEADER_CURL_CONFIG_H

/////////////////////////////////////////////////////////////////////////////
//
//	StromForge
//  Author: KyungKun Ko
//

#include <stdint.h>

#ifdef __ANDROID__
#define OS "android"
#elif defined(_WIN32) || defined(_WIN64)
#define OS "windows"
#elif defined(__IOS__)
#define OS "IOS"
#endif


#if defined(_WIN32) || defined(_WIN64)
#define STDC_HEADERS            1
#define HAVE_WINDOWS_H          1
#define HAVE_STRDUP             1
#define HAVE_IOCTLSOCKET_FIONBIO 1
#define HAVE_STRUCT_POLLFD 1

#include <BaseTsd.h>
typedef SSIZE_T ssize_t;

/* Define if struct sockaddr_in6 has the sin6_scope_id member. */
#define HAVE_SOCKADDR_IN6_SIN6_SCOPE_ID 1
#define HAVE_WINSOCK2_H         1
#define HAVE_WS2TCPIP_H         1

#else
#define HAVE_SYS_TIME_H         1
#define HAVE_UNISTD_H           1
#define HAVE_SYS_SOCKET_H       1
#define HAVE_UTIME_H            1
#define HAVE_FCNTL_O_NONBLOCK 1
#define HAVE_SYS_SELECT_H       1
#define HAVE_NETINET_IN_H       1
#define HAVE_NETDB_H            1
#define HAVE_ARPA_INET_H        1
#define HAVE_NET_IF_H           1
#define HAVE_SIGACTION          1

#endif

/* Define if you want the built-in manual */
#define USE_MANUAL              1
#define HAVE_ERRNO_H            1
#define HAVE_SYS_TYPES_H        1
#define HAVE_GETTIMEOFDAY       1
#define HAVE_FCNTL_H            1
#define HAVE_SYS_STAT_H         1
#define HAVE_ALLOCA_H           1
#define HAVE_STDLIB_H           1
#define HAVE_TIME_H             1
#define HAVE_SYS_UTIME_H        1
#define HAVE_STRUCT_TIMEVAL     1

#define TIME_WITH_SYS_TIME      1

#define HAVE_ALARM              1
#define HAVE_CLOSESOCKET		1
#define HAVE_FTRUNCATE          1
#define HAVE_GETSERVBYNAME		1
#define HAVE_GETPROTOBYNAME
#define HAVE_UTIME              1
#define HAVE_SETVBUF            1
#define HAVE_STRFTIME           1
#define HAVE_INET_ADDR          1
#define HAVE_MEMCPY             1
#define HAVE_SELECT             1
#define HAVE_SOCKET             1

#define HAVE_SIGNAL_H           1
#define HAVE_SIG_ATOMIC_T       1

#define USE_OPENSSL           1


#define CURL_DISABLE_LDAP       1

#define HAVE_RAND_STATUS        1
#define HAVE_RAND_EGD           1

//#define HAVE_IOCTL              1
//#define HAVE_IOCTL_FIONBIO      1
/* Define if you have the ioctlsocket function. */
#define HAVE_IOCTLSOCKET 1



#define RETSIGTYPE void

#define SIZEOF_INT              4
#define SIZEOF_SHORT            2

#if SIZE_MAX >= ULLONG_MAX
#define SIZEOF_SIZE_T           8
#else
#define SIZEOF_SIZE_T           4
#endif

#define HAVE_GETNAMEINFO 1
#define GETNAMEINFO_QUAL_ARG1 const
#define GETNAMEINFO_TYPE_ARG1 struct sockaddr *
#define GETNAMEINFO_TYPE_ARG2 socklen_t
#define GETNAMEINFO_TYPE_ARG46 size_t
#define GETNAMEINFO_TYPE_ARG7 int

#define HAVE_RECV 1
#define RECV_TYPE_ARG1 int
#define RECV_TYPE_ARG2 void *
#define RECV_TYPE_ARG3 size_t
#define RECV_TYPE_ARG4 int
#define RECV_TYPE_RETV ssize_t

#define HAVE_RECVFROM 1
#define RECVFROM_TYPE_ARG1 int
#define RECVFROM_TYPE_ARG2 void
#define RECVFROM_TYPE_ARG3 size_t
#define RECVFROM_TYPE_ARG4 int
#define RECVFROM_TYPE_ARG5 struct sockaddr
#define RECVFROM_TYPE_ARG6 int
#define RECVFROM_TYPE_RETV ssize_t
#define RECVFROM_TYPE_ARG2_IS_VOID 1

#define HAVE_SEND 1
#define SEND_TYPE_ARG1 int
#define SEND_QUAL_ARG2 const
#define SEND_TYPE_ARG2 void *
#define SEND_TYPE_ARG3 size_t
#define SEND_TYPE_ARG4 int
#define SEND_TYPE_RETV ssize_t

//#define HAVE_EXTRA_STRICMP_H 1
//#define HAVE_EXTRA_STRDUP_H  1

#endif /* HEADER_CURL_CONFIG_MAC_H */
