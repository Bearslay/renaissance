#ifndef BCOLORS
#define BCOLORS

#include "Utilities.hpp"

typedef enum {
    RGB_RELATE_COMMON,
    RGB_RELATE_RED,
    RGB_RELATE_GREEN,
    RGB_RELATE_BLUE,
    RGB_RELATE_ALPHA
} RelationsRGB;
typedef enum {
    HSV_RELATE_COMMON,
    HSV_RELATE_HUE,
    HSV_RELATE_SATURATION,
    HSV_RELATE_VALUE,
    HSV_RELATE_ALPHA
} RelationsHSV;
typedef enum {
    HSL_RELATE_COMMON,
    HSL_RELATE_HUE,
    HSL_RELATE_SATURATION,
    HSL_RELATE_LIGHTNESS,
    HSL_RELATE_ALPHA
} RelationsHSL;
typedef enum {
    CMYK_RELATE_COMMON,
    CMYK_RELATE_CYAN,
    CMYK_RELATE_MAGENTA,
    CMYK_RELATE_YELLOW,
    CMYK_RELATE_KEY,
    CMYK_RELATE_ALPHA
} RelationsCMYK;

class RGBRough;
class RGBExact;
class HSVRough;
class HSVExact;
class HSLRough;
class HSLExact;
class CMYKRough;
class CMYKExact;

HSVRough rgb2hsv(const RGBRough &rgb) {
    const unsigned char max = rgb.getR() > rgb.getG() ? (rgb.getR() > rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() > rgb.getB() ? rgb.getG() : rgb.getB());
    if (max == 0) {return HSVRough(0, 0, max, rgb.getA());}
    
    const unsigned char min = rgb.getR() < rgb.getG() ? (rgb.getR() < rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() < rgb.getB() ? rgb.getG() : rgb.getB());

    const unsigned char s = 255 * long(max - min) / max;
    if (s == 0) {return HSVRough(0, s, max, rgb.getA());}

    short h = 0;
    if (max == rgb.getR()) {h = 60 * (rgb.getG() - rgb.getB()) / (max - min);}
    else if (max == rgb.getG()) {h = 85  + 60 * (rgb.getB() - rgb.getR()) / (max - min);}
    else {h = 171 + 60 * (rgb.getR() - rgb.getG()) / (max - min);}

    return HSVRough(h, s, max, rgb.getA());
}
CMYKRough rgb2cmyk(const RGBRough &rgb) {
    const unsigned char k = 255 - ((rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) >= rgb.getB() ? (rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) : rgb.getB());
    return CMYKRough((255 - rgb.getR() - k) / (255 - k), (255 - rgb.getG() - k) / (255 - k), (255 - rgb.getB() - k) / (255 - k), k, rgb.getA());
}
RGBRough hsv2rgb(const HSVRough &hsv) {
    if (hsv.getS() == 0) {return RGBRough(hsv.getV(), hsv.getV(), hsv.getV(), hsv.getA());}
    
    const unsigned char region = hsv.getH() / 60;
    const unsigned char remainder = (hsv.getH() - (region * 60)) * 6; 
    const unsigned char p = (hsv.getV() * (255 - hsv.getS())) >> 8;
    const unsigned char q = (hsv.getV() * (255 - ((hsv.getS() * remainder) >> 8))) >> 8;
    const unsigned char t = (hsv.getV() * (255 - ((hsv.getS() * (255 - remainder)) >> 8))) >> 8;
    
    switch (region) {
        case 0:
            return RGBRough(hsv.getV(), t, p, hsv.getA());
            break;
        case 1:
            return RGBRough(q, hsv.getV(), p, hsv.getA());
            break;
        case 2:
            return RGBRough(p, hsv.getV(), t, hsv.getA());
            break;
        case 3:
            return RGBRough(p, q, hsv.getV(), hsv.getA());
            break;
        case 4:
            return RGBRough(t, p, hsv.getV(), hsv.getA());
            break;
        default:
            return RGBRough(hsv.getV(), p, q, hsv.getA());
            break;
    }
}
HSLRough hsv2hsl(const HSVRough &hsv) {
    const unsigned char l = hsv.getV() * (255 - hsv.getS() / 2);
    return HSLRough(hsv.getH(), l <= 0 || l >= 255 ? 0 : ((hsv.getV() - l) / (l <= 255 - l ? l : 255 - l)), l, hsv.getA());
}
HSVRough hsl2hsv(const HSLRough &hsl) {
    const unsigned char v = hsl.getL() + hsl.getS() * (hsl.getL() <= 255 - hsl.getL() ? hsl.getL() : 255 - hsl.getL());
    return HSVRough(hsl.getH(), v == 0 ? 0 : 2 * (255 - hsl.getL() / v), v, hsl.getA());
}
RGBRough cmyk2rgb(const CMYKRough &cmyk) {return RGBRough((255 - cmyk.getC()) * (255 - cmyk.getK()), (255 - cmyk.getM()) * (255 - cmyk.getK()), (255 - cmyk.getY()) * (255 - cmyk.getK()), cmyk.getA());}
HSLRough rgb2hsl(const RGBRough &rgb) {return hsv2hsl(rgb2hsv(rgb));}
CMYKRough hsv2cmyk(const HSVRough &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBRough hsl2rgb(const HSLRough &hsl) {return hsv2rgb(hsl2hsv(hsl));}
CMYKRough hsl2cmyk(const HSLRough &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
HSVRough cmyk2hsv(const CMYKRough &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLRough cmyk2hsl(const CMYKRough &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}

HSVExact rgb2hsv(const RGBExact &rgb) {
    const double min = rgb.getR() < rgb.getG() ? (rgb.getR() < rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() < rgb.getB() ? rgb.getG() : rgb.getB());
    const double max = rgb.getR() > rgb.getG() ? (rgb.getR() > rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() > rgb.getB() ? rgb.getG() : rgb.getB());

    const double delta = max - min;
    if (delta < 0.00001) {return {0, 0.0, max};}
    if (max <= 0.0) {return {0, 0.0, max};}

    double h = 0.0;
    if (rgb.getR() >= max) {h = (rgb.getG() - rgb.getB()) / delta;}
    else if (rgb.getG() >= max) {h = 2.0 + (rgb.getB() - rgb.getR()) / delta;}
    else {h = 4.0 + (rgb.getR() - rgb.getG()) / delta;}

    if ((h *= 60.0) < 0.0) {h += 360.0;}
    return {(short)h, delta / max, max};
}
CMYKExact rgb2cmyk(const RGBExact &rgb) {
    const double k = 1.0 - ((rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) >= rgb.getB() ? (rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) : rgb.getB());
    return {(1.0 - rgb.getR() - k) / (1.0 - k), (1.0 - rgb.getG() - k) / (1.0 - k), (1.0 - rgb.getB() - k) / (1.0 - k), k};
}
RGBExact hsv2rgb(const HSVExact &hsv) {
    if (hsv.getS() <= 0.0) {return {hsv.getV(), hsv.getV(), hsv.getV()};}

    const double h = (hsv.getH() >= 360.0 ? 0.0 : hsv.getH()) / 60.0;
    const long region = (long)h;
    const double p = hsv.getV() * (1.0 - hsv.getS());
    const double q = hsv.getV() * (1.0 - (hsv.getS() * (h - region)));
    const double t = hsv.getV() * (1.0 - (hsv.getS() * (1.0 - (h - region))));

    switch (region) {
        case 0:
            return {hsv.getV(), t, p};
            break;
        case 1:
            return {q, hsv.getV(), p};
            break;
        case 2:
            return {p, hsv.getV(), t};
            break;
        case 3:
            return {p, q, hsv.getV()};
            break;
        case 4:
            return {t, p, hsv.getV()};
            break;
        default:
            return {hsv.getV(), p, q};
            break;
    }
}
HSLExact hsv2hsl(const HSVExact &hsv) {
    const double l = hsv.getV() * (1.0 - hsv.getS() / 2.0);
    return {hsv.getH(), l <= 0.0 || l >= 1.0 ? 0.0 : ((hsv.getV() - l) / (l <= 1.0 - l ? l : 1.0 - l)), l};
}
HSVExact hsl2hsv(const HSLExact &hsl) {
    const double v = hsl.getL() + hsl.getS() * (hsl.getL() <= 1.0 - hsl.getL() ? hsl.getL() : 1.0 - hsl.getL());
    return {hsl.getH(), v == 0.0 ? 0.0 : 2 * (1.0 - hsl.getL() / v), v};
}
RGBExact cmyk2rgb(const CMYKExact &cmyk) {return {(1.0 - cmyk.getC()) * (1.0 - cmyk.getK()), (1.0 - cmyk.getM()) * (1.0 - cmyk.getK()), (1.0 - cmyk.getY()) * (1.0 - cmyk.getK())};}
HSLExact rgb2hsl(const RGBExact &rgb) {return hsv2hsl(rgb2hsv(rgb));}
CMYKExact hsv2cmyk(const HSVExact &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBExact hsl2rgb(const HSLExact &hsl) {return hsv2rgb(hsl2hsv(hsl));}
CMYKExact hsl2cmyk(const HSLExact &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
HSVExact cmyk2hsv(const CMYKExact &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLExact cmyk2hsl(const CMYKExact &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}

class RGBRough {
    private:
        unsigned char R = 255;
        unsigned char G = 255;
        unsigned char B = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a) : R(r), G(g), B(b), A(a) {}
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b) {RGBRough(r, g, b, 255);}
        
        RGBRough(const RGBRough &color) : R(color.getR()), G(color.getG()), B(color.getB()), A(color.getA()) {}
        RGBRough(const RGBExact &color) : R(std::round(color.getR() * 255.0)), G(std::round(color.getG() * 255.0)), B(std::round(color.getB() * 255.0)), A(std::round(color.getA() * 255.0)) {}
        RGBRough(const HSVRough &color) {RGBRough(hsv2rgb(color));}
        RGBRough(const HSVExact &color) {RGBRough(hsv2rgb(color));}
        RGBRough(const HSLRough &color) {RGBRough(hsl2rgb(color));}
        RGBRough(const HSLExact &color) {RGBRough(hsl2rgb(color));}
        RGBRough(const CMYKRough &color) {RGBRough(cmyk2rgb(color));}
        RGBRough(const CMYKExact &color) {RGBRough(cmyk2rgb(color));}

        #ifdef SDL_pixels_h_
        RGBRough(const SDL_Color &color) : R(color.r), G(color.g), B(color.b), A(color.a) {}
        SDL_Color toSDL() const {return {R, G, B, A};}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        unsigned char getR() const {return R;}
        unsigned char getG() const {return G;}
        unsigned char getB() const {return B;}
        unsigned char getA() const {return A;}

        unsigned char setR(const unsigned char &r) {return btils::set<unsigned char>(R, r);}
        unsigned char setG(const unsigned char &g) {return btils::set<unsigned char>(G, g);}
        unsigned char setB(const unsigned char &b) {return btils::set<unsigned char>(B, b);}
        unsigned char setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

        unsigned char adjR(const unsigned char &amount) {return setR(R + amount);}
        unsigned char adjG(const unsigned char &amount) {return setG(G + amount);}
        unsigned char adjB(const unsigned char &amount) {return setB(B + amount);}
        unsigned char adjA(const unsigned char &amount) {return setA(A + amount);}

        void operator=(const RGBRough &color) {
            R = color.getR();
            G = color.getG();
            B = color.getB();
            A = color.getA();
        }
        RGBRough operator!() const {return RGBRough(255 - R, 255 - G, 255 - B, A);}
        RGBExact toExact() const {return RGBExact((double)(R / 255.0), (double)(G / 255.0), (double)(B / 255.0), (double)(A / 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case RGB_RELATE_COMMON:
                    return R == color.getR() && G == color.getG() && B == color.getB();
                case RGB_RELATE_RED:
                    return R == color.getR();
                case RGB_RELATE_GREEN:
                    return G == color.getG();
                case RGB_RELATE_BLUE:
                    return B == color.getB();
                case RGB_RELATE_ALPHA:
                    return A == color.getA();
            }
        }
        bool notequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const RGBRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case RGB_RELATE_COMMON:
                    return R * R + G * G + B * B < color.getR() * color.getR() + color.getG() * color.getG() + color.getB() * color.getB();
                case RGB_RELATE_RED:
                    return R < color.getR();
                case RGB_RELATE_GREEN:
                    return G < color.getG();
                case RGB_RELATE_BLUE:
                    return B < color.getB();
                case RGB_RELATE_ALPHA:
                    return A < color.getA();
            }
        }
        bool greater(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const RGBRough &color) const {return equal(color, RelationMetric);}
        bool operator!=(const RGBRough &color) const {return notequal(color, RelationMetric);}
        bool operator<(const RGBRough &color) const {return less(color, RelationMetric);}
        bool operator>(const RGBRough &color) const {return greater(color, RelationMetric);}
        bool operator<=(const RGBRough &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const RGBRough &color) const {return greaterequal(color, RelationMetric);}

        RGBRough& operator+=(const RGBRough &color) {
            R += color.getR();
            G += color.getG();
            B += color.getB();
            return *this;
        }
        RGBRough& operator-=(const RGBRough &color) {
            R -= color.getR();
            G -= color.getG();
            B -= color.getB();
            return *this;
        }
        RGBRough& operator*=(const unsigned char &scalar) {
            R *= scalar;
            G *= scalar;
            B *= scalar;
            return *this;
        }
        RGBRough& operator/=(const unsigned char &scalar) {
            R /= scalar;
            G /= scalar;
            B /= scalar;
            return *this;
        }
        RGBRough& operator%=(const unsigned char &denom) {
            R %= denom;
            G %= denom;
            B %= denom;
            return *this;
        }
        RGBRough operator+(const RGBRough & color) const {return RGBRough(R + color.getR(), G + color.getG(), B + color.getB());}
        RGBRough operator-(const RGBRough & color) const {return RGBRough(R - color.getR(), G - color.getG(), B - color.getB());}
        RGBRough operator*(const unsigned char &scalar) const {return RGBRough(R * scalar, G * scalar, B * scalar);}
        RGBRough operator/(const unsigned char &scalar) const {return RGBRough(R / scalar, G / scalar, B / scalar);}
        RGBRough operator%(const unsigned char &denom) const {return RGBRough(R % denom, G % denom, B % denom);}
};
unsigned char RGBRough::RelationMetric = RGB_RELATE_COMMON;
class RGBExact {
    private:
        double R = 1.0;
        double G = 1.0;
        double B = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        RGBExact(const double &r, const double &g, const double &b, const double &a) : R(btils::clamp<double>(r, 1.0)), G(btils::clamp<double>(g, 1.0)), B(btils::clamp<double>(b, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
        RGBExact(const double &r, const double &g, const double &b) {RGBExact(r, g, b, 1.0);}

        RGBExact(const RGBRough &color) : R(color.getR() / 255.0), G(color.getG() / 255.0), B(color.getB() / 255.0), A(color.getA() / 255.0) {}
        RGBExact(const RGBExact &color) : R(color.getR()), G(color.getG()), B(color.getB()), A(color.getA()) {}
        RGBExact(const HSVRough &color) {RGBExact(hsv2rgb(color));}
        RGBExact(const HSVExact &color) {RGBExact(hsv2rgb(color));}
        RGBExact(const HSLRough &color) {RGBExact(hsl2rgb(color));}
        RGBExact(const HSLExact &color) {RGBExact(hsl2rgb(color));}
        RGBExact(const CMYKRough &color) {RGBExact(cmyk2rgb(color));}
        RGBExact(const CMYKExact &color) {RGBExact(cmyk2rgb(color));}

        #ifdef SDL_pixels_h_
        RGBExact(const SDL_Color &color) : R(color.r / 255.0), G(color.g / 255.0), B(color.b / 255.0), A(color.a / 255.0) {}
        SDL_Color toSDL() const {return {std::round(R * 255.0), std::round(G * 255.0), std::round(B * 255.0), std::round(A * 255.0)};}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        double getR() const {return R;}
        double getG() const {return G;}
        double getB() const {return B;}
        double getA() const {return A;}

        double setR(const double &r) {return btils::set<double>(R, btils::clamp<double>(r, 1.0));}
        double setG(const double &g) {return btils::set<double>(G, btils::clamp<double>(g, 1.0));}
        double setB(const double &b) {return btils::set<double>(B, btils::clamp<double>(b, 1.0));}
        double setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

        double adjR(const double &amount) {return setR(R + amount);}
        double adjG(const double &amount) {return setG(G + amount);}
        double adjB(const double &amount) {return setB(B + amount);}
        double adjA(const double &amount) {return setA(A + amount);}

        void operator=(const RGBExact &color) {
            R = color.getR();
            G = color.getG();
            B = color.getB();
            A = color.getA();
        }
        RGBExact operator!() const {return RGBExact(1.0 - R, 1.0 - G, 1.0 - B, A);}
        RGBRough toRough() const {return RGBRough(std::round(R * 255.0), std::round(G * 255.0), std::round(B * 255.0), std::round(A * 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case RGB_RELATE_COMMON:
                    return R == color.getR() && G == color.getG() && B == color.getB();
                case RGB_RELATE_RED:
                    return R == color.getR();
                case RGB_RELATE_GREEN:
                    return G == color.getG();
                case RGB_RELATE_BLUE:
                    return B == color.getB();
                case RGB_RELATE_ALPHA:
                    return A == color.getA();
            }
        }
        bool notequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const RGBExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case RGB_RELATE_COMMON:
                    return R * R + G * G + B * B < color.getR() * color.getR() + color.getG() * color.getG() + color.getB() * color.getB();
                case RGB_RELATE_RED:
                    return R < color.getR();
                case RGB_RELATE_GREEN:
                    return G < color.getG();
                case RGB_RELATE_BLUE:
                    return B < color.getB();
                case RGB_RELATE_ALPHA:
                    return A < color.getA();
            }
        }
        bool greater(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const RGBExact &color) const {return equal(color, RelationMetric);}
        bool operator!=(const RGBExact &color) const {return notequal(color, RelationMetric);}
        bool operator<(const RGBExact &color) const {return less(color, RelationMetric);}
        bool operator>(const RGBExact &color) const {return greater(color, RelationMetric);}
        bool operator<=(const RGBExact &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const RGBExact &color) const {return greaterequal(color, RelationMetric);}

        RGBExact& operator+=(const RGBExact &color) {
            R = btils::clamp<double>(R + color.getR(), 1.0);
            G = btils::clamp<double>(G + color.getG(), 1.0);
            B = btils::clamp<double>(B + color.getB(), 1.0);
            return *this;
        }
        RGBExact& operator-=(const RGBExact &color) {
            R = btils::clamp<double>(R - color.getR(), 1.0);
            G = btils::clamp<double>(G - color.getG(), 1.0);
            B = btils::clamp<double>(B - color.getB(), 1.0);
            return *this;
        }
        RGBExact& operator*=(const double &scalar) {
            R = btils::clamp<double>(R * scalar, 1.0);
            G = btils::clamp<double>(G * scalar, 1.0);
            B = btils::clamp<double>(B * scalar, 1.0);
            return *this;
        }
        RGBExact& operator/=(const double &scalar) {
            R = btils::clamp<double>(R / scalar, 1.0);
            G = btils::clamp<double>(G / scalar, 1.0);
            B = btils::clamp<double>(B / scalar, 1.0);
            return *this;
        }
        RGBExact& operator%=(const double &denom) {
            R = btils::clamp<double>(std::fmod(R, denom), 1.0);
            G = btils::clamp<double>(std::fmod(G, denom), 1.0);
            B = btils::clamp<double>(std::fmod(B, denom), 1.0);
            return *this;
        }
        RGBExact operator+(const RGBExact &color) const {return RGBExact(R + color.getR(), G + color.getG(), B + color.getB());}
        RGBExact operator-(const RGBExact &color) const {return RGBExact(R - color.getR(), G - color.getG(), B - color.getB());}
        RGBExact operator*(const double &scalar) const {return RGBExact(R * scalar, G * scalar, B * scalar);}
        RGBExact operator/(const double &scalar) const {return RGBExact(R / scalar, G / scalar, B / scalar);}
        RGBExact operator%(const double &denom) const {return RGBExact(std::fmod(R, denom), std::fmod(G, denom), std::fmod(B, denom));}
};
unsigned char RGBExact::RelationMetric = RGB_RELATE_COMMON;

class HSVRough {
    private:
        short H = 0;
        unsigned char S = 0;
        unsigned char V = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        HSVRough(const short &h, const unsigned char &s, const unsigned char &v, const unsigned char &a) : H(btils::clamp<short>(h, 360)), S(s), V(v), A(a) {}
        HSVRough(const short &h, const unsigned char &s, const unsigned char &v) {HSVRough(h, s, v, 255);}

        HSVRough(const RGBRough &color) {HSVRough(rgb2hsv(color));}
        HSVRough(const RGBExact &color) {HSVRough(rgb2hsv(color));}
        HSVRough(const HSVRough &color) : H(color.getH()), S(color.getS()), V(color.getV()), A(color.getA()) {}
        HSVRough(const HSVExact &color) : H(std::round(color.getH() * 360.0)), S(std::round(color.getS() * 255.0)), V(std::round(color.getV() * 255.0)), A(std::round(color.getA() * 255.0)) {}
        HSVRough(const HSLRough &color) {HSVRough(hsl2hsv(color));}
        HSVRough(const HSLExact &color) {HSVRough(hsl2hsv(color));}
        HSVRough(const CMYKRough &color) {HSVRough(cmyk2hsv(color));}
        HSVRough(const CMYKExact &color) {HSVRough(cmyk2hsv(color));}

        #ifdef SDL_pixels_h_
        HSVRough(const SDL_Color &color) {HSVRough(RGBRough(color));}
        SDL_Color toSDL() const {return hsv2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = HSV_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSV_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        short getH() const {return H;}
        unsigned char getS() const {return S;}
        unsigned char getV() const {return V;}
        unsigned char getA() const {return A;}

        short setH(const short &h) {return btils::set<short>(H, btils::clamp<short>(h, 360));}
        unsigned char setS(const unsigned char &s) {return btils::set<unsigned char>(S, s);}
        unsigned char setV(const unsigned char &v) {return btils::set<unsigned char>(V, v);}
        unsigned char setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

        short adjH(const short &amount) {return setH(H + amount);}
        unsigned char adjS(const unsigned char &amount) {return setS(S + amount);}
        unsigned char adjV(const unsigned char &amount) {return setV(V + amount);}
        unsigned char adjA(const unsigned char &amount) {return setA(A + amount);}

        void operator=(const HSVRough &color) {
            H = color.getH();
            S = color.getS();
            V = color.getV();
            A = color.getA();
        }
        HSVRough operator!() const {return HSVRough(btils::normalize<short>(H + 180, 360), S, V, A);}
        HSVExact toExact() const {return HSVExact((double)(H / 360.0), (double)(S / 255.0), (double)(V / 255.0), (double)(A / 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(V) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const HSVRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSV_RELATE_HUE:
                    return H == color.getH();
                case HSV_RELATE_SATURATION:
                    return S == color.getS();
                case HSV_RELATE_VALUE:
                    return V == color.getV();
                case HSV_RELATE_ALPHA:
                    return A == color.getA();
            }

            const short h1 = std::min(H, (short)(360 - H)), h2 = std::min(color.getH(), (short)(360 - color.getH()));
            return h1 * h1 + S * S + V * V == h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
        }
        bool notequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const HSVRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSV_RELATE_HUE:
                    return H < color.getH();
                case HSV_RELATE_SATURATION:
                    return S < color.getS();
                case HSV_RELATE_VALUE:
                    return V < color.getV();
                case HSV_RELATE_ALPHA:
                    return A < color.getA();
            }

            const short h1 = std::min(H, (short)(360 - H)), h2 = std::min(color.getH(), (short)(360 - color.getH()));
            return h1 * h1 + S * S + V * V < h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
        }
        bool greater(const HSVRough &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const HSVRough &color) const {return equal(color, RelationMetric);}
        bool operator!=(const HSVRough &color) const {return notequal(color, RelationMetric);}
        bool operator<(const HSVRough &color) const {return less(color, RelationMetric);}
        bool operator>(const HSVRough &color) const {return greater(color, RelationMetric);}
        bool operator<=(const HSVRough &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const HSVRough &color) const {return greaterequal(color, RelationMetric);}

        HSVRough& operator+=(const HSVRough &color) {
            *this = HSVRough(RGBRough(*this) + RGBRough(color));
            return *this;
        }
        HSVRough& operator-=(const HSVRough &color) {
            *this = HSVRough(RGBRough(*this) - RGBRough(color));
            return *this;
        }
        HSVRough& operator*=(const double &scalar) {
            *this = HSVRough(RGBRough(*this) * scalar);
            return *this;
        }
        HSVRough& operator/=(const double &scalar) {
            *this = HSVRough(RGBRough(*this) / scalar);
            return *this;
        }
        HSVRough& operator%=(const double &denom) {
            *this = HSVRough(RGBRough(*this) % denom);
            return *this;
        }
        HSVRough operator+(const HSVRough &color) const {return HSVRough(RGBRough(*this) + RGBRough(color));}
        HSVRough operator-(const HSVRough &color) const {return HSVRough(RGBRough(*this) - RGBRough(color));}
        HSVRough operator*(const double &scalar) const {return HSVRough(RGBRough(*this) * scalar);}
        HSVRough operator/(const double &scalar) const {return HSVRough(RGBRough(*this) / scalar);}
        HSVRough operator%(const double &denom) const {return HSVRough(RGBRough(*this) % denom);}
};
unsigned char HSVRough::RelationMetric = HSV_RELATE_COMMON;
class HSVExact {
    private:
        double H = 0.0;
        double S = 0.0;
        double V = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        HSVExact(const double &h, const double &s, const double &v, const double &a) : H(btils::clamp<double>(h, 1.0)), S(btils::clamp<double>(s, 1.0)), V(btils::clamp<double>(v, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
        HSVExact(const double &h, const double &s, const double &v) {HSVExact(h, s, v, 1.0);}

        HSVExact(const RGBRough &color) {HSVExact(rgb2hsv(color));}
        HSVExact(const RGBExact &color) {HSVExact(rgb2hsv(color));}
        HSVExact(const HSVRough &color) : H(color.getH() / 360.0), S(color.getS() / 255.0), V(color.getV() / 255.0), A(color.getA() / 255.0) {}
        HSVExact(const HSVExact &color) : H(color.getH()), S(color.getS()), V(color.getV()), A(color.getA()) {}
        HSVExact(const HSLRough &color) {HSVExact(hsl2hsv(color));}
        HSVExact(const HSLExact &color) {HSVExact(hsl2hsv(color));}
        HSVExact(const CMYKRough &color) {HSVExact(cmyk2hsv(color));}
        HSVExact(const CMYKExact &color) {HSVExact(cmyk2hsv(color));}

        #ifdef SDL_pixels_h_
        HSVExact(const SDL_Color &color) {HSVExact(RGBExact(color));}
        SDL_Color toSDL() const {return hsv2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = HSV_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSV_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        double getH() const {return H;}
        double getS() const {return S;}
        double getV() const {return V;}
        double getA() const {return A;}

        double setH(const double &h) {return btils::set<double>(H, btils::clamp<double>(h, 1.0));}
        double setS(const double &s) {return btils::set<double>(S, btils::clamp<double>(s, 1.0));}
        double setV(const double &v) {return btils::set<double>(V, btils::clamp<double>(v, 1.0));}
        double setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

        double adjH(const double &amount) {return setH(H + amount);}
        double adjS(const double &amount) {return setS(S + amount);}
        double adjV(const double &amount) {return setV(V + amount);}
        double adjA(const double &amount) {return setA(A + amount);}

        void operator=(const HSVExact &color) {
            H = color.getH();
            S = color.getS();
            V = color.getV();
            A = color.getA();
        }
        HSVExact operator!() const {return HSVExact(btils::normalize<double>(H + 0.5, 1.0), S, V, A);}
        HSVRough toRough() const {return HSVRough(std::round(H * 360.0), std::round(S * 255.0), std::round(V * 255.0), std::round(A * 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(V) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const HSVExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSV_RELATE_HUE:
                    return H == color.getH();
                case HSV_RELATE_SATURATION:
                    return S == color.getS();
                case HSV_RELATE_VALUE:
                    return V == color.getV();
                case HSV_RELATE_ALPHA:
                    return A == color.getA();
            }

            const double h1 = std::min(H, 1.0 - H), h2 = std::min(color.getH(), 1.0 - color.getH());
            return h1 * h1 + S * S + V * V == h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
        }
        bool notequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const HSVExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSV_RELATE_HUE:
                    return H < color.getH();
                case HSV_RELATE_SATURATION:
                    return S < color.getS();
                case HSV_RELATE_VALUE:
                    return V < color.getV();
                case HSV_RELATE_ALPHA:
                    return A < color.getA();
            }

            const double h1 = std::min(H, 1.0 - H), h2 = std::min(color.getH(), 1.0 - color.getH());
            return h1 * h1 + S * S + V * V < h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
        }
        bool greater(const HSVExact &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const HSVExact &color) const {return equal(color, RelationMetric);}
        bool operator!=(const HSVExact &color) const {return notequal(color, RelationMetric);}
        bool operator<(const HSVExact &color) const {return less(color, RelationMetric);}
        bool operator>(const HSVExact &color) const {return greater(color, RelationMetric);}
        bool operator<=(const HSVExact &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const HSVExact &color) const {return greaterequal(color, RelationMetric);}

        HSVExact& operator+=(const HSVExact &color) {
            *this = HSVExact(RGBExact(*this) + RGBExact(color));
            return *this;
        }
        HSVExact& operator-=(const HSVExact &color) {
            *this = HSVExact(RGBExact(*this) - RGBExact(color));
            return *this;
        }
        HSVExact& operator*=(const double &scalar) {
            *this = HSVExact(RGBExact(*this) * scalar);
            return *this;
        }
        HSVExact& operator/=(const double &scalar) {
            *this = HSVExact(RGBExact(*this) / scalar);
            return *this;
        }
        HSVExact& operator%=(const double &denom) {
            *this = HSVExact(RGBExact(*this) % denom);
            return *this;
        }
        HSVExact operator+(const HSVExact &color) const {return HSVExact(RGBExact(*this) + RGBExact(color));}
        HSVExact operator-(const HSVExact &color) const {return HSVExact(RGBExact(*this) - RGBExact(color));}
        HSVExact operator*(const double &scalar) const {return HSVExact(RGBExact(*this) * scalar);}
        HSVExact operator/(const double &scalar) const {return HSVExact(RGBExact(*this) / scalar);}
        HSVExact operator%(const double &denom) const {return HSVExact(RGBExact(*this) % denom);}
};
unsigned char HSVExact::RelationMetric = HSV_RELATE_COMMON;

class HSLRough {
    private:
        short H = 0;
        unsigned char S = 0;
        unsigned char L = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        HSLRough(const short &h, const unsigned char &s, const unsigned char &l, const unsigned char &a) : H(btils::clamp<short>(h, 360)), S(s), L(l), A(a) {}
        HSLRough(const short &h, const unsigned char &s, const unsigned char &l) {HSLRough(h, s, l, 255);}

        HSLRough(const RGBRough &color) {HSLRough(rgb2hsl(color));}
        HSLRough(const RGBExact &color) {HSLRough(rgb2hsl(color));}
        HSLRough(const HSVRough &color) {HSLRough(hsv2hsl(color));}
        HSLRough(const HSVExact &color) {HSLRough(hsv2hsl(color));}
        HSLRough(const HSLRough &color) : H(color.getH()), S(color.getS()), L(color.getL()), A(color.getA()) {}
        HSLRough(const HSLExact &color) : H(std::round(color.getH() * 360.0)), S(std::round(color.getS() * 255.0)), L(std::round(color.getL() * 255.0)), A(std::round(color.getA() * 255.0)) {}
        HSLRough(const CMYKRough &color) {HSLRough(cmyk2hsl(color));}
        HSLRough(const CMYKExact &color) {HSLRough(cmyk2hsl(color));}

        #ifdef SDL_pixels_h_
        HSLRough(const SDL_Color &color) {HSLRough(RGBRough(color));}
        SDL_Color toSDL() const {return hsl2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = HSL_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSL_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        short getH() const {return H;}
        unsigned char getS() const {return S;}
        unsigned char getL() const {return L;}
        unsigned char getA() const {return A;}

        short setH(const short &h) {return btils::set<short>(H, btils::clamp<short>(h, 360));}
        unsigned char setS(const unsigned char &s) {return btils::set<unsigned char>(S, s);}
        unsigned char setL(const unsigned char &l) {return btils::set<unsigned char>(L, l);}
        unsigned char setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

        short adjH(const short &amount) {return setH(H + amount);}
        unsigned char adjS(const unsigned char &amount) {return setS(S + amount);}
        unsigned char adjL(const unsigned char &amount) {return setL(L + amount);}
        unsigned char adjA(const unsigned char &amount) {return setA(A + amount);}

        void operator=(const HSLRough &color) {
            H = color.getH();
            S = color.getS();
            L = color.getL();
            A = color.getA();
        }
        HSLRough operator!() const {return HSLRough(btils::normalize<short>(H + 180, 360), S, L, A);}
        HSLExact toExact() const {return HSLExact((double)(H / 360.0), (double)(S / 255.0), (double)(L / 255.0), (double)(A / 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(L) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const HSLRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSL_RELATE_HUE:
                    return H == color.getH();
                case HSL_RELATE_SATURATION:
                    return S == color.getS();
                case HSL_RELATE_LIGHTNESS:
                    return L == color.getL();
                case HSL_RELATE_ALPHA:
                    return A == color.getA();
            }

            const short h1 = std::min(H, (short)(360 - H)), h2 = std::min(color.getH(), (short)(360 - color.getH()));
            return h1 * h1 + S * S + L * L == h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
        }
        bool notequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const HSLRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSL_RELATE_HUE:
                    return H < color.getH();
                case HSL_RELATE_SATURATION:
                    return S < color.getS();
                case HSL_RELATE_LIGHTNESS:
                    return L < color.getL();
                case HSL_RELATE_ALPHA:
                    return A < color.getA();
            }

            const short h1 = std::min(H, (short)(360 - H)), h2 = std::min(color.getH(), (short)(360 - color.getH()));
            return h1 * h1 + S * S + L * L < h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
        }
        bool greater(const HSLRough &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const HSLRough &color) const {return equal(color, RelationMetric);}
        bool operator!=(const HSLRough &color) const {return notequal(color, RelationMetric);}
        bool operator<(const HSLRough &color) const {return less(color, RelationMetric);}
        bool operator>(const HSLRough &color) const {return greater(color, RelationMetric);}
        bool operator<=(const HSLRough &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const HSLRough &color) const {return greaterequal(color, RelationMetric);}

        HSLRough& operator+=(const HSLRough &color) {
            *this = HSLRough(RGBRough(*this) + RGBRough(color));
            return *this;
        }
        HSLRough& operator-=(const HSLRough &color) {
            *this = HSLRough(RGBRough(*this) - RGBRough(color));
            return *this;
        }
        HSLRough& operator*=(const double &scalar) {
            *this = HSLRough(RGBRough(*this) * scalar);
            return *this;
        }
        HSLRough& operator/=(const double &scalar) {
            *this = HSLRough(RGBRough(*this) / scalar);
            return *this;
        }
        HSLRough& operator%=(const double &denom) {
            *this = HSLRough(RGBRough(*this) % denom);
            return *this;
        }
        HSLRough operator+(const HSLRough &color) const {return HSLRough(RGBRough(*this) + RGBRough(color));}
        HSLRough operator-(const HSLRough &color) const {return HSLRough(RGBRough(*this) - RGBRough(color));}
        HSLRough operator*(const double &scalar) const {return HSLRough(RGBRough(*this) * scalar);}
        HSLRough operator/(const double &scalar) const {return HSLRough(RGBRough(*this) / scalar);}
        HSLRough operator%(const double &denom) const {return HSLRough(RGBRough(*this) % denom);}
};
unsigned char HSLRough::RelationMetric = HSL_RELATE_COMMON;
class HSLExact {
    private:
        double H = 0.0;
        double S = 0.0;
        double L = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        HSLExact(const double &h, const double &s, const double &l, const double &a) : H(btils::clamp<double>(h, 1.0)), S(btils::clamp<double>(s, 1.0)), L(btils::clamp<double>(l, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
        HSLExact(const double &h, const double &s, const double &l) {HSLExact(h, s, l, 1.0);}

        HSLExact(const RGBRough &color) {HSLExact(rgb2hsl(color));}
        HSLExact(const RGBExact &color) {HSLExact(rgb2hsl(color));}
        HSLExact(const HSVRough &color) {HSLExact(hsv2hsl(color));}
        HSLExact(const HSVExact &color) {HSLExact(hsv2hsl(color));}
        HSLExact(const HSLRough &color) : H(color.getH() / 360.0), S(color.getS() / 255.0), L(color.getL() / 255.0), A(color.getA() / 255.0) {}
        HSLExact(const HSLExact &color) : H(color.getH()), S(color.getS()), L(color.getL()), A(color.getA()) {}
        HSLExact(const CMYKRough &color) {HSLExact(cmyk2hsl(color));}
        HSLExact(const CMYKExact &color) {HSLExact(cmyk2hsl(color));}

        #ifdef SDL_pixels_h_
        HSLExact(const SDL_Color &color) {HSLExact(RGBExact(color));}
        SDL_Color toSDL() const {return hsl2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = HSL_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSL_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        double getH() const {return H;}
        double getS() const {return S;}
        double getL() const {return L;}
        double getA() const {return A;}

        double setH(const double &h) {return btils::set<double>(H, btils::clamp<double>(h, 1.0));}
        double setS(const double &s) {return btils::set<double>(S, btils::clamp<double>(s, 1.0));}
        double setL(const double &l) {return btils::set<double>(L, btils::clamp<double>(l, 1.0));}
        double setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

        double adjH(const double &amount) {return setH(H + amount);}
        double adjS(const double &amount) {return setS(S + amount);}
        double adjL(const double &amount) {return setL(L + amount);}
        double adjA(const double &amount) {return setA(A + amount);}

        void operator=(const HSLExact &color) {
            H = color.getH();
            S = color.getS();
            L = color.getL();
            A = color.getA();
        }
        HSLExact operator!() const {return HSLExact(btils::normalize<double>(H + 0.5, 1.0), S, L, A);}
        HSLRough toRough() const {return HSLRough(std::round(H * 360.0), std::round(S * 255.0), std::round(L * 255.0), std::round(A * 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(L) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const HSLExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSL_RELATE_HUE:
                    return H == color.getH();
                case HSL_RELATE_SATURATION:
                    return S == color.getS();
                case HSL_RELATE_LIGHTNESS:
                    return L == color.getL();
                case HSL_RELATE_ALPHA:
                    return A == color.getA();
            }

            const double h1 = std::min(H, 1.0 - H), h2 = std::min(color.getH(), 1.0 - color.getH());
            return h1 * h1 + S * S + L * L == h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
        }
        bool notequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const HSLExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                case HSL_RELATE_HUE:
                    return H < color.getH();
                case HSL_RELATE_SATURATION:
                    return S < color.getS();
                case HSL_RELATE_LIGHTNESS:
                    return L < color.getL();
                case HSL_RELATE_ALPHA:
                    return A < color.getA();
            }

            const double h1 = std::min(H, 1.0 - H), h2 = std::min(color.getH(), 1.0 - color.getH());
            return h1 * h1 + S * S + L * L < h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
        }
        bool greater(const HSLExact &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const HSLExact &color) const {return equal(color, RelationMetric);}
        bool operator!=(const HSLExact &color) const {return notequal(color, RelationMetric);}
        bool operator<(const HSLExact &color) const {return less(color, RelationMetric);}
        bool operator>(const HSLExact &color) const {return greater(color, RelationMetric);}
        bool operator<=(const HSLExact &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const HSLExact &color) const {return greaterequal(color, RelationMetric);}

        HSLExact& operator+=(const HSLExact &color) {
            *this = HSLExact(RGBExact(*this) + RGBExact(color));
            return *this;
        }
        HSLExact& operator-=(const HSLExact &color) {
            *this = HSLExact(RGBExact(*this) - RGBExact(color));
            return *this;
        }
        HSLExact& operator*=(const double &scalar) {
            *this = HSLExact(RGBExact(*this) * scalar);
            return *this;
        }
        HSLExact& operator/=(const double &scalar) {
            *this = HSLExact(RGBExact(*this) / scalar);
            return *this;
        }
        HSLExact& operator%=(const double &denom) {
            *this = HSLExact(RGBExact(*this) % denom);
            return *this;
        }
        HSLExact operator+(const HSLExact &color) const {return HSLExact(RGBExact(*this) + RGBExact(color));}
        HSLExact operator-(const HSLExact &color) const {return HSLExact(RGBExact(*this) - RGBExact(color));}
        HSLExact operator*(const double &scalar) const {return HSLExact(RGBExact(*this) * scalar);}
        HSLExact operator/(const double &scalar) const {return HSLExact(RGBExact(*this) / scalar);}
        HSLExact operator%(const double &denom) const {return HSLExact(RGBExact(*this) % denom);}
};
unsigned char HSLExact::RelationMetric = HSL_RELATE_COMMON;

class CMYKRough {
    private:
        unsigned char C = 0;
        unsigned char M = 0;
        unsigned char Y = 0;
        unsigned char K = 0;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k, const unsigned char &a) : C(c), M(m), Y(y), K(k), A(a) {}
        CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k) {CMYKRough(c, m, y, k, 255);}
        
        CMYKRough(const RGBRough &color) {CMYKRough(rgb2cmyk(color));}
        CMYKRough(const RGBExact &color) {CMYKRough(rgb2cmyk(color));}
        CMYKRough(const HSVRough &color) {CMYKRough(hsv2cmyk(color));}
        CMYKRough(const HSVExact &color) {CMYKRough(hsv2cmyk(color));}
        CMYKRough(const HSLRough &color) {CMYKRough(hsl2cmyk(color));}
        CMYKRough(const HSLExact &color) {CMYKRough(hsl2cmyk(color));}
        CMYKRough(const CMYKRough &color) : C(color.getC()), M(color.getM()), Y(color.getY()), K(color.getK()), A(color.getA()) {}
        CMYKRough(const CMYKExact &color) : C(std::round(color.getC() * 255.0)), M(std::round(color.getM() * 255.0)), Y(std::round(color.getY() * 255.0)), K(std::round(color.getK() * 255.0)), A(std::round(color.getA() * 255.0)) {}

        #ifdef SDL_pixels_h_
        CMYKRough(const SDL_Color &color) {CMYKRough(RGBRough(color));}
        SDL_Color toSDL() const {return cmyk2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = CMYK_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 5 || metric < 0 ? CMYK_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        unsigned char getC() const {return C;}
        unsigned char getM() const {return M;}
        unsigned char getY() const {return Y;}
        unsigned char getK() const {return K;}
        unsigned char getA() const {return A;}

        unsigned char setC(const unsigned char &c) {return btils::set<unsigned char>(C, c);}
        unsigned char setM(const unsigned char &m) {return btils::set<unsigned char>(M, m);}
        unsigned char setY(const unsigned char &y) {return btils::set<unsigned char>(Y, y);}
        unsigned char setK(const unsigned char &k) {return btils::set<unsigned char>(K, k);}
        unsigned char setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

        unsigned char adjC(const unsigned char &amount) {return setC(C + amount);}
        unsigned char adjM(const unsigned char &amount) {return setM(M + amount);}
        unsigned char adjY(const unsigned char &amount) {return setY(Y + amount);}
        unsigned char adjK(const unsigned char &amount) {return setK(K + amount);}
        unsigned char adjA(const unsigned char &amount) {return setA(A + amount);}

        void operator=(const CMYKRough &color) {
            C = color.getC();
            M = color.getM();
            Y = color.getY();
            K = color.getK();
            A = color.getA();
        }
        CMYKRough operator!() const {return CMYKRough(255 - C, 255 - M, 255 - Y, 255 - K, A);}
        CMYKExact toExact() const {return CMYKExact((double)(C / 255.0), (double)(M / 255.0), (double)(Y / 255.0), (double)(K / 255.0), (double)(A / 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(C) + ", " + std::to_string(M) + ", " + std::to_string(Y) + ", " + std::to_string(K) + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case CMYK_RELATE_COMMON:
                    return C == color.getC() && M == color.getM() && Y == color.getY() && K == color.getK();
                case CMYK_RELATE_CYAN:
                    return C == color.getC();
                case CMYK_RELATE_MAGENTA:
                    return M == color.getM();
                case CMYK_RELATE_YELLOW:
                    return Y == color.getY();
                case CMYK_RELATE_KEY:
                    return K == color.getK();
                case CMYK_RELATE_ALPHA:
                    return A == color.getA();
            }
        }
        bool notequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case CMYK_RELATE_COMMON:
                    return C * C + M * M + Y * Y + K * K < color.getC() * color.getC() + color.getM() * color.getM() + color.getY() * color.getY() + color.getK() * color.getK();
                case CMYK_RELATE_CYAN:
                    return C < color.getC();
                case CMYK_RELATE_MAGENTA:
                    return M < color.getM();
                case CMYK_RELATE_YELLOW:
                    return Y < color.getY();
                case CMYK_RELATE_KEY:
                    return K < color.getK();
                case CMYK_RELATE_ALPHA:
                    return A < color.getA();
            }
        }
        bool greater(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const CMYKRough &color) const {return equal(color, RelationMetric);}
        bool operator!=(const CMYKRough &color) const {return notequal(color, RelationMetric);}
        bool operator<(const CMYKRough &color) const {return less(color, RelationMetric);}
        bool operator>(const CMYKRough &color) const {return greater(color, RelationMetric);}
        bool operator<=(const CMYKRough &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const CMYKRough &color) const {return greaterequal(color, RelationMetric);}

        CMYKRough& operator+=(const CMYKRough &color) {
            C += color.getC();
            M += color.getM();
            Y += color.getY();
            K += color.getK();
            return *this;
        }
        CMYKRough& operator-=(const CMYKRough &color) {
            C -= color.getC();
            M -= color.getM();
            Y -= color.getY();
            K -= color.getK();
            return *this;
        }
        CMYKRough& operator*=(const unsigned char &scalar) {
            C *= scalar;
            M *= scalar;
            Y *= scalar;
            K *= scalar;
            return *this;
        }
        CMYKRough& operator/=(const unsigned char &scalar) {
            C /= scalar;
            M /= scalar;
            Y /= scalar;
            K /= scalar;
            return *this;
        }
        CMYKRough& operator%=(const unsigned char &denom) {
            C %= denom;
            M %= denom;
            Y %= denom;
            K %= denom;
            return *this;
        }
        CMYKRough operator+(const CMYKRough & color) const {return CMYKRough(C + color.getC(), M + color.getM(), Y + color.getY(), K + color.getK());}
        CMYKRough operator-(const CMYKRough & color) const {return CMYKRough(C - color.getC(), M - color.getM(), Y - color.getY(), K - color.getK());}
        CMYKRough operator*(const unsigned char &scalar) const {return CMYKRough(C * scalar, M * scalar, Y * scalar, K * scalar);}
        CMYKRough operator/(const unsigned char &scalar) const {return CMYKRough(C / scalar, M / scalar, Y / scalar, K / scalar);}
        CMYKRough operator%(const unsigned char &denom) const {return CMYKRough(C % denom, M % denom, Y % denom, K % denom);}
};
unsigned char CMYKRough::RelationMetric = CMYK_RELATE_COMMON;
class CMYKExact {
    private:
        double C = 0.0;
        double M = 0.0;
        double Y = 0.0;
        double K = 0.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k, const double &a) : C(btils::clamp<double>(c, 1.0)), M(btils::clamp<double>(m, 1.0)), Y(btils::clamp<double>(y, 1.0)), K(btils::clamp<double>(k, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
        CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k) {CMYKExact(c, m, y, k, 1.0);}

        CMYKExact(const RGBRough &color) {CMYKExact(rgb2cmyk(color));}
        CMYKExact(const RGBExact &color) {CMYKExact(rgb2cmyk(color));}
        CMYKExact(const HSVRough &color) {CMYKExact(hsv2cmyk(color));}
        CMYKExact(const HSVExact &color) {CMYKExact(hsv2cmyk(color));}
        CMYKExact(const HSLRough &color) {CMYKExact(hsl2cmyk(color));}
        CMYKExact(const HSLExact &color) {CMYKExact(hsl2cmyk(color));}
        CMYKExact(const CMYKRough &color) : C(color.getC() / 255.0), M(color.getM() / 255.0), Y(color.getY() / 255.0), K(color.getK() / 255.0), A(color.getA() / 255.0) {}
        CMYKExact(const CMYKExact &color) : C(color.getC()), M(color.getM()), Y(color.getY()), K(color.getK()), A(color.getA()) {}

        #ifdef SDL_pixels_h_
        CMYKExact(const SDL_Color &color) {CMYKExact(RGBExact(color));}
        SDL_Color toSDL() const {return cmyk2rgb(*this).toSDL();}
        #endif

        static unsigned char getRelationMetric() {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = CMYK_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 5 || metric < 0 ? CMYK_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

        double getC() const {return C;}
        double getM() const {return M;}
        double getY() const {return Y;}
        double getK() const {return K;}
        double getA() const {return A;}

        double setC(const double &c) {return btils::set<double>(C, btils::clamp<double>(c, 1.0));}
        double setM(const double &m) {return btils::set<double>(M, btils::clamp<double>(m, 1.0));}
        double setY(const double &y) {return btils::set<double>(Y, btils::clamp<double>(y, 1.0));}
        double setK(const double &k) {return btils::set<double>(K, btils::clamp<double>(k, 1.0));}
        double setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

        double adjC(const double &amount) {return setC(C + amount);}
        double adjM(const double &amount) {return setM(M + amount);}
        double adjY(const double &amount) {return setY(Y + amount);}
        double adjK(const double &amount) {return setK(K + amount);}
        double adjA(const double &amount) {return setA(A + amount);}

        void operator=(const CMYKExact &color) {
            C = color.getC();
            M = color.getM();
            Y = color.getY();
            K = color.getK();
            A = color.getA();
        }
        CMYKExact operator!() const {return CMYKExact(1.0 - C, 1.0 - M, 1.0 - Y, 1.0 - K, A);}
        CMYKRough toRough() const {return CMYKRough(std::round(C * 255.0), std::round(M * 255.0), std::round(Y * 255.0), std::round(K * 255.0), std::round(A * 255.0));}

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const {return std::to_string(C) + ", " + std::to_string(M) + ", " + std::to_string(Y) + ", " + std::to_string(K) + (includeAlpha ? ", " + std::to_string(A) : "");}
        #endif

        bool equal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case CMYK_RELATE_COMMON:
                    return C * C + M * M + Y * Y + K * K == color.getC() * color.getC() + color.getM() * color.getM() + color.getY() * color.getY() + color.getK() * color.getK();
                case CMYK_RELATE_CYAN:
                    return C == color.getC();
                case CMYK_RELATE_MAGENTA:
                    return M == color.getM();
                case CMYK_RELATE_YELLOW:
                    return Y == color.getY();
                case CMYK_RELATE_KEY:
                    return K == color.getK();
                case CMYK_RELATE_ALPHA:
                    return A == color.getA();
            }
        }
        bool notequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool less(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {
            switch (metric) {
                default:
                case CMYK_RELATE_COMMON:
                    return C * C + M * M + Y * Y + K * K < color.getC() * color.getC() + color.getM() * color.getM() + color.getY() * color.getY() + color.getK() * color.getK();
                case CMYK_RELATE_CYAN:
                    return C < color.getC();
                case CMYK_RELATE_MAGENTA:
                    return M < color.getM();
                case CMYK_RELATE_YELLOW:
                    return Y < color.getY();
                case CMYK_RELATE_KEY:
                    return K < color.getK();
                case CMYK_RELATE_ALPHA:
                    return A < color.getA();
            }
        }
        bool greater(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool lessequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool operator==(const CMYKExact &color) const {return equal(color, RelationMetric);}
        bool operator!=(const CMYKExact &color) const {return notequal(color, RelationMetric);}
        bool operator<(const CMYKExact &color) const {return less(color, RelationMetric);}
        bool operator>(const CMYKExact &color) const {return greater(color, RelationMetric);}
        bool operator<=(const CMYKExact &color) const {return lessequal(color, RelationMetric);}
        bool operator>=(const CMYKExact &color) const {return greaterequal(color, RelationMetric);}

        CMYKExact& operator+=(const CMYKExact &color) {
            C = btils::clamp<double>(C + color.getC(), 1.0);
            M = btils::clamp<double>(M + color.getM(), 1.0);
            Y = btils::clamp<double>(Y + color.getY(), 1.0);
            K = btils::clamp<double>(K + color.getK(), 1.0);
            return *this;
        }
        CMYKExact& operator-=(const CMYKExact &color) {
            C = btils::clamp<double>(C - color.getC(), 1.0);
            M = btils::clamp<double>(M - color.getM(), 1.0);
            Y = btils::clamp<double>(Y - color.getY(), 1.0);
            K = btils::clamp<double>(K - color.getK(), 1.0);
            return *this;
        }
        CMYKExact& operator*=(const double &scalar) {
            C = btils::clamp<double>(C * scalar, 1.0);
            M = btils::clamp<double>(M * scalar, 1.0);
            Y = btils::clamp<double>(Y * scalar, 1.0);
            K = btils::clamp<double>(K * scalar, 1.0);
            return *this;
        }
        CMYKExact& operator/=(const double &scalar) {
            C = btils::clamp<double>(C / scalar, 1.0);
            M = btils::clamp<double>(M / scalar, 1.0);
            Y = btils::clamp<double>(Y / scalar, 1.0);
            K = btils::clamp<double>(K / scalar, 1.0);
            return *this;
        }
        CMYKExact& operator%=(const double &denom) {
            C = btils::clamp<double>(std::fmod(C, denom), 1.0);
            M = btils::clamp<double>(std::fmod(M, denom), 1.0);
            Y = btils::clamp<double>(std::fmod(Y, denom), 1.0);
            K = btils::clamp<double>(std::fmod(K, denom), 1.0);
            return *this;
        }
        CMYKExact operator+(const CMYKExact &color) const {return CMYKExact(C + color.getC(), M + color.getM(), Y + color.getY(), K + color.getK());}
        CMYKExact operator-(const CMYKExact &color) const {return CMYKExact(C - color.getC(), M - color.getM(), Y - color.getY(), K - color.getK());}
        CMYKExact operator*(const double &scalar) const {return CMYKExact(C * scalar, M * scalar, Y * scalar, K * scalar);}
        CMYKExact operator/(const double &scalar) const {return CMYKExact(C / scalar, M / scalar, Y / scalar, K / scalar);}
        CMYKExact operator%(const double &denom) const {return CMYKExact(std::fmod(C, denom), std::fmod(M, denom), std::fmod(Y, denom), std::fmod(K, denom));}
};
unsigned char CMYKExact::RelationMetric = CMYK_RELATE_COMMON;

#endif /* BCOLORS */
