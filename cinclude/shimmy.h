#ifndef SHIMMY_H

#include <stdint.h>

// Functions are defined in libshimmy.a (static lib from Rust code)
void shmy_init(void);

// Check if domain is blocked. Returns 0 if not blocked, 1 if blocked
uint8_t shmy_is_domain_blocked(const char *domain);

#define SHIMMY_H
#endif
