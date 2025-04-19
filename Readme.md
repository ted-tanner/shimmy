# Shimmy

Shimmy is a tool for blocking web requests to a configured set of domains in programs into which it is installed. Shimmy does this by intercepting calls to the `connect()` function in `libc` (via a "shim") when it is dynamically linked, as it is in most consumer-grade software.

## Some concerns

* If the browser manages DNS lookups on its own (e.g. in order to use a privacy-respecting DNS server), the shimmed system DNS will be bypassed.
* macOS prevents scripts from interposing DLLs without superuser access for security purposes. A setting must change on the .app package that lowers macOS's protections for the app. On Linux, developers can use a "set‑UID" binary that doesn't allow shimming, similar to macOS (though it is up to the developer to enable this.
