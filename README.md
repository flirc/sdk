# Flirc SDK

Currently only for flirc usb with eventual support for the skip line of 
remotes.

## [cli](./cli/README.md)

This is the same flirc_util as the publicly released binary distributed 
through the [downloads section](https://www.flirc.tv) website. All apps
are built using our own SDK. Feel free to build your own using this example.

## [ir](./ir/README.md)

This is a new example project which is a minimal example of using flirc usb
to capture IR signals into a buffer. Also included is a new libir library
which is a general purpose IR decoder and IR encoder library.

This minimal application is to demonstrate how to use the library.

libir is also the same DLL powering a new IR Decoding [website](https://ir.flirc.io) 
for general public usage.

The website will be constantly updated as libir gets updated.
