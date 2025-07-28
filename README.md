# DeSmuME
[![AppVeyor CI Build Status](https://ci.appveyor.com/api/projects/status/abfd7jm09wnmxyvu?svg=true)](https://ci.appveyor.com/project/zeromus/desmume)

DeSmuME is a Nintendo DS emulator.

http://desmume.org/download

# DeSmuME Matrix
DeSmuMe matrix is a fork of [TASEmulatos/desmume](https://www.github.com/TASEmulators/desmume) that adds in functionality
to optionally send frame data over TCP/IPv4 to a server attached to an RGB
LED matrix. The server code and network protocol definition can be found [here](https://www.github.com/ZachWWalden/rpi-rgb-matrix-sink-server)

The added functionality is currently only accessible on POSIX systems through
a CLI frontend.

## Video Demonstration

[![Video Demonstration](https://img.youtube.com/vi/yfpDpgpz51Q/0.jpg)](https://www.youtube.com/watch?v=yfpDpgpz51Q "Video Demonstration")

# Usage
The program allows you to send frame data to a server representing the
top screen of the DS or the bottom screen, or both. To do so you must
pass an IPv4 address and port combination where your matrix server is
located. There are separate, optional, CLI arguments for both screen.

To send top screen frame data:
~~~zsh
$ desmume-matrix --ts-sink-address "IPv4_ADDR:PORT" /path/to/rom
~~~

To send bottom screen frame data:
~~~zsh
$ desmume-matrix --bs-sink-address "IPv4_ADDR:PORT" /path/to/rom
~~~

To send both top screen and bottom screen frame data:
~~~zsh
$ desmume-matrix --ts-sink-address "IPv4_ADDR_TS:PORT_TS" --bs-sink-address "IPv4_ADDR_BS:PORT_BS" /path/to/rom
~~~

If there is not a server that implements the correct protocol at either of
the passed addresses, the emulator will close.

# Building
To build the program follow the instructions for building the posix
frontends in [README.LIN](desmume/README.LIN)
