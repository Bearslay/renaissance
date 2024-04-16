#ifndef COLORS
#define COLORS

#include <cmath>

struct ColorRGBA {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255;
};

struct ColorHSVA {
    short         h = 0;
    unsigned char s = 0;
    unsigned char v = 255;
    unsigned char a = 255;
};

struct ColorRGB {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
};

struct ColorHSV {
    short         h = 0;
    unsigned char s = 0;
    unsigned char v = 255;
};

class Color {
    private:
        unsigned char R = 255;
        unsigned char G = 255;
        unsigned char B = 255;
        short         H = 0;
        unsigned char S = 0;
        unsigned char V = 255;
        unsigned char A = 255;

        short mapAngle(const short &angle) {
            short output = angle;
            while (angle < 0)   {output += 360;}
            while (angle > 360) {output += 360;}
            return output;
        }

        void calcHSV() {

        }

        void calcRGB() {

        }
    
    public:
        Color(const unsigned char &r, const unsigned char &g, const unsigned char &b) : R(r), G(g), B(b) {calcHSV();}
        Color(const short &h, const unsigned char &s, const unsigned char &v) : H(mapAngle(h)), S(s), V(v) {calcRGB();}

        unsigned char getR() const {return R;}
        unsigned char getG() const {return G;}
        unsigned char getB() const {return B;}
        short         getH() const {return H;}
        unsigned char getS() const {return S;}
        unsigned char getV() const {return V;}
        unsigned char getA() const {return A;}

        unsigned char setR(const unsigned char &r) {
            const unsigned char output = R;
            R = r;
            return output;
        }
        unsigned char setG(const unsigned char &g) {
            const unsigned char output = G;
            G = g;
            return output;
        }
        unsigned char setB(const unsigned char &b) {
            const unsigned char output = B;
            B = b;
            return output;
        }
        short         setH(const short         &h) {
            const short output = H;
            H = mapAngle(h);
            return output;
        }
        unsigned char setS(const unsigned char &s) {
            const unsigned char output = S;
            S = s;
            return output;
        }
        unsigned char setV(const unsigned char &v) {
            const unsigned char output = V;
            V = v;
            return output;
        }
        unsigned char setA(const unsigned char &a) {
            const unsigned char output = A;
            A = a;
            return output;
        }

        unsigned char adjR(const unsigned char &r) {
            const unsigned char output = R;
            R += r;
            return output;
        }
        unsigned char adjG(const unsigned char &g) {
            const unsigned char output = G;
            G += g;
            return output;
        }
        unsigned char adjB(const unsigned char &b) {
            const unsigned char output = B;
            B += b;
            return output;
        }
        short         adjH(const short         &h) {
            const short output = H;
            H = mapAngle(H + h);
            return output;
        }
        unsigned char adjS(const unsigned char &s) {
            const unsigned char output = S;
            S += s;
            return output;
        }
        unsigned char adjV(const unsigned char &v) {
            const unsigned char output = V;
            V += v;
            return output;
        }
        unsigned char adjA(const unsigned char &a) {
            const unsigned char output = A;
            A += a;
            return output;
        }
};

// ColorHSV toHSV(const ColorRGB &rgb) {
//     const unsigned char min = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
//     const unsigned char max = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

//     const double delta = (double)max / 255 - (double)min / 255;
//     if (delta < 0.00001) {return {0, 0, max, rgb.a};}
//     if (max == 0)        {return {0, 0, max, rgb.a};}

//     double h = (rgb.r >= max ? rgb.g - rgb.b : (rgb.g >= max ? rgb.b - rgb.r : rgb.r - rgb.g)) / 255 / delta;
    
//     h *= 60;
//     if (h < 0) {h += 360;}

//     return {(unsigned char)(h * 255), (unsigned char)(delta * 255 / max), max, rgb.a};
// }

// ColorHSV toHSV(const ColorRGB &rgb) {
//     ColorHSV hsv;
//     unsigned char rgbMin, rgbMax;

//     rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
//     rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
    
//     hsv.v = rgbMax;
//     if (hsv.v == 0)
//     {
//         hsv.h = 0;
//         hsv.s = 0;
//         return hsv;
//     }

//     hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
//     if (hsv.s == 0)
//     {
//         hsv.h = 0;
//         return hsv;
//     }

//     if (rgbMax == rgb.r)
//         hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
//     else if (rgbMax == rgb.g)
//         hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
//     else
//         hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

//     return hsv;
// }

// ColorRGB toRGB(const ColorHSV &hsv) {
//     ColorRGB rgb;
//     short region, remainder;
//     unsigned char p, q, t;
    
//     if (hsv.s == 0) {
//         rgb.r = hsv.v;
//         rgb.g = hsv.v;
//         rgb.b = hsv.v;
//         return rgb;
//     }
    
//     region = hsv.h / 43;
//     remainder = (hsv.h - (region * 43)) * 6; 
    
//     p = (hsv.v * (255 - hsv.s)) >> 8;
//     q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
//     t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;
    
//     switch (region) {
//         case 0:
//             rgb.r = hsv.v;
//             rgb.g = t;
//             rgb.b = p;
//             break;
//         case 1:
//             rgb.r = q;
//             rgb.g = hsv.v;
//             rgb.b = p;
//             break;
//         case 2:
//             rgb.r = p;
//             rgb.g = hsv.v;
//             rgb.b = t;
//             break;
//         case 3:
//             rgb.r = p;
//             rgb.g = q;
//             rgb.b = hsv.v;
//             break;
//         case 4:
//             rgb.r = t;
//             rgb.g = p;
//             rgb.b = hsv.v;
//             break;
//         case 5:
//         default:
//             rgb.r = hsv.v;
//             rgb.g = p;
//             rgb.b = q;
//             break;
//     }
    
//     return rgb;
// }

ColorRGB toRGB(const ColorHSV &hsv) {
    ColorRGB rgb;
    double s = hsv.s / 255;
    double v = hsv.v / 255;

    const double C = v * s;
    const double X = C * (1 - std::fabs(std::fmod((double)hsv.h / 60, 2) - 1));
    const double m = v - C;
}

#endif /* COLORS */
