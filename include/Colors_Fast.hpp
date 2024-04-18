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

HSVf rgb2hsv(const RGBf &rgb) {
    const unsigned char max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    if (max == 0) {return {0, 0, max};}
    
    const unsigned char min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);

    const unsigned char s = 255 * long(max - min) / max;
    if (s == 0) {return {0, s, max};}

    short h = 0;
    if      (max == rgb.r) {h =       60 * (rgb.g - rgb.b) / (max - min);}
    else if (max == rgb.g) {h = 85  + 60 * (rgb.b - rgb.r) / (max - min);}
    else                   {h = 171 + 60 * (rgb.r - rgb.g) / (max - min);}

    return {h, s, max};
}
CMYKf rgb2cmyk(const RGBf  & rgb) {
    const unsigned char k = 255 - ((rgb.r >= rgb.g ? rgb.r : rgb.g) >= rgb.b ? (rgb.r >= rgb.g ? rgb.r : rgb.g) : rgb.b);
    return {(255 - rgb.r - k) / (255 - k), (255 - rgb.g - k) / (255 - k), (255 - rgb.b - k) / (255 - k), k};
}
RGBf hsv2rgb(const HSVf &hsv) {
    if (hsv.s == 0) {return {hsv.v, hsv.v, hsv.v};}
    
    const unsigned char region = hsv.h / 60;
    const unsigned char remainder = (hsv.h - (region * 60)) * 6; 
    const unsigned char p = (hsv.v * (255 -   hsv.s))                    >> 8;
    const unsigned char q = (hsv.v * (255 - ((hsv.s *        remainder)  >> 8))) >> 8;
    const unsigned char t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
    
    switch (region) {
        case 0:
            return {hsv.v, t, p};
            break;
        case 1:
            return {q, hsv.v, p};
            break;
        case 2:
            return {p, hsv.v, t};
            break;
        case 3:
            return {p, q, hsv.v};
            break;
        case 4:
            return {t, p, hsv.v};
            break;
        default:
            return {hsv.v, p, q};
            break;
    }
}
HSLf   hsv2hsl(const HSVf  & hsv) {
    const unsigned char l = hsv.v * (255 - hsv.s / 2);
    return {hsv.h, l <= 0 || l >= 255 ? 0 : ((hsv.v - l) / (l <= 255 - l ? l : 255 - l)), l};
}
HSVf   hsl2hsv(const HSLf  & hsl) {
    const unsigned char v = hsl.l + hsl.s * (hsl.l <= 255 - hsl.l ? hsl.l : 255 - hsl.l);
    return {hsl.h, v == 0 ? 0 : 2 * (255 - hsl.l / v), v};
}
RGBf  cmyk2rgb(const CMYKf &cmyk) {return {(255 - cmyk.c) * (255 - cmyk.k), (255 - cmyk.m) * (255 - cmyk.k), (255 - cmyk.y) * (255 - cmyk.k)};}
HSLf   rgb2hsl(const RGBf  & rgb) {return  hsv2hsl( rgb2hsv( rgb));}
CMYKf hsv2cmyk(const HSVf  & hsv) {return rgb2cmyk( hsv2rgb( hsv));}
RGBf   hsl2rgb(const HSLf  & hsl) {return  hsv2rgb( hsl2hsv( hsl));}
CMYKf hsl2cmyk(const HSLf  & hsl) {return rgb2cmyk( hsl2rgb( hsl));}
HSVf  cmyk2hsv(const CMYKf &cmyk) {return  rgb2hsv(cmyk2rgb(cmyk));}
HSLf  cmyk2hsl(const CMYKf &cmyk) {return  rgb2hsl(cmyk2rgb(cmyk));}

#endif /* COLORS_FAST */
