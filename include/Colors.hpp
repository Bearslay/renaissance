#ifndef COLORS
#define COLORS

#include <string>
#include <SDL2/SDL_pixels.h>

#include "Utilities.hpp"

#define RGB_RELATE_COMMON 0
#define RGB_RELATE_RED    1
#define RGB_RELATE_GREEN  2
#define RGB_RELATE_BLUE   3
#define RGB_RELATE_ALPHA  4

#define HSV_RELATE_COMMON     0
#define HSV_RELATE_HUE        1
#define HSV_RELATE_SATURATION 2
#define HSV_RELATE_VALUE      3
#define HSV_RELATE_ALPHA      4

#define HSL_RELATE_COMMON     0
#define HSL_RELATE_HUE        1
#define HSL_RELATE_SATURATION 2
#define HSL_RELATE_LIGHTNESS  3
#define HSL_RELATE_ALPHA      4

#define CMYK_RELATE_COMMON  0
#define CMYK_RELATE_CYAN    1
#define CMYK_RELATE_MAGENTA 2
#define CMYK_RELATE_YELLOW  3
#define CMYK_RELATE_KEY     4
#define CMYK_RELATE_ALPHA   5

class RGBRough;
class RGBExact;
class HSVRough;
class HSVExact;
class HSLRough;
class HSLExact;
class CMYKRough;
class CMYKExact;

class RGBRough {
    private:
        unsigned char R = 255;
        unsigned char G = 255;
        unsigned char B = 255;
        unsigned char A = 255;

        static unsigned char RelationMetric;

    public:
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a) : R(r), G(g), B(b), A(a) {}
        RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b) : R(r), G(g), B(b) {}
        
        RGBRough(const  RGBRough &color) : R(color.getR()),                   G(color.getG()),                   B(color.getB()),                   A(color.getA())                   {}
        RGBRough(const  RGBExact &color) : R(std::round(color.getR() * 255)), G(std::round(color.getG() * 255)), B(std::round(color.getB() * 255)), A(std::round(color.getA() * 255)) {}
        RGBRough(const  HSVRough &color) {}
        RGBRough(const  HSVExact &color) {}
        RGBRough(const  HSLRough &color) {}
        RGBRough(const  HSLExact &color) {}
        RGBRough(const CMYKRough &color) {}
        RGBRough(const CMYKExact &color) {}

        RGBRough(const SDL_Color &color) : R(color.r), G(color.g), B(color.b), A(color.a) {}
        SDL_Color toSDL() const {return {R, G, B, A};}

        static unsigned char getRelationMetric()                                                {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON) {return                                 btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount)                     {return btils::normalize<unsigned char>(btils::adj<unsigned char>(RelationMetric, amount), 4);}

        unsigned char getR() const {return R;}
        unsigned char getG() const {return G;}
        unsigned char getB() const {return B;}
        unsigned char getA() const {return A;}

        unsigned char setR(const unsigned char &r) {return btils::set<unsigned char>(R, r);}
        unsigned char setG(const unsigned char &g) {return btils::set<unsigned char>(G, g);}
        unsigned char setB(const unsigned char &b) {return btils::set<unsigned char>(B, b);}
        unsigned char setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

        unsigned char adjR(const unsigned char &amount) {return btils::adj<unsigned char>(R, amount);}
        unsigned char adjG(const unsigned char &amount) {return btils::adj<unsigned char>(G, amount);}
        unsigned char adjB(const unsigned char &amount) {return btils::adj<unsigned char>(B, amount);}
        unsigned char adjA(const unsigned char &amount) {return btils::adj<unsigned char>(A, amount);}

        void operator=(const RGBRough &color) {
            R = color.getR();
            G = color.getG();
            B = color.getB();
            A = color.getA();
        }
        RGBRough    operator!()                                 const {return RGBRough(255 - R, 255 - G, 255 - B, A);}
        std::string  toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
        RGBExact      toExact() const {return RGBExact((double)(R / 255.0), (double)(G / 255.0), (double)(B / 255.0), (double)(A / 255.0));}

        bool        equal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {
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
        bool     notequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool         less(const RGBRough &color, const unsigned char &metric = RelationMetric) const {
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
        bool      greater(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool    lessequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const RGBRough &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool   operator==(const RGBRough &color) const {return        equal(color, RelationMetric);}
        bool   operator!=(const RGBRough &color) const {return     notequal(color, RelationMetric);}
        bool    operator<(const RGBRough &color) const {return         less(color, RelationMetric);}
        bool    operator>(const RGBRough &color) const {return      greater(color, RelationMetric);}
        bool   operator<=(const RGBRough &color) const {return    lessequal(color, RelationMetric);}
        bool   operator>=(const RGBRough &color) const {return greaterequal(color, RelationMetric);}

        RGBRough& operator+=(const      RGBRough & color) {
            R += color.getR();
            G += color.getG();
            B += color.getB();
            return *this;
        }
        RGBRough& operator-=(const      RGBRough & color) {
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
        RGBRough& operator%=(const unsigned char & denom) {
            R %= denom;
            G %= denom;
            B %= denom;
            return *this;
        }
        RGBRough   operator+(const      RGBRough & color) const {return RGBRough(R + color.getR(), G + color.getG(), B + color.getB());}
        RGBRough   operator-(const      RGBRough & color) const {return RGBRough(R - color.getR(), G - color.getG(), B - color.getB());}
        RGBRough   operator*(const unsigned char &scalar) const {return RGBRough(R *       scalar, G *       scalar, B *       scalar);}
        RGBRough   operator/(const unsigned char &scalar) const {return RGBRough(R /       scalar, G /       scalar, B /       scalar);}
        RGBRough   operator%(const unsigned char & denom) const {return RGBRough(R %        denom, G %        denom, B %        denom);}
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

        RGBExact(const  RGBRough &color) : R(color.getR() / 255.0), G(color.getG() / 255.0), B(color.getB() / 255.0), A(color.getA() / 255.0) {}
        RGBExact(const  RGBExact &color) : R(color.getR()),         G(color.getG()),         B(color.getB()),         A(color.getA())         {}
        RGBExact(const  HSVRough &color) {}
        RGBExact(const  HSVExact &color) {}
        RGBExact(const  HSLRough &color) {}
        RGBExact(const  HSLExact &color) {}
        RGBExact(const CMYKRough &color) {}
        RGBExact(const CMYKExact &color) {}

        RGBExact(const SDL_Color &color) : R(color.r / 255.0), G(color.g / 255.0), B(color.b / 255.0), A(color.a / 255.0) {}
        SDL_Color toSDL() const {return {std::round(R * 255.0), std::round(G * 255.0), std::round(B * 255.0), std::round(A * 255.0)};}

        static unsigned char getRelationMetric()                                                {return RelationMetric;}
        static unsigned char setRelationMetric(const unsigned char &metric = RGB_RELATE_COMMON) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
        static unsigned char adjRelationMetric(const unsigned char &amount)                     {return setRelationMetric(RelationMetric + amount);}

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
        RGBExact    operator!()                                 const {return RGBExact(1.0 - R, 1.0 - G, 1.0 - B, A);}
        std::string  toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
        RGBRough      toRough() const {return RGBRough(std::round(R * 255.0), std::round(G * 255.0), std::round(B * 255.0), std::round(A * 255.0));}

        bool        equal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {
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
        bool     notequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !equal(color, metric);}
        bool         less(const RGBExact &color, const unsigned char &metric = RelationMetric) const {
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
        bool      greater(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return color.less(*this, metric);}
        bool    lessequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !greater(*this, metric);}
        bool greaterequal(const RGBExact &color, const unsigned char &metric = RelationMetric) const {return !less(*this, metric);}
        bool   operator==(const RGBExact &color) const {return        equal(color, RelationMetric);}
        bool   operator!=(const RGBExact &color) const {return     notequal(color, RelationMetric);}
        bool    operator<(const RGBExact &color) const {return         less(color, RelationMetric);}
        bool    operator>(const RGBExact &color) const {return      greater(color, RelationMetric);}
        bool   operator<=(const RGBExact &color) const {return    lessequal(color, RelationMetric);}
        bool   operator>=(const RGBExact &color) const {return greaterequal(color, RelationMetric);}

        RGBExact& operator+=(const RGBExact & color) {
            adjR(color.getR());
            adjG(color.getG());
            adjB(color.getB());
            return *this;
        }
        RGBExact& operator-=(const RGBExact & color) {
            adjR(-color.getR());
            adjG(-color.getG());
            adjB(-color.getB());
            return *this;
        }
        RGBExact& operator*=(const   double &scalar) {
            R = btils::clamp<double>(R * scalar, 1.0);
            G = btils::clamp<double>(G * scalar, 1.0);
            B = btils::clamp<double>(B * scalar, 1.0);
            return *this;
        }
        RGBExact& operator/=(const   double &scalar) {
            R = btils::clamp<double>(R / scalar, 1.0);
            G = btils::clamp<double>(G / scalar, 1.0);
            B = btils::clamp<double>(B / scalar, 1.0);
            return *this;
        }
        RGBExact& operator%=(const   double & denom) {
            R = btils::clamp<double>(std::fmod(R, denom), 1.0);
            G = btils::clamp<double>(std::fmod(G, denom), 1.0);
            B = btils::clamp<double>(std::fmod(B, denom), 1.0);
            return *this;
        }
        RGBExact   operator+(const RGBExact & color) const {return RGBExact(R + color.getR(), G + color.getG(), B + color.getB());}
        RGBExact   operator-(const RGBExact & color) const {return RGBExact(R - color.getR(), G - color.getG(), B - color.getB());}
        RGBExact   operator*(const   double &scalar) const {return RGBExact(R *       scalar, G *       scalar, B *       scalar);}
        RGBExact   operator/(const   double &scalar) const {return RGBExact(R /       scalar, G /       scalar, B /       scalar);}
        RGBExact   operator%(const   double & denom) const {return RGBExact(std::fmod(R, denom), std::fmod(G, denom), std::fmod(B, denom));}
};
unsigned char RGBExact::RelationMetric = RGB_RELATE_COMMON;

#endif /* COLORS */
