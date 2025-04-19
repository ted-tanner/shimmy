#define DLL_EXPORT __attribute__((visibility("default")))

#ifdef __APPLE__

#include <stdint.h>
#include <netdb.h>

#include "shimmy.h"

__attribute__((constructor)) void init(void) {
    shmy_init();
}

DLL_EXPORT int shim_getaddrinfo(const char *node, const char *service,
                                const struct addrinfo *hints, struct addrinfo **res) {
    if (shmy_is_domain_blocked(node)) {
        return EAI_NONAME;
    }

    return getaddrinfo(node, service, hints, res);
}

__attribute__((used)) static struct {
    const void *replacement;
    const void *replacee;
} _interpose_getaddrinfo

__attribute__((section("__DATA,__interpose"))) = {
    (const void *)(unsigned long)&shim_getaddrinfo,
    (const void *)(unsigned long)&getaddrinfo,
};
    

#elif defined __linux__

#error "Not yet implemented for this platform"

#elif defined _WIN32 || defined _WIN64

#error "Not yet implemented for this platform"

#else
#error "Unsupported platform"
#endif
