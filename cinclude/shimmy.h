#ifndef SHIMMY_H

#include <stdint.h>

// Functions are defined in libshimmy.a (static lib from Rust code)
void shmy_init(void);

// Check if domain is blocked. Returns 0 if not blocked, 1 if blocked
uint8_t shmy_get(char *domain);

// Get domain from cache. Returns 0 if not found
char *shmy_domain_lookup_ipv4(uint32_t ip);
char *shmy_domain_lookup_ipv6(uint8_t *ip);

// Adds a domain to the cache
void shmy_add_domain_ipv4(char *domain, uint32_t ip);
void shmy_add_domain_ipv6(char *domain, uint8_t *ip);

#define SHIMMY_H
#endif
