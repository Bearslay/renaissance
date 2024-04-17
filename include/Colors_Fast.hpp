#ifndef COLORS_FAST
#define COLORS_FAST

struct RGBf {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
};

struct HSVf {
    short h = 0;
    unsigned char s = 0;
    unsigned char v = 255;
};

struct HSLf {
    short h = 0;
    unsigned char s = 0;
    unsigned char l = 255;
};

struct CMYKf {
    unsigned char c = 0;
    unsigned char m = 0;
    unsigned char y = 0;
    unsigned char k = 0;
};

#endif /* COLORS_FAST */
