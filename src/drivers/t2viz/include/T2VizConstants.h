/* -*- C++ -*- */
#ifndef T2VIZCONSTANTS_H
#define T2VIZCONSTANTS_H

#define T2_SCREEN_WIDTH 480
#define T2_SCREEN_HEIGHT 320

#define ROOT_WIDTH_CROP 8   /* Bezel covers some pixels on right */
#define ROOT_WINDOW_WIDTH (T2_SCREEN_WIDTH-ROOT_WIDTH_CROP) 
#define ROOT_WINDOW_HEIGHT 320

namespace MFM {
#define makeColor(R,G,B) ((0xff<<24)|(((R)&0xff)<<16)|(((G)&0xff)<<8)|(((B)&0xff)<<0))
  enum T2_COLOR {
    T2_COLOR_BLACK = makeColor(1,3,1),
    T2_COLOR_DARK_GREY = makeColor(63,63,63),
    T2_COLOR_MEDIUM_GREY = makeColor(127,127,127),
    T2_COLOR_LIGHT_GREY = makeColor(190,190,190),
    T2_COLOR_RED = makeColor(255,4,4),
    T2_COLOR_MEDIUM_RED = makeColor(200,4,4),
    T2_COLOR_DARK_RED = makeColor(127,4,4),
    T2_COLOR_BLUE = makeColor(4,4,255),
    T2_COLOR_DARK_BLUE = makeColor(4,4,127),
    T2_COLOR_LIGHT_BLUE = makeColor(127,127,255),
    T2_COLOR_YELLOW = makeColor(255,255,0),
    T2_COLOR_DARK_YELLOW = makeColor(127,127,0),
    T2_COLOR_CYAN = makeColor(0,255,255),

    T2_COLOR_BKGD_RED = makeColor(10,0,0),
    T2_COLOR_BKGD_GREEN = makeColor(0,10,0),
    T2_COLOR_BKGD_BLUE = makeColor(0,0,10),
    T2_COLOR_BKGD_YELLOW = makeColor(10,10,0),

    T2_END_OF_LIST_UNUSED
  };
#undef makeColor

  enum Constants {
    ITC_IN_PIXELS = 32,
    ITC_ACROSS_PIXELS = 2*ITC_IN_PIXELS,

    LOG_WINDOW_WIDTH = ROOT_WINDOW_WIDTH,
    LOG_WINDOW_HEIGHT = 42,
    LOG_WINDOW_XPOS = 0,
    LOG_WINDOW_YPOS = ROOT_WINDOW_HEIGHT - ITC_IN_PIXELS - LOG_WINDOW_HEIGHT,

    CORNER_LIGHT_WINDOW_WIDTH = 5,
    CORNER_LIGHT_WINDOW_HEIGHT = 5,

    CONTENT_WINDOW_WIDTH = ROOT_WINDOW_WIDTH - 2 * ITC_IN_PIXELS,
    CONTENT_WINDOW_HEIGHT = ROOT_WINDOW_HEIGHT - 2 * ITC_IN_PIXELS - LOG_WINDOW_HEIGHT,
    CONTENT_WINDOW_XPOS = ITC_IN_PIXELS + 1,
    CONTENT_WINDOW_YPOS = ITC_IN_PIXELS + 1,
    STATUS_WINDOW_WIDTH = 115,
    STATUS_WINDOW_HEIGHT = 210,

    STATIC_WINDOW_WIDTH = 128,
    STATIC_WINDOW_HEIGHT = 32,
    STATIC_WINDOW_XPOS = (ROOT_WINDOW_WIDTH - STATIC_WINDOW_WIDTH)/2,
    STATIC_WINDOW_YPOS = ROOT_WINDOW_HEIGHT - STATIC_WINDOW_HEIGHT,

    DELTAS_WINDOW_WIDTH = CONTENT_WINDOW_WIDTH - STATUS_WINDOW_WIDTH,
    DELTAS_WINDOW_HEIGHT = CONTENT_WINDOW_HEIGHT,
    ITC_COUNT = 6,
    ROSE_DIR_COUNT = 8  // Including (T2 non-existent) NT and ST
  };
}

#endif /* T2VIZCONSTANTS_H */
