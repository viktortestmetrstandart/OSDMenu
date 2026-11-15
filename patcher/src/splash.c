#include "splash.h"
#include "gs.h"
#include "settings.h"
#include "splash_bmp.h"

// Initializes the screen according to config options and shows the splash screen
void showSplash() {
  GSVideoMode vmode = GS_MODE_NTSC; // Use NTSC by default

  // Respect preferred mode
  if (!settings.videoMode) {
    // If mode is not set, read console region from ROM
    if (settings.romver[4] == 'E')
      vmode = GS_MODE_PAL;
  } else if (settings.videoMode == GS_MODE_PAL)
    vmode = GS_MODE_PAL;

#ifdef ENABLE_SPLASH
  gsDisplaySplash(vmode);
#else
  gsInit(vmode);
  gsClearScreen();
#endif
}

// Initializes GS and displays splash screen
void gsDisplaySplash(GSVideoMode mode) {
  gsInit(mode);
  gsClearScreen();

  int splashX = ((int)gsGetMaxX() - (int)splashWidth) / 2;
  int splashY = ((int)gsGetMaxY() - (int)splashHeight) / 2;
  if (splashX < 0)
    splashX = 0;
  if (splashY < 0)
    splashY = 0;

  gsPrintBitmap(splashX, splashY, splashWidth, splashHeight, splash);

  if (!(settings.patcherFlags & FLAG_APP_GAMEID))
    return;

  // Draw OSDMenu visual game ID
  uint8_t data[] = {0xA5, 0x00, 0xA9, 0x07, 'O', 'S', 'D', 'M', 'e', 'n', 'u', 0x00, 0xD5, 0x00};

  int xstart = (gsGetMaxX() / 2) - (sizeof(data) * 8);
  int ystart = gsGetMaxY() - (((gsGetMaxY() / 8) * 2) + 20);

  for (int i = 0; i < sizeof(data); i++) {
    for (int j = 7; j >= 0; j--) {
      int x = xstart + (i * 16 + ((7 - j) * 2));
      int x1 = x + 1;
      gsDrawSprite(x, ystart, x1, ystart + 2, 0, GS_RGBAQ(0xFF, 0x00, 0xFF, 0, 0));

      if ((data[i] >> j) & 1)
        gsDrawSprite(x1, ystart, x1 + 1, ystart + 2, 0, GS_RGBAQ(0x00, 0xFF, 0xFF, 0, 0));
      else
        gsDrawSprite(x1, ystart, x1 + 1, ystart + 2, 0, GS_RGBAQ(0xFF, 0xFF, 0x00, 0, 0));
    }
  }
}
