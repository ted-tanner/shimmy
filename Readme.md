# Shimmy

Shimmy is a tool for blocking web requests to a configured set of domains in programs into which it is installed. Shimmy does this by intercepting calls to the `connect()` function in `libc` (via a "shim") when it is dynamically linked, as it is in most consumer-grade software.

## Some concerns

* A call to getaddrinfo is made to translate IPs into domains. If the browser manages DNS lookups on its own (e.g. in order to use a privacy-respecting DNS server), there may be rare discrepancies.
    1. If the browser provides an API for browser plugins to access the DNS cache, an accompanying browser plugin that integrates with the shim could be installed
    2. The browser setting that determines which DNS to use is stored on disk and can be read and mimicked by the shim (i.e. the shim could use the same DNS service as the browser and manage a duplicate DNS cache)
* macOS prevents scripts from interposing DLLs without superuser access for security purposes. A setting must change on the .app package that lowers macOS's protections for the app. These protections don't exist for Windows and Linux.
