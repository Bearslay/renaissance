#ifndef COLORS
#define COLORS

#include <string>

#include "Utilities.hpp"

struct RGBf {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
};
struct HSVf {
    short         h = 0;
    unsigned char s = 0;
    unsigned char v = 1;
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

class Color {
    private:
        unsigned char R = 255;
        unsigned char G = 255;
        unsigned char B = 255;
        short         H = 0;
        unsigned char S = 0;
        unsigned char V = 255;
        unsigned char A = 255;

        void calcHSV() {           
            const unsigned char max = R > G ? (R > B ? R : B) : (G > B ? G : B);
            if (max == 0) {
                H = S = 0;
                return;
            }
            const unsigned char min = R < G ? (R < B ? R : B) : (G < B ? G : B);
            
            V = max;

            S = 255 * long(max - min) / V;
            if (S == 0) {
                H = 0;
                return;
            }

            if (max == R) {H = 60 * (G - B) / (max - min);}
            else if (max == G) {H = 85 + 60 * (B - R) / (max - min);}
            else {H = 171 + 60 * (R - G) / (max - min);}
        }

        void calcRGB() {
            if (S == 0) {
                R = G = B = V;
                return;
            }
            
            const unsigned char region = H / 60;
            const unsigned char remainder = (H - (region * 60)) * 6; 
            
            const unsigned char p = (V * (255 - S)) >> 8;
            const unsigned char q = (V * (255 - ((S * remainder) >> 8))) >> 8;
            const unsigned char t = (V * (255 - ((S * (255 - remainder)) >> 8))) >> 8;
            
            switch (region) {
                case 0:
                    R = V; G = t; B = p;
                    break;
                case 1:
                    R = q; G = V; B = p;
                    break;
                case 2:
                    R = p; G = V; B = t;
                    break;
                case 3:
                    R = p; G = q; B = V;
                    break;
                case 4:
                    R = t; G = p; B = V;
                    break;
                default:
                    R = V; G = p; B = q;
                    break;
            }
        }
    
    public:
        Color(const unsigned char &r, const unsigned char &g, const unsigned char &b) : R(r), G(g), B(b) {calcHSV();}
        Color(const short &h, const unsigned char &s, const unsigned char &v) : H(btils::normalizeDegree(h)), S(s), V(v) {calcRGB();}

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
            calcHSV();
            return output;
        }
        unsigned char setG(const unsigned char &g) {
            const unsigned char output = G;
            G = g;
            calcHSV();
            return output;
        }
        unsigned char setB(const unsigned char &b) {
            const unsigned char output = B;
            B = b;
            calcHSV();
            return output;
        }
        short         setH(const short         &h) {
            const short output = H;
            H = btils::normalizeDegree(h);
            calcRGB();
            return output;
        }
        unsigned char setS(const unsigned char &s) {
            const unsigned char output = S;
            S = s;
            calcRGB();
            return output;
        }
        unsigned char setV(const unsigned char &v) {
            const unsigned char output = V;
            V = v;
            calcRGB();
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
            calcHSV();
            return output;
        }
        unsigned char adjG(const unsigned char &g) {
            const unsigned char output = G;
            G += g;
            calcHSV();
            return output;
        }
        unsigned char adjB(const unsigned char &b) {
            const unsigned char output = B;
            B += b;
            calcHSV();
            return output;
        }
        short         adjH(const short         &h) {
            const short output = H;
            H = btils::normalizeDegree(H + h);
            calcRGB();
            return output;
        }
        unsigned char adjS(const unsigned char &s) {
            const unsigned char output = S;
            S += s;
            calcRGB();
            return output;
        }
        unsigned char adjV(const unsigned char &v) {
            const unsigned char output = V;
            V += v;
            calcRGB();
            return output;
        }
        unsigned char adjA(const unsigned char &a) {
            const unsigned char output = A;
            A += a;
            return output;
        }

        std::string rgbString() const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B);}
        std::string hsvString() const {return std::to_string(H) + ", " + std::to_string(S) + ", " + std::to_string(V);}
};

#endif /* COLORS */
