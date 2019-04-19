## GameBoy-Classic-Emulator by Felix Weichselgartner

Copyright (c) 2019 Felix Weichselgartner

All rights are reserved.

You may not sell this code.

Do not claim this as your own work.

If you have improvements on the code pls contact me.

contact information:

info@felix-weichselgartner.de


## Installation:

Download the emulator:
```
git clone https://github.com/FelixWeichselgartner/GameBoy-Classic-Emulator.git
cd GameBoy-Classic-Emulator/
```

### For Linux:

Make sure you have the current version of SDL2 installed.
```
sudo apt-get install libsdl2-dev
```

Install it:
```
cmake .
make
```

And run:
```
./GameBoy-Classic-Emulator
```

### Windows:

Installing on windows is a bit more trickier than on linux.

Make sure you have SDL2 installed in your MinGW folder (https://www.libsdl.org/download-2.0.php).

You can compile it with:
```
cmake . -G "MinGW Makefiles"
mingw32-make
``` 

And run:
```
.\GameBoy-Classic-Emulator.exe
```

However for me the SDL window never showed up here.

Probably the best method to compile on windows is to use Microsoft Visual Studio.


## Known Issues:

* on Ubuntu screen is flickering in the upper line (however not on windows).
* joypad input not working.
* MBC1 is not working. Games that use rom banking don't load.
* blarggs test roms: failing in every one I tried so far.
* cmake and mingw32 compiling on windows does not bring a sdl2 window at execution.
* emulator is running in slower speed -> at least in the bootstrap sequence.

## Resources:

* http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
* http://www.codeslinger.co.uk/pages/projects/gameboy/opcodes.html
* https://cturt.github.io/cinoop.html
* http://gbdev.gg8.se/wiki/articles/Main_Page
* http://imrannazar.com/Gameboy-Z80-Opcode-Map
* https://rednex.github.io/rgbds/gbz80.7.html#INC_r8
* http://www.chibiakumas.com/z80/Gameboy.php
* http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
* https://blog.rekawek.eu/2017/02/09/coffee-gb/
* http://www.z80.info/z80syntx.htm#DAA
* http://gbdev.gg8.se/files/roms/blargg-gb-tests/
* http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics
* https://blog.ryanlevick.com/DMG-01/public/book/introduction.html
* https://fms.komkon.org/GameBoy/Tech/Software.html
* http://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM
* http://bgb.bircd.org/pandocs.txt
* https://gekkio.fi/blog/2016-10-10-game-boy-test-rom-dos-and-donts.html