# Fantasy Console

![](https://pbs.twimg.com/media/DWx0C4TVMAAuq7e.jpg)

> The size of this image is 445 KB. The original file size for Super Mario Bros. was only 32 KB.

make many games for my fantasy console and foster a smol community and make me feel accomplished

Basically a ROM binary interpretter (in the 6502 instruction set, compiled via cc65 or similar) with specifications for I/O in virtual memory.

Artificially limited CPU clock cycles per second, working RAM/loadable ROM (cart) size, and types of 'hardware' rendering.

I'd have my own spec, 256x240 screen, specific address spaces for input reports, a limited amount of RAM, a section of VRAM for sprites and then "hardware rendering" that can draw those sprites to the window

I think some things that'd be tricky would be timing (how do I ensure a steady 60fps) and just laying things out in memory properly (how do I ensure sprite data in the ROM is where the system expects it to be)
