#include <stdio.h>

#define DLL_EXPORT __attribute__((visibility("default")))

#ifdef __APPLE__

#include <dlfcn.h>
#include <sys/socket.h>

DLL_EXPORT int shim_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    // TODO
    return connect(sockfd, addr, addrlen);
}

__attribute__((used)) static struct { const void *replacement; const void *replacee; } _interpose_connect
__attribute__ ((section ("__DATA,__interpose"))) = { (const void *)(unsigned long)&shim_connect, (const void *)(unsigned long)&connect };

#elif defined __linux__

// TODO

#elif defined _WIN32 || defined _WIN64

// TODO

#else
#error "Unsupported platform"
#endif
