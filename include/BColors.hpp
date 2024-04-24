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
class HSVRough;
class HSLRough;
class CMYKRough;
class RGBExact;
class HSVExact;
class HSLExact;
class CMYKExact;

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

class RGBRough {
    private:
        unsigned char R = 255;
        unsigned char G = 255;
        unsigned char B = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a);
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b);

        RGBRough(const RGBRough &color);
        RGBRough(const RGBExact &color);
        RGBRough(const HSVRough &color);
        RGBRough(const HSVExact &color);
        RGBRough(const HSLRough &color);
        RGBRough(const HSLExact &color);
        RGBRough(const CMYKRough &color);
        RGBRough(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        RGBRough(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        unsigned char getR() const;
        unsigned char getG() const;
        unsigned char getB() const;
        unsigned char getA() const;

        unsigned char setR(const unsigned char &r);
        unsigned char setG(const unsigned char &g);
        unsigned char setB(const unsigned char &b);
        unsigned char setA(const unsigned char &a);

        unsigned char adjR(const unsigned char &amount);
        unsigned char adjG(const unsigned char &amount);
        unsigned char adjB(const unsigned char &amount);
        unsigned char adjA(const unsigned char &amount);

        void operator=(const RGBRough &color);
        RGBRough operator!() const;
        RGBExact toExact() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool less(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const RGBRough &color) const;
        bool operator!=(const RGBRough &color) const;
        bool operator<(const RGBRough &color) const;
        bool operator>(const RGBRough &color) const;
        bool operator<=(const RGBRough &color) const;
        bool operator>=(const RGBRough &color) const;

        RGBRough& operator+=(const RGBRough &color);
        RGBRough& operator-=(const RGBRough &color);
        RGBRough& operator*=(const unsigned char &scalar);
        RGBRough& operator/=(const unsigned char &scalar);
        RGBRough& operator%=(const unsigned char &denom);
        RGBRough operator+(const RGBRough & color) const;
        RGBRough operator-(const RGBRough & color) const;
        RGBRough operator*(const unsigned char &scalar) const;
        RGBRough operator/(const unsigned char &scalar) const;
        RGBRough operator%(const unsigned char &denom) const;
};
class RGBExact {
    private:
        double R = 1.0;
        double G = 1.0;
        double B = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        RGBExact(const double &r, const double &g, const double &b, const double &a);
        RGBExact(const double &r, const double &g, const double &b);

        RGBExact(const RGBRough &color);
        RGBExact(const RGBExact &color);
        RGBExact(const HSVRough &color);
        RGBExact(const HSVExact &color);
        RGBExact(const HSLRough &color);
        RGBExact(const HSLExact &color);
        RGBExact(const CMYKRough &color);
        RGBExact(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        RGBExact(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        double getR() const;
        double getG() const;
        double getB() const;
        double getA() const;

        double setR(const double &r);
        double setG(const double &g);
        double setB(const double &b);
        double setA(const double &a);

        double adjR(const double &amount);
        double adjG(const double &amount);
        double adjB(const double &amount);
        double adjA(const double &amount);

        void operator=(const RGBExact &color);
        RGBExact operator!() const;
        RGBRough toRough() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool less(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const RGBExact &color) const;
        bool operator!=(const RGBExact &color) const;
        bool operator<(const RGBExact &color) const;
        bool operator>(const RGBExact &color) const;
        bool operator<=(const RGBExact &color) const;
        bool operator>=(const RGBExact &color) const;

        RGBExact& operator+=(const RGBExact &color);
        RGBExact& operator-=(const RGBExact &color);
        RGBExact& operator*=(const double &scalar);
        RGBExact& operator/=(const double &scalar);
        RGBExact& operator%=(const double &denom);
        RGBExact operator+(const RGBExact &color) const;
        RGBExact operator-(const RGBExact &color) const;
        RGBExact operator*(const double &scalar) const;
        RGBExact operator/(const double &scalar) const;
        RGBExact operator%(const double &denom) const;
};

class HSVRough {
    private:
        short H = 0;
        unsigned char S = 0;
        unsigned char V = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        HSVRough(const short &h, const unsigned char &s, const unsigned char &v, const unsigned char &a);
        HSVRough(const short &h, const unsigned char &s, const unsigned char &v);

        HSVRough(const RGBRough &color);
        HSVRough(const RGBExact &color);
        HSVRough(const HSVRough &color);
        HSVRough(const HSVExact &color);
        HSVRough(const HSLRough &color);
        HSVRough(const HSLExact &color);
        HSVRough(const CMYKRough &color);
        HSVRough(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        HSVRough(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = HSV_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        short getH() const;
        unsigned char getS() const;
        unsigned char getV() const;
        unsigned char getA() const;

        short setH(const short &h);
        unsigned char setS(const unsigned char &s);
        unsigned char setV(const unsigned char &v);
        unsigned char setA(const unsigned char &a);

        short adjH(const short &amount);
        unsigned char adjS(const unsigned char &amount);
        unsigned char adjV(const unsigned char &amount);
        unsigned char adjA(const unsigned char &amount);

        void operator=(const HSVRough &color);
        HSVRough operator!() const;
        HSVExact toExact() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool less(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const HSVRough &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const HSVRough &color) const;
        bool operator!=(const HSVRough &color) const;
        bool operator<(const HSVRough &color) const;
        bool operator>(const HSVRough &color) const;
        bool operator<=(const HSVRough &color) const;
        bool operator>=(const HSVRough &color) const;

        HSVRough& operator+=(const HSVRough &color);
        HSVRough& operator-=(const HSVRough &color);
        HSVRough& operator*=(const double &scalar);
        HSVRough& operator/=(const double &scalar);
        HSVRough& operator%=(const double &denom);
        HSVRough operator+(const HSVRough &color) const;
        HSVRough operator-(const HSVRough &color) const;
        HSVRough operator*(const double &scalar) const;
        HSVRough operator/(const double &scalar) const;
        HSVRough operator%(const double &denom) const;
};
class HSVExact {
    private:
        double H = 0.0;
        double S = 0.0;
        double V = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        HSVExact(const double &h, const double &s, const double &v, const double &a);
        HSVExact(const double &h, const double &s, const double &v);

        HSVExact(const RGBRough &color);
        HSVExact(const RGBExact &color);
        HSVExact(const HSVRough &color);
        HSVExact(const HSVExact &color);
        HSVExact(const HSLRough &color);
        HSVExact(const HSLExact &color);
        HSVExact(const CMYKRough &color);
        HSVExact(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        HSVExact(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = HSV_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        double getH() const;
        double getS() const;
        double getV() const;
        double getA() const;

        double setH(const double &h);
        double setS(const double &s);
        double setV(const double &v);
        double setA(const double &a);

        double adjH(const double &amount);
        double adjS(const double &amount);
        double adjV(const double &amount);
        double adjA(const double &amount);

        void operator=(const HSVExact &color);
        HSVExact operator!() const;
        HSVRough toRough() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool less(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const HSVExact &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const HSVExact &color) const;
        bool operator!=(const HSVExact &color) const;
        bool operator<(const HSVExact &color) const;
        bool operator>(const HSVExact &color) const;
        bool operator<=(const HSVExact &color) const;
        bool operator>=(const HSVExact &color) const;

        HSVExact& operator+=(const HSVExact &color);
        HSVExact& operator-=(const HSVExact &color);
        HSVExact& operator*=(const double &scalar);
        HSVExact& operator/=(const double &scalar);
        HSVExact& operator%=(const double &denom);
        HSVExact operator+(const HSVExact &color) const;
        HSVExact operator-(const HSVExact &color) const;
        HSVExact operator*(const double &scalar) const;
        HSVExact operator/(const double &scalar) const;
        HSVExact operator%(const double &denom) const;
};

class HSLRough {
    private:
        short H = 0;
        unsigned char S = 0;
        unsigned char L = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        HSLRough(const short &h, const unsigned char &s, const unsigned char &l, const unsigned char &a);
        HSLRough(const short &h, const unsigned char &s, const unsigned char &l);

        HSLRough(const RGBRough &color);
        HSLRough(const RGBExact &color);
        HSLRough(const HSVRough &color);
        HSLRough(const HSVExact &color);
        HSLRough(const HSLRough &color);
        HSLRough(const HSLExact &color);
        HSLRough(const CMYKRough &color);
        HSLRough(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        HSLRough(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = HSL_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        short getH() const;
        unsigned char getS() const;
        unsigned char getL() const;
        unsigned char getA() const;

        short setH(const short &h);
        unsigned char setS(const unsigned char &s);
        unsigned char setL(const unsigned char &l);
        unsigned char setA(const unsigned char &a);

        short adjH(const short &amount);
        unsigned char adjS(const unsigned char &amount);
        unsigned char adjL(const unsigned char &amount);
        unsigned char adjA(const unsigned char &amount);

        void operator=(const HSLRough &color);
        HSLRough operator!() const;
        HSLExact toExact() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool less(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const HSLRough &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const HSLRough &color) const;
        bool operator!=(const HSLRough &color) const;
        bool operator<(const HSLRough &color) const;
        bool operator>(const HSLRough &color) const;
        bool operator<=(const HSLRough &color) const;
        bool operator>=(const HSLRough &color) const;

        HSLRough& operator+=(const HSLRough &color);
        HSLRough& operator-=(const HSLRough &color);
        HSLRough& operator*=(const double &scalar);
        HSLRough& operator/=(const double &scalar);
        HSLRough& operator%=(const double &denom);
        HSLRough operator+(const HSLRough &color) const;
        HSLRough operator-(const HSLRough &color) const;
        HSLRough operator*(const double &scalar) const;
        HSLRough operator/(const double &scalar) const;
        HSLRough operator%(const double &denom) const;
};
class HSLExact {
    private:
        double H = 0.0;
        double S = 0.0;
        double L = 1.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        HSLExact(const double &h, const double &s, const double &l, const double &a);
        HSLExact(const double &h, const double &s, const double &l);

        HSLExact(const RGBRough &color);
        HSLExact(const RGBExact &color);
        HSLExact(const HSVRough &color);
        HSLExact(const HSVExact &color);
        HSLExact(const HSLRough &color);
        HSLExact(const HSLExact &color);
        HSLExact(const CMYKRough &color);
        HSLExact(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        HSLExact(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = HSL_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        double getH() const;
        double getS() const;
        double getL() const;
        double getA() const;

        double setH(const double &h);
        double setS(const double &s);
        double setL(const double &l);
        double setA(const double &a);

        double adjH(const double &amount);
        double adjS(const double &amount);
        double adjL(const double &amount);
        double adjA(const double &amount);

        void operator=(const HSLExact &color);
        HSLExact operator!() const;
        HSLRough toRough() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool less(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const HSLExact &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const HSLExact &color) const;
        bool operator!=(const HSLExact &color) const;
        bool operator<(const HSLExact &color) const;
        bool operator>(const HSLExact &color) const;
        bool operator<=(const HSLExact &color) const;
        bool operator>=(const HSLExact &color) const;

        HSLExact& operator+=(const HSLExact &color);
        HSLExact& operator-=(const HSLExact &color);
        HSLExact& operator*=(const double &scalar);
        HSLExact& operator/=(const double &scalar);
        HSLExact& operator%=(const double &denom);
        HSLExact operator+(const HSLExact &color) const;
        HSLExact operator-(const HSLExact &color) const;
        HSLExact operator*(const double &scalar) const;
        HSLExact operator/(const double &scalar) const;
        HSLExact operator%(const double &denom) const;
};

class CMYKRough {
    private:
        unsigned char C = 0;
        unsigned char M = 0;
        unsigned char Y = 0;
        unsigned char K = 0;
        unsigned char A = 255;

        static unsigned char RelationMetric;
    
    public:
        CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k, const unsigned char &a);
        CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k);
        
        CMYKRough(const RGBRough &color);
        CMYKRough(const RGBExact &color);
        CMYKRough(const HSVRough &color);
        CMYKRough(const HSVExact &color);
        CMYKRough(const HSLRough &color);
        CMYKRough(const HSLExact &color);
        CMYKRough(const CMYKRough &color);
        CMYKRough(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        CMYKRough(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = CMYK_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);

        unsigned char getC() const;
        unsigned char getM() const;
        unsigned char getY() const;
        unsigned char getK() const;
        unsigned char getA() const;

        unsigned char setC(const unsigned char &c);
        unsigned char setM(const unsigned char &m);
        unsigned char setY(const unsigned char &y);
        unsigned char setK(const unsigned char &k);
        unsigned char setA(const unsigned char &a);

        unsigned char adjC(const unsigned char &amount);
        unsigned char adjM(const unsigned char &amount);
        unsigned char adjY(const unsigned char &amount);
        unsigned char adjK(const unsigned char &amount);
        unsigned char adjA(const unsigned char &amount);

        void operator=(const CMYKRough &color);
        CMYKRough operator!() const;
        CMYKExact toExact() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool less(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const CMYKRough &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const CMYKRough &color) const;
        bool operator!=(const CMYKRough &color) const;
        bool operator<(const CMYKRough &color) const;
        bool operator>(const CMYKRough &color) const;
        bool operator<=(const CMYKRough &color) const;
        bool operator>=(const CMYKRough &color) const;

        CMYKRough& operator+=(const CMYKRough &color);
        CMYKRough& operator-=(const CMYKRough &color);
        CMYKRough& operator*=(const unsigned char &scalar);
        CMYKRough& operator/=(const unsigned char &scalar);
        CMYKRough& operator%=(const unsigned char &denom);
        CMYKRough operator+(const CMYKRough & color) const;
        CMYKRough operator-(const CMYKRough & color) const;
        CMYKRough operator*(const unsigned char &scalar) const;
        CMYKRough operator/(const unsigned char &scalar) const;
        CMYKRough operator%(const unsigned char &denom) const;
};
class CMYKExact {
    private:
        double C = 0.0;
        double M = 0.0;
        double Y = 0.0;
        double K = 0.0;
        double A = 1.0;

        static unsigned char RelationMetric;
    
    public:
        CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k, const double &a);
        CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k);

        CMYKExact(const RGBRough &color);
        CMYKExact(const RGBExact &color);
        CMYKExact(const HSVRough &color);
        CMYKExact(const HSVExact &color);
        CMYKExact(const HSLRough &color);
        CMYKExact(const HSLExact &color);
        CMYKExact(const CMYKRough &color);
        CMYKExact(const CMYKExact &color);

        #ifdef SDL_pixels_h_
        CMYKExact(const SDL_Color &color);
        SDL_Color toSDL() const;
        #endif

        static unsigned char getRelationMetric();
        static unsigned char setRelationMetric(const unsigned char &metric = CMYK_RELATE_COMMON);
        static unsigned char adjRelationMetric(const unsigned char &amount);
        
        double getC() const;
        double getM() const;
        double getY() const;
        double getK() const;
        double getA() const;

        double setC(const double &c);
        double setM(const double &m);
        double setY(const double &y);
        double setK(const double &k);
        double setA(const double &a);

        double adjC(const double &amount);
        double adjM(const double &amount);
        double adjY(const double &amount);
        double adjK(const double &amount);
        double adjA(const double &amount);

        void operator=(const CMYKExact &color);
        CMYKExact operator!() const;
        CMYKRough toRough() const;

        #ifdef _GLIBCXX_STRING
        std::string toString(const bool &includeAlpha = false) const;
        #endif

        bool equal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool notequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool less(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool greater(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool lessequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool greaterequal(const CMYKExact &color, const unsigned char &metric = RelationMetric) const;
        bool operator==(const CMYKExact &color) const;
        bool operator!=(const CMYKExact &color) const;
        bool operator<(const CMYKExact &color) const;
        bool operator>(const CMYKExact &color) const;
        bool operator<=(const CMYKExact &color) const;
        bool operator>=(const CMYKExact &color) const;

        CMYKExact& operator+=(const CMYKExact &color);
        CMYKExact& operator-=(const CMYKExact &color);
        CMYKExact& operator*=(const double &scalar);
        CMYKExact& operator/=(const double &scalar);
        CMYKExact& operator%=(const double &denom);
        CMYKExact operator+(const CMYKExact &color) const;
        CMYKExact operator-(const CMYKExact &color) const;
        CMYKExact operator*(const double &scalar) const;
        CMYKExact operator/(const double &scalar) const;
        CMYKExact operator%(const double &denom) const;
};

#endif /* BCOLORS */
