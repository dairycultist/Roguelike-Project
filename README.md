# Fantasy Console

![](https://pbs.twimg.com/media/DWx0C4TVMAAuq7e.jpg)

> The size of this image is 445 KB. The original file size for Super Mario Bros. was only 32 KB.

make many games for my fantasy console and foster a smol community and make me feel accomplished

Basically a ROM binary interpretter (in the 6502 instruction set, compiled via cc65 or similar) with specifications for I/O in virtual memory.

I think some things that'd be tricky would be timing (how do I ensure a steady 60fps) and just laying things out in memory properly (how do I ensure sprite data in the ROM is where the system expects it to be)

Specification:
- 32 KiB ROM (cart size)
	- 16 KiB sprite sheet (256 8x8 sprites)
	- 16 KiB text/data
- 4 KiB of general-purpose working RAM (stack, heap)
- 0.9375 KiB VRAM (256x240 tiles) that indexes into the ROM's sprite sheet for 'hardware rendering' (that draws those sprites to the window/virtual screen)
- Specific addresses for input reports
- Specific addresses for non-background sprite rendering
- Specific (1-byte) address for background offset
- ~1 million instructions per second (since timing is implemented via cycle-counting)

NOT designed to be faithful to the original 6502 hardware outside of the instruction set and amount of addressable space.