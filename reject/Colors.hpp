#ifndef COLORS
#define COLORS

#include <SDL2/SDL_pixels.h>

struct RGBRough {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};
struct RGBExact {
    double r;
    double g;
    double b;
    double a;
};
struct HSVRough {
    short h;
    unsigned char s;
    unsigned char v;
    unsigned char a;
};
struct HSVExact {
    double h;
    double s;
    double v;
    double a;
};
struct HSLRough {
    short h;
    unsigned char s;
    unsigned char l;
    unsigned char a;
};
struct HSLExact {
    double h;
    double s;
    double l;
    double a;
};
struct CMYKRough {
    unsigned char c;
    unsigned char m;
    unsigned char y;
    unsigned char k;
    unsigned char a;
};
struct CMYKExact {
    double c;
    double m;
    double y;
    double k;
    double a;
};

HSVRough rgb2hsv(const RGBRough &rgb);
HSLRough rgb2hsl(const RGBRough &rgb);
CMYKRough rgb2cmyk(const RGBRough &rgb);
HSVExact rgb2hsv(const RGBExact &rgb);
HSLExact rgb2hsl(const RGBExact &rgb);
CMYKExact rgb2cmyk(const RGBExact &rgb);
RGBRough hsv2rgb(const HSVRough &hsv);
HSLRough hsv2hsl(const HSVRough &hsv);
CMYKRough hsv2cmyk(const HSVRough &hsv);
RGBExact hsv2rgb(const HSVExact &hsv);
HSLExact hsv2hsl(const HSVExact &hsv);
CMYKExact hsv2cmyk(const HSVExact &hsv);
RGBRough hsl2rgb(const HSLRough &hsl);
HSVRough hsl2hsv(const HSLRough &hsl);
CMYKRough hsl2cmyk(const HSLRough &hsl);
RGBExact hsl2rgb(const HSLExact &hsl);
HSVExact hsl2hsv(const HSLExact &hsl);
CMYKExact hsl2cmyk(const HSLExact &hsl);
RGBRough cmyk2rgb(const CMYKRough &cmyk);
HSVRough cmyk2hsv(const CMYKRough &cmyk);
HSLRough cmyk2hsl(const CMYKRough &cmyk);
RGBExact cmyk2rgb(const CMYKExact &cmyk);
HSVExact cmyk2hsv(const CMYKExact &cmyk);
HSLExact cmyk2hsl(const CMYKExact &cmyk);

SDL_Color rgb2SDL(const RGBRough &rgb);
SDL_Color rgb2SDL(const RGBExact &rgb);
SDL_Color hsv2SDL(const HSVRough &hsv);
SDL_Color hsv2SDL(const HSVExact &hsv);
SDL_Color hsl2SDL(const HSLRough &hsl);
SDL_Color hsl2SDL(const HSLExact &hsl);
SDL_Color cmyk2SDL(const CMYKRough &cmyk);
SDL_Color cmyk2SDL(const CMYKExact &cmyk);

#endif /* COLORS */
