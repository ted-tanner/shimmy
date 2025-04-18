#define DLL_EXPORT __attribute__((visibility("default")))

#ifdef __APPLE__

#include <stdint.h>

#include <arpa/inet.h>
#include <dlfcn.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>

#include "shimmy.h"

__attribute__((constructor)) void init(void) {
    shmy_init();
}

DLL_EXPORT int shim_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
     char host[NI_MAXHOST];

    uint8_t blocked = 0;
    char *domain = 0;

    if (addr->sa_family == AF_INET) {
        struct sockaddr_in* addrV4 = (struct sockaddr_in*) addr;
        domain = shmy_domain_lookup_ipv4(addrV4->sin_addr.s_addr);

        if (!domain) {
            int err = getnameinfo(addr, addrlen, host, sizeof(host), NULL, 0, NI_NAMEREQD);
            if (err) {
                // Failed to resolve domain
                errno = EAI_FAIL;
                return -1;
            }

            shmy_add_domain_ipv4(host, addrV4->sin_addr.s_addr);
            domain = host;
        }
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6* addrV6 = (struct sockaddr_in6*) addr;
        domain = shmy_domain_lookup_ipv6(addrV6->sin6_addr.s6_addr);

        if (!domain) {
            int err = getnameinfo(addr, addrlen, host, sizeof(host), NULL, 0, NI_NAMEREQD);
            if (err) {
                // Failed to resolve domain
                errno = EAI_FAIL;
                return -1;
            }

            shmy_add_domain_ipv6(host, addrV6->sin6_addr.s6_addr);
            domain = host;
        }
    }

    blocked = shmy_get(domain);

    if (blocked) {
        errno = ECONNREFUSED;
        return -1;
    }

    return connect(sockfd, addr, addrlen);;
}

__attribute__((used)) static struct { const void *replacement; const void *replacee; } _interpose_connect
__attribute__((section ("__DATA,__interpose"))) = { (const void *)(unsigned long)&shim_connect, (const void *)(unsigned long)&connect };

#elif defined __linux__

#error "Not yet implemented for this platform"

#elif defined _WIN32 || defined _WIN64

#error "Not yet implemented for this platform"

#else
#error "Unsupported platform"
#endif
