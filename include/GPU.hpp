#ifndef GPU_HPP_
#define GPU_HPP_

//----------------------------------------------------------------------------------------------
#include "../include/CPU.hpp"
#include "../include/PPU.hpp"
#include "../include/Window.hpp"
#include <string>
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// screen resolution:

// amount of pixels in x direction.
#define X_RES 160
// amount of pixels in y direction.
#define Y_RES 144
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// GPU MODES:

enum {
	GPU_MODE_HBLANK,
	GPU_MODE_VBLANK,
	GPU_MODE_OAM,
	GPU_MODE_VRAM
};
//----------------------------------------------------------------------------------------------


/*----------------------------------------------------------------------------------------------
// lcd control register @ 0xFF40
Bit 7 - LCD Display Enable(0 = Off, 1 = On)
Bit 6 - Window Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 5 - Window Display Enable(0 = Off, 1 = On)
Bit 4 - BG & Window Tile Data Select(0 = 8800 - 97FF, 1 = 8000 - 8FFF)
Bit 3 - BG Tile Map Display Select(0 = 9800 - 9BFF, 1 = 9C00 - 9FFF)
Bit 2 - OBJ(Sprite) Size(0 = 8x8, 1 = 8x16)
Bit 1 - OBJ(Sprite) Display Enable(0 = Off, 1 = On)
Bit 0 - BG Display(for CGB see below) (0 = Off, 1 = On)
*///--------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// format gameboy:
// 0xAARRGGBB (Alhpa-Red-Green-Blue)


//----------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------
class GPU {

private:

	class CPU* cpu;
	class Memory* memory;
    class Window *window;

    int ScanLineCounter;
    const int pixelsPerTile = 8;
    const int SCANLINECYCLES = 456;
	int GpuMode;
	bool inc_en;


public:

    void reset();

    GPU(class CPU*);

	Byte getControl() const;
	void setControl(Byte);

	Byte getStatus() const;
	void setStatus(Byte);
	void setStatusBit(int, Byte);
	void setStatusMode(Byte);

	Byte getScanline() const;
	void setScanline(Byte);
	void IncScanline();
	void resetScanline();

	void clearScreen();

	bool IsLCDEnabled() const;
	void SetLCDStatus();

	Byte getColor(Byte, Word);

	void RenderNintendoLogo();
	void RenderTiles(Byte);
	void RenderSprites(Byte);

	void DrawScanLine();

	void renderDisplay(Byte);

	void requestLcdcInterrupt(int);

	void tick();
	void UpdateGraphics(int);

	void render();

	void screenshot();

};
//----------------------------------------------------------------------------------------------

#endif