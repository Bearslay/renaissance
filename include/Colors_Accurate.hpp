#ifndef COLORS_ACCURATE
#define COLORS_ACCURATE

struct RGBa {
    double r = 1;
    double g = 1;
    double b = 1;
};
struct HSVa {
    short  h = 0;
    double s = 0;
    double v = 1;
};
struct HSLa {
    short  h = 0;
    double s = 0;
    double l = 1;
};
struct CMYKa {
    double c = 0;
    double m = 0;
    double y = 0;
    double k = 0;
};

HSVa   rgb2hsv(const RGBa  & rgb) {
    const double min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    const double max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    const double delta = max - min;
    if (delta <  0.00001) {return {0, 0.0, max};}
    if (max   <= 0.0)     {return {0, 0.0, max};}

    double h = 0.0;
    if      (rgb.r >= max) {h =       (rgb.g - rgb.b) / delta;}
    else if (rgb.g >= max) {h = 2.0 + (rgb.b - rgb.r) / delta;}
    else                   {h = 4.0 + (rgb.r - rgb.g) / delta;}

    if ((h *= 60.0) < 0.0) {h += 360.0;}
    return {(short)h, delta / max, max};
}
CMYKa rgb2cmyk(const RGBa  & rgb) {
    const double k = 1.0 - ((rgb.r >= rgb.g ? rgb.r : rgb.g) >= rgb.b ? (rgb.r >= rgb.g ? rgb.r : rgb.g) : rgb.b);
    return {(1.0 - rgb.r - k) / (1.0 - k), (1.0 - rgb.g - k) / (1.0 - k), (1.0 - rgb.b - k) / (1.0 - k), k};
}
RGBa   hsv2rgb(const HSVa  & hsv) {
    if (hsv.s <= 0.0) {return {hsv.v, hsv.v, hsv.v};}

    const double h = (hsv.h >= 360.0 ? 0.0 : hsv.h) / 60.0;
    const long region = (long)h;
    const double p = hsv.v * (1.0 -  hsv.s);
    const double q = hsv.v * (1.0 - (hsv.s *        (h - region)));
    const double t = hsv.v * (1.0 - (hsv.s * (1.0 - (h - region))));

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
HSLa   hsv2hsl(const HSVa  & hsv) {
    const double l = hsv.v * (1.0 - hsv.s / 2.0);
    return {hsv.h, l <= 0.0 || l >= 1.0 ? 0.0 : ((hsv.v - l) / (l <= 1.0 - l ? l : 1.0 - l)), l};
}
HSVa   hsl2hsv(const HSLa  & hsl) {
    const double v = hsl.l + hsl.s * (hsl.l <= 1.0 - hsl.l ? hsl.l : 1.0 - hsl.l);
    return {hsl.h, v == 0.0 ? 0.0 : 2 * (1.0 - hsl.l / v), v};
}
RGBa  cmyk2rgb(const CMYKa &cmyk) {return {(1.0 - cmyk.c) * (1.0 - cmyk.k), (1.0 - cmyk.m) * (1.0 - cmyk.k), (1.0 - cmyk.y) * (1.0 - cmyk.k)};}
HSLa   rgb2hsl(const RGBa  & rgb) {return  hsv2hsl( rgb2hsv( rgb));}
CMYKa hsv2cmyk(const HSVa  & hsv) {return rgb2cmyk( hsv2rgb( hsv));}
RGBa   hsl2rgb(const HSLa  & hsl) {return  hsv2rgb( hsl2hsv( hsl));}
CMYKa hsl2cmyk(const HSLa  & hsl) {return rgb2cmyk( hsl2rgb( hsl));}
HSVa  cmyk2hsv(const CMYKa &cmyk) {return  rgb2hsv(cmyk2rgb(cmyk));}
HSLa  cmyk2hsl(const CMYKa &cmyk) {return  rgb2hsl(cmyk2rgb(cmyk));}

#endif /* COLORS_ACCURATE */
