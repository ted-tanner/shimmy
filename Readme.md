# Shimmy

Shimmy is a tool for blocking web requests to a configured set of domains in programs into which it is installed. Shimmy does this by intercepting calls to the `connect()` function in `libc` (via a "shim") when it is dynamically linked, as it is in most consumer-grade software. Shimmy can filter out calls to a configured domain entirely or can spoof a response that the calling program will see.
