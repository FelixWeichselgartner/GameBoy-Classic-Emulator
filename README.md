## GameBoy-Classic-Emulator by Felix Weichselgartner

Copyright (c) 2019 Felix Weichselgartner

All rights are reserved.

You may not sell this code.

Do not claim this as your own work.

Do not use this software for illegal purposes.

If you have improvements on the code pls contact me.

contact information:

info@felix-weichselgartner.de

## Test-Roms:

| Test Rom       | Passing? | Image                                                                                                                       |
|----------------|----------|-----------------------------------------------------------------------------------------------------------------------------|
| Blargg:        |          |                                                                                                                             |
| cpu_instr      | Yes      | ![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/04b0382e-acc7-4381-b469-3ba1b6c15255.png) |
| instr_timing   | Yes      | ![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/1b7c192f-0c23-49eb-86da-9db3dd4a832a.png) |
| halt_bug       | Yes      | ![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/5cd5efaf-c9b8-4b57-b3fa-1b48d2111440.png) |
| mem_timing     | No       |                                                                                                                             |
| mem_timing-2   | No       |                                                                                                                             |
| oam_bug        | No       |                                                                                                                             |
| interrupt_time | No       |                                                                                                                             |
| dmg_sound      | No       |                                                                                                                             |
| cgb_sound      | No       |                                                                                                                             |
|                |          |                                                                                                                             |
| Mooneye        |          |                                                                                                                             |
| timer (all)    | Yes      |                                                                                                                             |
|                |          |                                                                                                                             |

## Screens:

Tetris - Loading Screen:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/a71e0f5e-81b3-4aa6-aa23-17d5fcbc632f.png)

Tetris - Menu:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/d9b76f5c-2be8-40a9-8e88-b27dfd0c60cb.png)

Dr. Mario - Menu:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/b0f8e811-818d-40a7-a315-71f117d49859.png)

Minesweeper - Loading Screen:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/95c2a7ea-0ff0-4ade-8f10-78f52beb46c9.png)

Minesweeper - Menu:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/28f172f9-5700-450e-9dea-bae4d31cc402.png)

Asterix - Loading Screen:

![img](http://felix-weichselgartner.westeurope.cloudapp.azure.com/media/markdownx/49902649-a70e-41b4-967a-a8451a040fcc.png)


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

* performance seems to be better when compiled with g++ on linux, than with vs2019 on windows.
* MBC1 is not working. Games that use rom banking don't load.
* blarggs test roms: only passing cpu_instrs, instr_timing and halt_bug so far.
* cmake and mingw32 compiling on windows does not bring a sdl2 window at execution.
* emulator is running in slower speed -> at least when compiled with vs2019 on windows.

## Currently in Work:

* memory timing
* making peace with blargg's test roms :P.
* serial data transfer.
* other memory bank controllers.

## To Do:

* maybe make pointers of all self-made objects. make references & of them and use . to access.
* implement sound.
* finish MBC_Cartridge to be able to play form real cartidges (maybe one version for pi, arduino).

## Resources:

* http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
* http://www.codeslinger.co.uk/pages/projects/gameboy/opcodes.html
* https://cturt.github.io/cinoop.html
* http://gbdev.gg8.se/wiki/articles/Main_Page
* https://rednex.github.io/rgbds/gbz80.7.html#RRC_r8
* http://imrannazar.com/Gameboy-Z80-Opcode-Map
* https://rednex.github.io/rgbds/gbz80.7.html#INC_r8
* http://www.chibiakumas.com/z80/Gameboy.php
* http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
* https://blog.rekawek.eu/2017/02/09/coffee-gb/
* http://www.z80.info/z80syntx.htm#DAA
* https://meatfighter.com/gameboy/TheNintendoGameboy.pdf
* http://gbdev.gg8.se/files/roms/blargg-gb-tests/
* http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Graphics
* https://blog.ryanlevick.com/DMG-01/public/book/introduction.html
* https://fms.komkon.org/GameBoy/Tech/Software.html
* http://gbdev.gg8.se/wiki/articles/Gameboy_Bootstrap_ROM
* http://bgb.bircd.org/pandocs.txt
* https://gekkio.fi/blog/2016-10-10-game-boy-test-rom-dos-and-donts.html
* https://github.com/trekawek/coffee-gb