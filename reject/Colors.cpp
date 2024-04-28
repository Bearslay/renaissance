#include "Colors.hpp"

HSVRough rgb2hsv(const RGBRough &rgb) {
    const unsigned char max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    if (max == 0) {return {0, 0, max, rgb.a};}
    
    const unsigned char min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);

    const unsigned char s = 255 * long(max - min) / max;
    if (s == 0) {return {0, s, max, rgb.a};}

    short h = 0;
    if (max == rgb.r) {h = 60 * (rgb.g - rgb.b) / (max - min);}
    else if (max == rgb.g) {h = 85  + 60 * (rgb.b - rgb.r) / (max - min);}
    else {h = 171 + 60 * (rgb.r - rgb.g) / (max - min);}

    return {h, s, max, rgb.a};
}
HSLRough rgb2hsl(const RGBRough &rgb) {return hsv2hsl(rgb2hsv(rgb));}
CMYKRough rgb2cmyk(const RGBRough &rgb) {
    const unsigned char k = 255 - ((rgb.r >= rgb.g ? rgb.r : rgb.g) >= rgb.b ? (rgb.r >= rgb.g ? rgb.r : rgb.g) : rgb.b);
    return {(unsigned char)((255 - rgb.r - k) / (255 - k)), (unsigned char)((255 - rgb.g - k) / (255 - k)), (unsigned char)((255 - rgb.b - k) / (255 - k)), k, rgb.a};
}
RGBRough hsv2rgb(const HSVRough &hsv) {
    if (hsv.s == 0) {return {hsv.v, hsv.v, hsv.v, hsv.a};}
    
    const unsigned char region = hsv.h / 60;
    const unsigned char remainder = (hsv.h - (region * 60)) * 6; 
    const unsigned char p = (hsv.v * (255 - hsv.s)) >> 8;
    const unsigned char q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    const unsigned char t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
    
    switch (region) {
        case 0:
            return {hsv.v, t, p, hsv.a};
            break;
        case 1:
            return {q, hsv.v, p, hsv.a};
            break;
        case 2:
            return {p, hsv.v, t, hsv.a};
            break;
        case 3:
            return {p, q, hsv.v, hsv.a};
            break;
        case 4:
            return {t, p, hsv.v, hsv.a};
            break;
        default:
            return {hsv.v, p, q, hsv.a};
            break;
    }
}
HSLRough hsv2hsl(const HSVRough &hsv) {
    const unsigned char l = hsv.v * (255 - hsv.s / 2);
    return {hsv.h, (unsigned char)(l <= 0 || l >= 255 ? 0 : ((hsv.v - l) / (l <= 255 - l ? l : 255 - l))), l, hsv.a};
}
CMYKRough hsv2cmyk(const HSVRough &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBRough hsl2rgb(const HSLRough &hsl) {return hsv2rgb(hsl2hsv(hsl));}
HSVRough hsl2hsv(const HSLRough &hsl) {
    const unsigned char v = hsl.l + hsl.s * (hsl.l <= 255 - hsl.l ? hsl.l : 255 - hsl.l);
    return {hsl.h, (unsigned char)(v == 0 ? 0 : 2 * (255 - hsl.l / v)), v, hsl.a};
}
CMYKRough hsl2cmyk(const HSLRough &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
RGBRough cmyk2rgb(const CMYKRough &cmyk) {return {(unsigned char)((255 - cmyk.c) * (255 - cmyk.k)), (unsigned char)((255 - cmyk.m) * (255 - cmyk.k)), (unsigned char)((255 - cmyk.y) * (255 - cmyk.k)), cmyk.a};}
HSVRough cmyk2hsv(const CMYKRough &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLRough cmyk2hsl(const CMYKRough &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}

HSVExact rgb2hsv(const RGBExact &rgb) {
    const double min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    const double max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    const double delta = max - min;
    if (delta < 0.00001) {return {0.0, 0.0, max, rgb.a};}
    if (max <= 0.0) {return {0.0, 0.0, max, rgb.a};}

    double h = 0.0;
    if (rgb.r >= max) {h = (rgb.g - rgb.b) / delta;}
    else if (rgb.g >= max) {h = 2.0 + (rgb.b - rgb.r) / delta;}
    else {h = 4.0 + (rgb.r - rgb.g) / delta;}

    if ((h *= 60.0) < 0.0) {h += 360.0;}
    return {h, delta / max, max, rgb.a};
}
HSLExact rgb2hsl(const RGBExact &rgb) {return hsv2hsl(rgb2hsv(rgb));}
CMYKExact rgb2cmyk(const RGBExact &rgb) {
    const double k = 1.0 - ((rgb.r >= rgb.g ? rgb.r : rgb.g) >= rgb.b ? (rgb.r >= rgb.g ? rgb.r : rgb.g) : rgb.b);
    return {(1.0 - rgb.r - k) / (1.0 - k), (1.0 - rgb.g - k) / (1.0 - k), (1.0 - rgb.b - k) / (1.0 - k), k, rgb.a};
}
RGBExact hsv2rgb(const HSVExact &hsv) {
    if (hsv.s <= 0.0) {return {hsv.v, hsv.v, hsv.v, hsv.a};}

    const double h = (hsv.h >= 360.0 ? 0.0 : hsv.h) / 60.0;
    const long region = (long)h;
    const double p = hsv.v * (1.0 - hsv.s);
    const double q = hsv.v * (1.0 - (hsv.s * (h - region)));
    const double t = hsv.v * (1.0 - (hsv.s * (1.0 - (h - region))));

    switch (region) {
        case 0:
            return {hsv.v, t, p, hsv.a};
            break;
        case 1:
            return {q, hsv.v, p, hsv.a};
            break;
        case 2:
            return {p, hsv.v, t, hsv.a};
            break;
        case 3:
            return {p, q, hsv.v, hsv.a};
            break;
        case 4:
            return {t, p, hsv.v, hsv.a};
            break;
        default:
            return {hsv.v, p, q, hsv.a};
            break;
    }
}
HSLExact hsv2hsl(const HSVExact &hsv) {
    const double l = hsv.v * (1.0 - hsv.s / 2.0);
    return {hsv.h, l <= 0.0 || l >= 1.0 ? 0.0 : ((hsv.v - l) / (l <= 1.0 - l ? l : 1.0 - l)), l, hsv.a};
}
CMYKExact hsv2cmyk(const HSVExact &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBExact hsl2rgb(const HSLExact &hsl) {return hsv2rgb(hsl2hsv(hsl));}
HSVExact hsl2hsv(const HSLExact &hsl) {
    const double v = hsl.l + hsl.s * (hsl.l <= 1.0 - hsl.l ? hsl.l : 1.0 - hsl.l);
    return {hsl.h, v == 0.0 ? 0.0 : 2 * (1.0 - hsl.l / v), v, hsl.a};
}
CMYKExact hsl2cmyk(const HSLExact &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
RGBExact cmyk2rgb(const CMYKExact &cmyk) {return {(1.0 - cmyk.c) * (1.0 - cmyk.k), (1.0 - cmyk.m) * (1.0 - cmyk.k), (1.0 - cmyk.y) * (1.0 - cmyk.k), cmyk.a};}
HSVExact cmyk2hsv(const CMYKExact &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLExact cmyk2hsl(const CMYKExact &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}

SDL_Color rgb2SDL(const RGBRough &rgb) {return {rgb.r, rgb.g, rgb.b, rgb.a};}
SDL_Color rgb2SDL(const RGBExact &rgb) {return {(unsigned char)(rgb.r * 255.0), (unsigned char)(rgb.g * 255.0), (unsigned char)(rgb.b * 255.0), (unsigned char)(rgb.a * 255.0)};}
SDL_Color hsv2SDL(const HSVRough &hsv) {return rgb2SDL(hsv2rgb(hsv));}
SDL_Color hsv2SDL(const HSVExact &hsv) {return rgb2SDL(hsv2rgb(hsv));}
SDL_Color hsl2SDL(const HSLRough &hsl) {return rgb2SDL(hsl2rgb(hsl));}
SDL_Color hsl2SDL(const HSLExact &hsl) {return rgb2SDL(hsl2rgb(hsl));}
SDL_Color cmyk2SDL(const CMYKRough &cmyk) {return rgb2SDL(cmyk2rgb(cmyk));}
SDL_Color cmyk2SDL(const CMYKExact &cmyk) {return rgb2SDL(cmyk2rgb(cmyk));}
