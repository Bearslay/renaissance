#include "BColors.hpp"

unsigned char RGBRough::RelationMetric = RGB_RELATE_COMMON;

RGBRough::RGBRough() : R(255), G(255), B(255), A(255) {}
RGBRough::RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b, const unsigned char &a) : R(r), G(g), B(b), A(a) {}
RGBRough::RGBRough(const unsigned char &r, const unsigned char &g, const unsigned char &b) {RGBRough(r, g, b, 255);}

RGBRough::RGBRough(const RGBRough &color) : R(color.getR()), G(color.getG()), B(color.getB()), A(color.getA()) {}
// RGBRough::RGBRough(const RGBExact &color) : R(std::round(color.getR() * 255.0)), G(std::round(color.getG() * 255.0)), B(std::round(color.getB() * 255.0)), A(std::round(color.getA() * 255.0)) {}
// RGBRough::RGBRough(const HSVRough &color) {RGBRough(hsv2rgb(color));}
// RGBRough::RGBRough(const HSVExact &color) {RGBRough(hsv2rgb(color));}
// RGBRough::RGBRough(const HSLRough &color) {RGBRough(hsl2rgb(color));}
// RGBRough::RGBRough(const HSLExact &color) {RGBRough(hsl2rgb(color));}
// RGBRough::RGBRough(const CMYKRough &color) {RGBRough(cmyk2rgb(color));}
// RGBRough::RGBRough(const CMYKExact &color) {RGBRough(cmyk2rgb(color));}

#ifdef SDL_pixels_h_
RGBRough::RGBRough(const SDL_Color &color) : R(color.r), G(color.g), B(color.b), A(color.a) {}
SDL_Color RGBRough::toSDL() const {return {R, G, B, A};}
#endif

unsigned char RGBRough::getRelationMetric() {return RelationMetric;}
unsigned char RGBRough::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
unsigned char RGBRough::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

unsigned char RGBRough::getR() const {return R;}
unsigned char RGBRough::getG() const {return G;}
unsigned char RGBRough::getB() const {return B;}
unsigned char RGBRough::getA() const {return A;}

unsigned char RGBRough::setR(const unsigned char &r) {return btils::set<unsigned char>(R, r);}
unsigned char RGBRough::setG(const unsigned char &g) {return btils::set<unsigned char>(G, g);}
unsigned char RGBRough::setB(const unsigned char &b) {return btils::set<unsigned char>(B, b);}
unsigned char RGBRough::setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

unsigned char RGBRough::adjR(const unsigned char &amount) {return setR(R + amount);}
unsigned char RGBRough::adjG(const unsigned char &amount) {return setG(G + amount);}
unsigned char RGBRough::adjB(const unsigned char &amount) {return setB(B + amount);}
unsigned char RGBRough::adjA(const unsigned char &amount) {return setA(A + amount);}

void RGBRough::operator=(const RGBRough &color) {
    R = color.getR();
    G = color.getG();
    B = color.getB();
    A = color.getA();
}

RGBRough RGBRough::operator!() const {return RGBRough(255 - R, 255 - G, 255 - B, A);}
// RGBExact RGBRough::toExact() const {return RGBExact((double)(R / 255.0), (double)(G / 255.0), (double)(B / 255.0), (double)(A / 255.0));}

#ifdef _GLIBCXX_STRING
std::string toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool RGBRough::equal(const RGBRough &color, const unsigned char &metric) const {
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
bool RGBRough::notequal(const RGBRough &color, const unsigned char &metric) const {return !equal(color, metric);}
bool RGBRough::less(const RGBRough &color, const unsigned char &metric) const {
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
bool RGBRough::greater(const RGBRough &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool RGBRough::lessequal(const RGBRough &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool RGBRough::greaterequal(const RGBRough &color, const unsigned char &metric) const {return !less(*this, metric);}
bool RGBRough::operator==(const RGBRough &color) const {return equal(color, RelationMetric);}
bool RGBRough::operator!=(const RGBRough &color) const {return notequal(color, RelationMetric);}
bool RGBRough::operator<(const RGBRough &color) const {return less(color, RelationMetric);}
bool RGBRough::operator>(const RGBRough &color) const {return greater(color, RelationMetric);}
bool RGBRough::operator<=(const RGBRough &color) const {return lessequal(color, RelationMetric);}
bool RGBRough::operator>=(const RGBRough &color) const {return greaterequal(color, RelationMetric);}

RGBRough& RGBRough::operator+=(const RGBRough &color) {
    R += color.getR();
    G += color.getG();
    B += color.getB();
    return *this;
}
RGBRough& RGBRough::operator-=(const RGBRough &color) {
    R -= color.getR();
    G -= color.getG();
    B -= color.getB();
    return *this;
}
RGBRough& RGBRough::operator*=(const unsigned char &scalar) {
    R *= scalar;
    G *= scalar;
    B *= scalar;
    return *this;
}
RGBRough& RGBRough::operator/=(const unsigned char &scalar) {
    R /= scalar;
    G /= scalar;
    B /= scalar;
    return *this;
}
RGBRough& RGBRough::operator%=(const unsigned char &denom) {
    R %= denom;
    G %= denom;
    B %= denom;
    return *this;
}
RGBRough RGBRough::operator+(const RGBRough & color) const {return RGBRough(R + color.getR(), G + color.getG(), B + color.getB());}
RGBRough RGBRough::operator-(const RGBRough & color) const {return RGBRough(R - color.getR(), G - color.getG(), B - color.getB());}
RGBRough RGBRough::operator*(const unsigned char &scalar) const {return RGBRough(R * scalar, G * scalar, B * scalar);}
RGBRough RGBRough::operator/(const unsigned char &scalar) const {return RGBRough(R / scalar, G / scalar, B / scalar);}
RGBRough RGBRough::operator%(const unsigned char &denom) const {return RGBRough(R % denom, G % denom, B % denom);}



unsigned char RGBExact::RelationMetric = RGB_RELATE_COMMON;

RGBExact::RGBExact() : R(1.0), G(1.0), B(1.0), A(1.0) {}
RGBExact::RGBExact(const double &r, const double &g, const double &b, const double &a) : R(btils::clamp<double>(r, 1.0)), G(btils::clamp<double>(g, 1.0)), B(btils::clamp<double>(b, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
RGBExact::RGBExact(const double &r, const double &g, const double &b) {RGBExact(r, g, b, 1.0);}

// RGBExact::RGBExact(const RGBRough &color) : R(color.getR() / 255.0), G(color.getG() / 255.0), B(color.getB() / 255.0), A(color.getA() / 255.0) {}
RGBExact::RGBExact(const RGBExact &color) : R(color.getR()), G(color.getG()), B(color.getB()), A(color.getA()) {}
// RGBExact::RGBExact(const HSVRough &color) {RGBExact(hsv2rgb(color));}
// RGBExact::RGBExact(const HSVExact &color) {RGBExact(hsv2rgb(color));}
// RGBExact::RGBExact(const HSLRough &color) {RGBExact(hsl2rgb(color));}
// RGBExact::RGBExact(const HSLExact &color) {RGBExact(hsl2rgb(color));}
// RGBExact::RGBExact(const CMYKRough &color) {RGBExact(cmyk2rgb(color));}
// RGBExact::RGBExact(const CMYKExact &color) {RGBExact(cmyk2rgb(color));}

#ifdef SDL_pixels_h_
RGBExact::RGBExact(const SDL_Color &color) : R(color.r / 255.0), G(color.g / 255.0), B(color.b / 255.0), A(color.a / 255.0) {}
SDL_Color RGBExact::toSDL() const {return {(unsigned char)std::round(R * 255.0), (unsigned char)std::round(G * 255.0), (unsigned char)std::round(B * 255.0), (unsigned char)std::round(A * 255.0)};}
#endif

unsigned char RGBExact::getRelationMetric() {return RelationMetric;}
unsigned char RGBExact::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? RGB_RELATE_COMMON : metric);}
unsigned char RGBExact::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

double RGBExact::getR() const {return R;}
double RGBExact::getG() const {return G;}
double RGBExact::getB() const {return B;}
double RGBExact::getA() const {return A;}

double RGBExact::setR(const double &r) {return btils::set<double>(R, btils::clamp<double>(r, 1.0));}
double RGBExact::setG(const double &g) {return btils::set<double>(G, btils::clamp<double>(g, 1.0));}
double RGBExact::setB(const double &b) {return btils::set<double>(B, btils::clamp<double>(b, 1.0));}
double RGBExact::setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

double RGBExact::adjR(const double &amount) {return setR(R + amount);}
double RGBExact::adjG(const double &amount) {return setG(G + amount);}
double RGBExact::adjB(const double &amount) {return setB(B + amount);}
double RGBExact::adjA(const double &amount) {return setA(A + amount);}

void RGBExact::operator=(const RGBExact &color) {
    R = color.getR();
    G = color.getG();
    B = color.getB();
    A = color.getA();
}
RGBExact RGBExact::operator!() const {return RGBExact(1.0 - R, 1.0 - G, 1.0 - B, A);}
// RGBRough RGBExact::toRough() const {return RGBRough(std::round(R * 255.0), std::round(G * 255.0), std::round(B * 255.0), std::round(A * 255.0));}

#ifdef _GLIBCXX_STRING
std::string RGBExact::toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool RGBExact::equal(const RGBExact &color, const unsigned char &metric) const {
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
bool RGBExact::notequal(const RGBExact &color, const unsigned char &metric) const {return !equal(color, metric);}
bool RGBExact::less(const RGBExact &color, const unsigned char &metric) const {
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
bool RGBExact::greater(const RGBExact &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool RGBExact::lessequal(const RGBExact &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool RGBExact::greaterequal(const RGBExact &color, const unsigned char &metric) const {return !less(*this, metric);}
bool RGBExact::operator==(const RGBExact &color) const {return equal(color, RelationMetric);}
bool RGBExact::operator!=(const RGBExact &color) const {return notequal(color, RelationMetric);}
bool RGBExact::operator<(const RGBExact &color) const {return less(color, RelationMetric);}
bool RGBExact::operator>(const RGBExact &color) const {return greater(color, RelationMetric);}
bool RGBExact::operator<=(const RGBExact &color) const {return lessequal(color, RelationMetric);}
bool RGBExact::operator>=(const RGBExact &color) const {return greaterequal(color, RelationMetric);}

RGBExact& RGBExact::operator+=(const RGBExact &color) {
    R = btils::clamp<double>(R + color.getR(), 1.0);
    G = btils::clamp<double>(G + color.getG(), 1.0);
    B = btils::clamp<double>(B + color.getB(), 1.0);
    return *this;
}
RGBExact& RGBExact::operator-=(const RGBExact &color) {
    R = btils::clamp<double>(R - color.getR(), 1.0);
    G = btils::clamp<double>(G - color.getG(), 1.0);
    B = btils::clamp<double>(B - color.getB(), 1.0);
    return *this;
}
RGBExact& RGBExact::operator*=(const double &scalar) {
    R = btils::clamp<double>(R * scalar, 1.0);
    G = btils::clamp<double>(G * scalar, 1.0);
    B = btils::clamp<double>(B * scalar, 1.0);
    return *this;
}
RGBExact& RGBExact::operator/=(const double &scalar) {
    R = btils::clamp<double>(R / scalar, 1.0);
    G = btils::clamp<double>(G / scalar, 1.0);
    B = btils::clamp<double>(B / scalar, 1.0);
    return *this;
}
RGBExact& RGBExact::operator%=(const double &denom) {
    R = btils::clamp<double>(std::fmod(R, denom), 1.0);
    G = btils::clamp<double>(std::fmod(G, denom), 1.0);
    B = btils::clamp<double>(std::fmod(B, denom), 1.0);
    return *this;
}
RGBExact RGBExact::operator+(const RGBExact &color) const {return RGBExact(R + color.getR(), G + color.getG(), B + color.getB());}
RGBExact RGBExact::operator-(const RGBExact &color) const {return RGBExact(R - color.getR(), G - color.getG(), B - color.getB());}
RGBExact RGBExact::operator*(const double &scalar) const {return RGBExact(R * scalar, G * scalar, B * scalar);}
RGBExact RGBExact::operator/(const double &scalar) const {return RGBExact(R / scalar, G / scalar, B / scalar);}
RGBExact RGBExact::operator%(const double &denom) const {return RGBExact(std::fmod(R, denom), std::fmod(G, denom), std::fmod(B, denom));}



unsigned char HSVRough::RelationMetric = HSV_RELATE_COMMON;

HSVRough::HSVRough() : H(0), S(0), V(255) {}
HSVRough::HSVRough(const short &h, const unsigned char &s, const unsigned char &v, const unsigned char &a) : H(btils::clamp<short>(h, 360)), S(s), V(v), A(a) {}
HSVRough::HSVRough(const short &h, const unsigned char &s, const unsigned char &v) {HSVRough(h, s, v, 255);}

// HSVRough::HSVRough(const RGBRough &color) {HSVRough(rgb2hsv(color));}
// HSVRough::HSVRough(const RGBExact &color) {HSVRough(rgb2hsv(color));}
HSVRough::HSVRough(const HSVRough &color) : H(color.getH()), S(color.getS()), V(color.getV()), A(color.getA()) {}
// HSVRough::HSVRough(const HSVExact &color) : H(std::round(color.getH() * 360.0)), S(std::round(color.getS() * 255.0)), V(std::round(color.getV() * 255.0)), A(std::round(color.getA() * 255.0)) {}
// HSVRough::HSVRough(const HSLRough &color) {HSVRough(hsl2hsv(color));}
// HSVRough::HSVRough(const HSLExact &color) {HSVRough(hsl2hsv(color));}
// HSVRough::HSVRough(const CMYKRough &color) {HSVRough(cmyk2hsv(color));}
// HSVRough::HSVRough(const CMYKExact &color) {HSVRough(cmyk2hsv(color));}

#ifdef SDL_pixels_h_
HSVRough::HSVRough(const SDL_Color &color) {HSVRough(rgb2hsv(RGBRough(color)));}
SDL_Color HSVRough::toSDL() const {return hsv2rgb(*this).toSDL();}
#endif

unsigned char HSVRough::getRelationMetric() {return RelationMetric;}
unsigned char HSVRough::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSV_RELATE_COMMON : metric);}
unsigned char HSVRough::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

short HSVRough::getH() const {return H;}
unsigned char HSVRough::getS() const {return S;}
unsigned char HSVRough::getV() const {return V;}
unsigned char HSVRough::getA() const {return A;}

short HSVRough::setH(const short &h) {return btils::set<short>(H, btils::clamp<short>(h, 360));}
unsigned char HSVRough::setS(const unsigned char &s) {return btils::set<unsigned char>(S, s);}
unsigned char HSVRough::setV(const unsigned char &v) {return btils::set<unsigned char>(V, v);}
unsigned char HSVRough::setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

short HSVRough::adjH(const short &amount) {return setH(H + amount);}
unsigned char HSVRough::adjS(const unsigned char &amount) {return setS(S + amount);}
unsigned char HSVRough::adjV(const unsigned char &amount) {return setV(V + amount);}
unsigned char HSVRough::adjA(const unsigned char &amount) {return setA(A + amount);}

void HSVRough::operator=(const HSVRough &color) {
    H = color.getH();
    S = color.getS();
    V = color.getV();
    A = color.getA();
}
HSVRough HSVRough::operator!() const {return HSVRough(btils::normalize<short>(H + 180, 360), S, V, A);}
// HSVExact HSVRough::toExact() const {return HSVExact((double)(H / 360.0), (double)(S / 255.0), (double)(V / 255.0), (double)(A / 255.0));}

#ifdef _GLIBCXX_STRING
std::string HSVRough::toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(V) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool HSVRough::equal(const HSVRough &color, const unsigned char &metric) const {
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

    const short h1 = H < (short)(360 - H) ? H : (short)(360 - H), h2 = color.getH() < (short)(360 - color.getH()) ? color.getH() : (short)(360 - color.getH());
    return h1 * h1 + S * S + V * V == h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
}
bool HSVRough::notequal(const HSVRough &color, const unsigned char &metric) const {return !equal(color, metric);}
bool HSVRough::less(const HSVRough &color, const unsigned char &metric) const {
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

    const short h1 = H < (short)(360 - H) ? H : (short)(360 - H), h2 = color.getH() < (short)(360 - color.getH()) ? color.getH() : (short)(360 - color.getH());
    return h1 * h1 + S * S + V * V < h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
}
bool HSVRough::greater(const HSVRough &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool HSVRough::lessequal(const HSVRough &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool HSVRough::greaterequal(const HSVRough &color, const unsigned char &metric) const {return !less(*this, metric);}
bool HSVRough::operator==(const HSVRough &color) const {return equal(color, RelationMetric);}
bool HSVRough::operator!=(const HSVRough &color) const {return notequal(color, RelationMetric);}
bool HSVRough::operator<(const HSVRough &color) const {return less(color, RelationMetric);}
bool HSVRough::operator>(const HSVRough &color) const {return greater(color, RelationMetric);}
bool HSVRough::operator<=(const HSVRough &color) const {return lessequal(color, RelationMetric);}
bool HSVRough::operator>=(const HSVRough &color) const {return greaterequal(color, RelationMetric);}

HSVRough& HSVRough::operator+=(const HSVRough &color) {
    *this = rgb2hsv(hsv2rgb(*this) + hsv2rgb(color));
    return *this;
}
HSVRough& HSVRough::operator-=(const HSVRough &color) {
    *this = rgb2hsv(hsv2rgb(*this) - hsv2rgb(color));
    return *this;
}
HSVRough& HSVRough::operator*=(const double &scalar) {
    *this = rgb2hsv(hsv2rgb(*this) * scalar);
    return *this;
}
HSVRough& HSVRough::operator/=(const double &scalar) {
    *this = rgb2hsv(hsv2rgb(*this) / scalar);
    return *this;
}
HSVRough& HSVRough::operator%=(const double &denom) {
    *this = rgb2hsv(hsv2rgb(*this) % denom);
    return *this;
}
HSVRough HSVRough::operator+(const HSVRough &color) const {return rgb2hsv(hsv2rgb(*this) + hsv2rgb(color));}
HSVRough HSVRough::operator-(const HSVRough &color) const {return rgb2hsv(hsv2rgb(*this) - hsv2rgb(color));}
HSVRough HSVRough::operator*(const double &scalar) const {return rgb2hsv(hsv2rgb(*this) * scalar);}
HSVRough HSVRough::operator/(const double &scalar) const {return rgb2hsv(hsv2rgb(*this) / scalar);}
HSVRough HSVRough::operator%(const double &denom) const {return rgb2hsv(hsv2rgb(*this) % denom);}



unsigned char HSVExact::RelationMetric = HSV_RELATE_COMMON;

HSVExact::HSVExact() : H(0.0), S(0.0), V(1.0) {}
HSVExact::HSVExact(const double &h, const double &s, const double &v, const double &a) : H(btils::clamp<double>(h, 1.0)), S(btils::clamp<double>(s, 1.0)), V(btils::clamp<double>(v, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
HSVExact::HSVExact(const double &h, const double &s, const double &v) {HSVExact(h, s, v, 1.0);}

// HSVExact::HSVExact(const RGBRough &color) {HSVExact(rgb2hsv(color));}
// HSVExact::HSVExact(const RGBExact &color) {HSVExact(rgb2hsv(color));}
// HSVExact::HSVExact(const HSVRough &color) : H(color.getH() / 360.0), S(color.getS() / 255.0), V(color.getV() / 255.0), A(color.getA() / 255.0) {}
HSVExact::HSVExact(const HSVExact &color) : H(color.getH()), S(color.getS()), V(color.getV()), A(color.getA()) {}
// HSVExact::HSVExact(const HSLRough &color) {HSVExact(hsl2hsv(color));}
// HSVExact::HSVExact(const HSLExact &color) {HSVExact(hsl2hsv(color));}
// HSVExact::HSVExact(const CMYKRough &color) {HSVExact(cmyk2hsv(color));}
// HSVExact::HSVExact(const CMYKExact &color) {HSVExact(cmyk2hsv(color));}

#ifdef SDL_pixels_h_
HSVExact::HSVExact(const SDL_Color &color) {HSVExact(rgb2hsv(RGBExact(color)));}
SDL_Color HSVExact::toSDL() const {return hsv2rgb(*this).toSDL();}
#endif

unsigned char HSVExact::getRelationMetric() {return RelationMetric;}
unsigned char HSVExact::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSV_RELATE_COMMON : metric);}
unsigned char HSVExact::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

double HSVExact::getH() const {return H;}
double HSVExact::getS() const {return S;}
double HSVExact::getV() const {return V;}
double HSVExact::getA() const {return A;}

double HSVExact::setH(const double &h) {return btils::set<double>(H, btils::clamp<double>(h, 1.0));}
double HSVExact::setS(const double &s) {return btils::set<double>(S, btils::clamp<double>(s, 1.0));}
double HSVExact::setV(const double &v) {return btils::set<double>(V, btils::clamp<double>(v, 1.0));}
double HSVExact::setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

double HSVExact::adjH(const double &amount) {return setH(H + amount);}
double HSVExact::adjS(const double &amount) {return setS(S + amount);}
double HSVExact::adjV(const double &amount) {return setV(V + amount);}
double HSVExact::adjA(const double &amount) {return setA(A + amount);}

void HSVExact::operator=(const HSVExact &color) {
    H = color.getH();
    S = color.getS();
    V = color.getV();
    A = color.getA();
}
HSVExact HSVExact::operator!() const {return HSVExact(btils::normalize<double>(H + 0.5, 1.0), S, V, A);}
// HSVRough HSVExact::toRough() const {return HSVRough(std::round(H * 360.0), std::round(S * 255.0), std::round(V * 255.0), std::round(A * 255.0));}

#ifdef _GLIBCXX_STRING
std::string HSVExact::toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(V) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool HSVExact::equal(const HSVExact &color, const unsigned char &metric) const {
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

    const double h1 = H < 1.0 - H ? H : 1.0 - H, h2 = color.getH() < 1.0 - color.getH() ? color.getH() : 1.0 - color.getH();
    return h1 * h1 + S * S + V * V == h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
}
bool HSVExact::notequal(const HSVExact &color, const unsigned char &metric) const {return !equal(color, metric);}
bool HSVExact::less(const HSVExact &color, const unsigned char &metric) const {
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

    const double h1 = H < 1.0 - H ? H : 1.0 - H, h2 = color.getH() < 1.0 - color.getH() ? color.getH() : 1.0 - color.getH();
    return h1 * h1 + S * S + V * V < h2 * h2 + color.getS() * color.getS() + color.getV() * color.getV();
}
bool HSVExact::greater(const HSVExact &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool HSVExact::lessequal(const HSVExact &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool HSVExact::greaterequal(const HSVExact &color, const unsigned char &metric) const {return !less(*this, metric);}
bool HSVExact::operator==(const HSVExact &color) const {return equal(color, RelationMetric);}
bool HSVExact::operator!=(const HSVExact &color) const {return notequal(color, RelationMetric);}
bool HSVExact::operator<(const HSVExact &color) const {return less(color, RelationMetric);}
bool HSVExact::operator>(const HSVExact &color) const {return greater(color, RelationMetric);}
bool HSVExact::operator<=(const HSVExact &color) const {return lessequal(color, RelationMetric);}
bool HSVExact::operator>=(const HSVExact &color) const {return greaterequal(color, RelationMetric);}

HSVExact& HSVExact::operator+=(const HSVExact &color) {
    *this = rgb2hsv(hsv2rgb(*this) + hsv2rgb(color));
    return *this;
}
HSVExact& HSVExact::operator-=(const HSVExact &color) {
    *this = rgb2hsv(hsv2rgb(*this) - hsv2rgb(color));
    return *this;
}
HSVExact& HSVExact::operator*=(const double &scalar) {
    *this = rgb2hsv(hsv2rgb(*this) * scalar);
    return *this;
}
HSVExact& HSVExact::operator/=(const double &scalar) {
    *this = rgb2hsv(hsv2rgb(*this) / scalar);
    return *this;
}
HSVExact& HSVExact::operator%=(const double &denom) {
    *this = rgb2hsv(hsv2rgb(*this) % denom);
    return *this;
}
HSVExact HSVExact::operator+(const HSVExact &color) const {return rgb2hsv(hsv2rgb(*this) + hsv2rgb(color));}
HSVExact HSVExact::operator-(const HSVExact &color) const {return rgb2hsv(hsv2rgb(*this) - hsv2rgb(color));}
HSVExact HSVExact::operator*(const double &scalar) const {return rgb2hsv(hsv2rgb(*this) * scalar);}
HSVExact HSVExact::operator/(const double &scalar) const {return rgb2hsv(hsv2rgb(*this) / scalar);}
HSVExact HSVExact::operator%(const double &denom) const {return rgb2hsv(hsv2rgb(*this) % denom);}



unsigned char HSLRough::RelationMetric = HSL_RELATE_COMMON;

HSLRough::HSLRough() : H(0), S(0), L(255) {}
HSLRough::HSLRough(const short &h, const unsigned char &s, const unsigned char &l, const unsigned char &a) : H(btils::clamp<short>(h, 360)), S(s), L(l), A(a) {}
HSLRough::HSLRough(const short &h, const unsigned char &s, const unsigned char &l) {HSLRough(h, s, l, 255);}

// HSLRough::HSLRough(const RGBRough &color) {HSLRough(rgb2hsl(color));}
// HSLRough::HSLRough(const RGBExact &color) {HSLRough(rgb2hsl(color));}
// HSLRough::HSLRough(const HSVRough &color) {HSLRough(hsv2hsl(color));}
// HSLRough::HSLRough(const HSVExact &color) {HSLRough(hsv2hsl(color));}
HSLRough::HSLRough(const HSLRough &color) : H(color.getH()), S(color.getS()), L(color.getL()), A(color.getA()) {}
// HSLRough::HSLRough(const HSLExact &color) : H(std::round(color.getH() * 360.0)), S(std::round(color.getS() * 255.0)), L(std::round(color.getL() * 255.0)), A(std::round(color.getA() * 255.0)) {}
// HSLRough::HSLRough(const CMYKRough &color) {HSLRough(cmyk2hsl(color));}
// HSLRough::HSLRough(const CMYKExact &color) {HSLRough(cmyk2hsl(color));}

#ifdef SDL_pixels_h_
HSLRough::HSLRough(const SDL_Color &color) {HSLRough(rgb2hsl(RGBRough(color)));}
SDL_Color HSLRough::toSDL() const {return hsl2rgb(*this).toSDL();}
#endif

unsigned char HSLRough::getRelationMetric() {return RelationMetric;}
unsigned char HSLRough::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSL_RELATE_COMMON : metric);}
unsigned char HSLRough::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

short HSLRough::getH() const {return H;}
unsigned char HSLRough::getS() const {return S;}
unsigned char HSLRough::getL() const {return L;}
unsigned char HSLRough::getA() const {return A;}

short HSLRough::setH(const short &h) {return btils::set<short>(H, btils::clamp<short>(h, 360));}
unsigned char HSLRough::setS(const unsigned char &s) {return btils::set<unsigned char>(S, s);}
unsigned char HSLRough::setL(const unsigned char &l) {return btils::set<unsigned char>(L, l);}
unsigned char HSLRough::setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

short HSLRough::adjH(const short &amount) {return setH(H + amount);}
unsigned char HSLRough::adjS(const unsigned char &amount) {return setS(S + amount);}
unsigned char HSLRough::adjL(const unsigned char &amount) {return setL(L + amount);}
unsigned char HSLRough::adjA(const unsigned char &amount) {return setA(A + amount);}

void HSLRough::operator=(const HSLRough &color) {
    H = color.getH();
    S = color.getS();
    L = color.getL();
    A = color.getA();
}
HSLRough HSLRough::operator!() const {return HSLRough(btils::normalize<short>(H + 180, 360), S, L, A);}
// HSLExact HSLRough::toExact() const {return HSLExact((double)(H / 360.0), (double)(S / 255.0), (double)(L / 255.0), (double)(A / 255.0));}

#ifdef _GLIBCXX_STRING
std::string HSLRough::toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(L) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool HSLRough::equal(const HSLRough &color, const unsigned char &metric) const {
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

    const short h1 = H < (short)(360 - H) ? H : (short)(360 - H), h2 = color.getH() < (short)(360 - color.getH()) ? color.getH() : (short)(360 - color.getH());
    return h1 * h1 + S * S + L * L == h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
}
bool HSLRough::notequal(const HSLRough &color, const unsigned char &metric) const {return !equal(color, metric);}
bool HSLRough::less(const HSLRough &color, const unsigned char &metric) const {
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

    const short h1 = H < (short)(360 - H) ? H : (short)(360 - H), h2 = color.getH() < (short)(360 - color.getH()) ? color.getH() : (short)(360 - color.getH());
    return h1 * h1 + S * S + L * L < h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
}
bool HSLRough::greater(const HSLRough &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool HSLRough::lessequal(const HSLRough &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool HSLRough::greaterequal(const HSLRough &color, const unsigned char &metric) const {return !less(*this, metric);}
bool HSLRough::operator==(const HSLRough &color) const {return equal(color, RelationMetric);}
bool HSLRough::operator!=(const HSLRough &color) const {return notequal(color, RelationMetric);}
bool HSLRough::operator<(const HSLRough &color) const {return less(color, RelationMetric);}
bool HSLRough::operator>(const HSLRough &color) const {return greater(color, RelationMetric);}
bool HSLRough::operator<=(const HSLRough &color) const {return lessequal(color, RelationMetric);}
bool HSLRough::operator>=(const HSLRough &color) const {return greaterequal(color, RelationMetric);}

HSLRough& HSLRough::operator+=(const HSLRough &color) {
    *this = rgb2hsl(hsl2rgb(*this) + hsl2rgb(color));
    return *this;
}
HSLRough& HSLRough::operator-=(const HSLRough &color) {
    *this = rgb2hsl(hsl2rgb(*this) - hsl2rgb(color));
    return *this;
}
HSLRough& HSLRough::operator*=(const double &scalar) {
    *this = rgb2hsl(hsl2rgb(*this) * scalar);
    return *this;
}
HSLRough& HSLRough::operator/=(const double &scalar) {
    *this = rgb2hsl(hsl2rgb(*this) / scalar);
    return *this;
}
HSLRough& HSLRough::operator%=(const double &denom) {
    *this = rgb2hsl(hsl2rgb(*this) % denom);
    return *this;
}
HSLRough HSLRough::operator+(const HSLRough &color) const {return rgb2hsl(hsl2rgb(*this) + hsl2rgb(color));}
HSLRough HSLRough::operator-(const HSLRough &color) const {return rgb2hsl(hsl2rgb(*this) - hsl2rgb(color));}
HSLRough HSLRough::operator*(const double &scalar) const {return rgb2hsl(hsl2rgb(*this) * scalar);}
HSLRough HSLRough::operator/(const double &scalar) const {return rgb2hsl(hsl2rgb(*this) / scalar);}
HSLRough HSLRough::operator%(const double &denom) const {return rgb2hsl(hsl2rgb(*this) % denom);}



unsigned char HSLExact::RelationMetric = HSL_RELATE_COMMON;

HSLExact::HSLExact() : H(0.0), S(0.0), L(1.0) {}
HSLExact::HSLExact(const double &h, const double &s, const double &l, const double &a) : H(btils::clamp<double>(h, 1.0)), S(btils::clamp<double>(s, 1.0)), L(btils::clamp<double>(l, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
HSLExact::HSLExact(const double &h, const double &s, const double &l) {HSLExact(h, s, l, 1.0);}

// HSLExact::HSLExact(const RGBRough &color) {HSLExact(rgb2hsl(color));}
// HSLExact::HSLExact(const RGBExact &color) {HSLExact(rgb2hsl(color));}
// HSLExact::HSLExact(const HSVRough &color) {HSLExact(hsv2hsl(color));}
// HSLExact::HSLExact(const HSVExact &color) {HSLExact(hsv2hsl(color));}
// HSLExact::HSLExact(const HSLRough &color) : H(color.getH() / 360.0), S(color.getS() / 255.0), L(color.getL() / 255.0), A(color.getA() / 255.0) {}
HSLExact::HSLExact(const HSLExact &color) : H(color.getH()), S(color.getS()), L(color.getL()), A(color.getA()) {}
// HSLExact::HSLExact(const CMYKRough &color) {HSLExact(cmyk2hsl(color));}
// HSLExact::HSLExact(const CMYKExact &color) {HSLExact(cmyk2hsl(color));}

#ifdef SDL_pixels_h_
HSLExact::HSLExact(const SDL_Color &color) {HSLExact(rgb2hsl(RGBExact(color)));}
SDL_Color HSLExact::toSDL() const {return hsl2rgb(*this).toSDL();}
#endif

unsigned char HSLExact::getRelationMetric() {return RelationMetric;}
unsigned char HSLExact::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 4 || metric < 0 ? HSL_RELATE_COMMON : metric);}
unsigned char HSLExact::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

double HSLExact::getH() const {return H;}
double HSLExact::getS() const {return S;}
double HSLExact::getL() const {return L;}
double HSLExact::getA() const {return A;}

double HSLExact::setH(const double &h) {return btils::set<double>(H, btils::clamp<double>(h, 1.0));}
double HSLExact::setS(const double &s) {return btils::set<double>(S, btils::clamp<double>(s, 1.0));}
double HSLExact::setL(const double &l) {return btils::set<double>(L, btils::clamp<double>(l, 1.0));}
double HSLExact::setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

double HSLExact::adjH(const double &amount) {return setH(H + amount);}
double HSLExact::adjS(const double &amount) {return setS(S + amount);}
double HSLExact::adjL(const double &amount) {return setL(L + amount);}
double HSLExact::adjA(const double &amount) {return setA(A + amount);}

void HSLExact::operator=(const HSLExact &color) {
    H = color.getH();
    S = color.getS();
    L = color.getL();
    A = color.getA();
}
HSLExact HSLExact::operator!() const {return HSLExact(btils::normalize<double>(H + 0.5, 1.0), S, L, A);}
// HSLRough HSLExact::toRough() const {return HSLRough(std::round(H * 360.0), std::round(S * 255.0), std::round(L * 255.0), std::round(A * 255.0));}

#ifdef _GLIBCXX_STRING
std::string HSLExact::toString(const bool &includeAlpha = false) const {return std::to_string(H) + "°, " + std::to_string(S) + "%, " + std::to_string(L) + "%" + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool HSLExact::equal(const HSLExact &color, const unsigned char &metric) const {
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

    const double h1 = H < 1.0 - H ? H : 1.0 - H, h2 = color.getH() < 1.0 - color.getH() ? color.getH() : 1.0 - color.getH();
    return h1 * h1 + S * S + L * L == h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
}
bool HSLExact::notequal(const HSLExact &color, const unsigned char &metric) const {return !equal(color, metric);}
bool HSLExact::less(const HSLExact &color, const unsigned char &metric) const {
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

    const double h1 = H < 1.0 - H ? H : 1.0 - H, h2 = color.getH() < 1.0 - color.getH() ? color.getH() : 1.0 - color.getH();
    return h1 * h1 + S * S + L * L < h2 * h2 + color.getS() * color.getS() + color.getL() * color.getL();
}
bool HSLExact::greater(const HSLExact &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool HSLExact::lessequal(const HSLExact &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool HSLExact::greaterequal(const HSLExact &color, const unsigned char &metric) const {return !less(*this, metric);}
bool HSLExact::operator==(const HSLExact &color) const {return equal(color, RelationMetric);}
bool HSLExact::operator!=(const HSLExact &color) const {return notequal(color, RelationMetric);}
bool HSLExact::operator<(const HSLExact &color) const {return less(color, RelationMetric);}
bool HSLExact::operator>(const HSLExact &color) const {return greater(color, RelationMetric);}
bool HSLExact::operator<=(const HSLExact &color) const {return lessequal(color, RelationMetric);}
bool HSLExact::operator>=(const HSLExact &color) const {return greaterequal(color, RelationMetric);}

HSLExact& HSLExact::operator+=(const HSLExact &color) {
    *this = rgb2hsl(hsl2rgb(*this) + hsl2rgb(color));
    return *this;
}
HSLExact& HSLExact::operator-=(const HSLExact &color) {
    *this = rgb2hsl(hsl2rgb(*this) - hsl2rgb(color));
    return *this;
}
HSLExact& HSLExact::operator*=(const double &scalar) {
    *this = rgb2hsl(hsl2rgb(*this) * scalar);
    return *this;
}
HSLExact& HSLExact::operator/=(const double &scalar) {
    *this = rgb2hsl(hsl2rgb(*this) / scalar);
    return *this;
}
HSLExact& HSLExact::operator%=(const double &denom) {
    *this = rgb2hsl(hsl2rgb(*this) % denom);
    return *this;
}
HSLExact HSLExact::operator+(const HSLExact &color) const {return rgb2hsl(hsl2rgb(*this) + hsl2rgb(color));}
HSLExact HSLExact::operator-(const HSLExact &color) const {return rgb2hsl(hsl2rgb(*this) - hsl2rgb(color));}
HSLExact HSLExact::operator*(const double &scalar) const {return rgb2hsl(hsl2rgb(*this) * scalar);}
HSLExact HSLExact::operator/(const double &scalar) const {return rgb2hsl(hsl2rgb(*this) / scalar);}
HSLExact HSLExact::operator%(const double &denom) const {return rgb2hsl(hsl2rgb(*this) % denom);}



unsigned char CMYKRough::RelationMetric = CMYK_RELATE_COMMON;

CMYKRough::CMYKRough() : C(0), M(0), Y(0), K(0) {}
CMYKRough::CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k, const unsigned char &a) : C(c), M(m), Y(y), K(k), A(a) {}
CMYKRough::CMYKRough(const unsigned char &c, const unsigned char &m, const unsigned char &y, const unsigned char &k) {CMYKRough(c, m, y, k, 255);}

// CMYKRough::CMYKRough(const RGBRough &color) {CMYKRough(rgb2cmyk(color));}
// CMYKRough::CMYKRough(const RGBExact &color) {CMYKRough(rgb2cmyk(color));}
// CMYKRough::CMYKRough(const HSVRough &color) {CMYKRough(hsv2cmyk(color));}
// CMYKRough::CMYKRough(const HSVExact &color) {CMYKRough(hsv2cmyk(color));}
// CMYKRough::CMYKRough(const HSLRough &color) {CMYKRough(hsl2cmyk(color));}
// CMYKRough::CMYKRough(const HSLExact &color) {CMYKRough(hsl2cmyk(color));}
CMYKRough::CMYKRough(const CMYKRough &color) : C(color.getC()), M(color.getM()), Y(color.getY()), K(color.getK()), A(color.getA()) {}
// CMYKRough::CMYKRough(const CMYKExact &color) : C(std::round(color.getC() * 255.0)), M(std::round(color.getM() * 255.0)), Y(std::round(color.getY() * 255.0)), K(std::round(color.getK() * 255.0)), A(std::round(color.getA() * 255.0)) {}

#ifdef SDL_pixels_h_
CMYKRough::CMYKRough(const SDL_Color &color) {CMYKRough(rgb2cmyk(RGBRough(color)));}
SDL_Color CMYKRough::toSDL() const {return cmyk2rgb(*this).toSDL();}
#endif

unsigned char CMYKRough::getRelationMetric() {return RelationMetric;}
unsigned char CMYKRough::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 5 || metric < 0 ? CMYK_RELATE_COMMON : metric);}
unsigned char CMYKRough::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

unsigned char CMYKRough::getC() const {return C;}
unsigned char CMYKRough::getM() const {return M;}
unsigned char CMYKRough::getY() const {return Y;}
unsigned char CMYKRough::getK() const {return K;}
unsigned char CMYKRough::getA() const {return A;}

unsigned char CMYKRough::setC(const unsigned char &c) {return btils::set<unsigned char>(C, c);}
unsigned char CMYKRough::setM(const unsigned char &m) {return btils::set<unsigned char>(M, m);}
unsigned char CMYKRough::setY(const unsigned char &y) {return btils::set<unsigned char>(Y, y);}
unsigned char CMYKRough::setK(const unsigned char &k) {return btils::set<unsigned char>(K, k);}
unsigned char CMYKRough::setA(const unsigned char &a) {return btils::set<unsigned char>(A, a);}

unsigned char CMYKRough::adjC(const unsigned char &amount) {return setC(C + amount);}
unsigned char CMYKRough::adjM(const unsigned char &amount) {return setM(M + amount);}
unsigned char CMYKRough::adjY(const unsigned char &amount) {return setY(Y + amount);}
unsigned char CMYKRough::adjK(const unsigned char &amount) {return setK(K + amount);}
unsigned char CMYKRough::adjA(const unsigned char &amount) {return setA(A + amount);}

void CMYKRough::operator=(const CMYKRough &color) {
    C = color.getC();
    M = color.getM();
    Y = color.getY();
    K = color.getK();
    A = color.getA();
}
CMYKRough CMYKRough::operator!() const {return CMYKRough(255 - C, 255 - M, 255 - Y, 255 - K, A);}
// CMYKExact CMYKRough::toExact() const {return CMYKExact((double)(C / 255.0), (double)(M / 255.0), (double)(Y / 255.0), (double)(K / 255.0), (double)(A / 255.0));}

#ifdef _GLIBCXX_STRING
std::string CMYKRough::toString(const bool &includeAlpha = false) const {return std::to_string(C) + ", " + std::to_string(M) + ", " + std::to_string(Y) + ", " + std::to_string(K) + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool CMYKRough::equal(const CMYKRough &color, const unsigned char &metric) const {
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
bool CMYKRough::notequal(const CMYKRough &color, const unsigned char &metric) const {return !equal(color, metric);}
bool CMYKRough::less(const CMYKRough &color, const unsigned char &metric) const {
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
bool CMYKRough::greater(const CMYKRough &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool CMYKRough::lessequal(const CMYKRough &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool CMYKRough::greaterequal(const CMYKRough &color, const unsigned char &metric) const {return !less(*this, metric);}
bool CMYKRough::operator==(const CMYKRough &color) const {return equal(color, RelationMetric);}
bool CMYKRough::operator!=(const CMYKRough &color) const {return notequal(color, RelationMetric);}
bool CMYKRough::operator<(const CMYKRough &color) const {return less(color, RelationMetric);}
bool CMYKRough::operator>(const CMYKRough &color) const {return greater(color, RelationMetric);}
bool CMYKRough::operator<=(const CMYKRough &color) const {return lessequal(color, RelationMetric);}
bool CMYKRough::operator>=(const CMYKRough &color) const {return greaterequal(color, RelationMetric);}

CMYKRough& CMYKRough::operator+=(const CMYKRough &color) {
    C += color.getC();
    M += color.getM();
    Y += color.getY();
    K += color.getK();
    return *this;
}
CMYKRough& CMYKRough::operator-=(const CMYKRough &color) {
    C -= color.getC();
    M -= color.getM();
    Y -= color.getY();
    K -= color.getK();
    return *this;
}
CMYKRough& CMYKRough::operator*=(const unsigned char &scalar) {
    C *= scalar;
    M *= scalar;
    Y *= scalar;
    K *= scalar;
    return *this;
}
CMYKRough& CMYKRough::operator/=(const unsigned char &scalar) {
    C /= scalar;
    M /= scalar;
    Y /= scalar;
    K /= scalar;
    return *this;
}
CMYKRough& CMYKRough::operator%=(const unsigned char &denom) {
    C %= denom;
    M %= denom;
    Y %= denom;
    K %= denom;
    return *this;
}
CMYKRough CMYKRough::operator+(const CMYKRough & color) const {return CMYKRough(C + color.getC(), M + color.getM(), Y + color.getY(), K + color.getK());}
CMYKRough CMYKRough::operator-(const CMYKRough & color) const {return CMYKRough(C - color.getC(), M - color.getM(), Y - color.getY(), K - color.getK());}
CMYKRough CMYKRough::operator*(const unsigned char &scalar) const {return CMYKRough(C * scalar, M * scalar, Y * scalar, K * scalar);}
CMYKRough CMYKRough::operator/(const unsigned char &scalar) const {return CMYKRough(C / scalar, M / scalar, Y / scalar, K / scalar);}
CMYKRough CMYKRough::operator%(const unsigned char &denom) const {return CMYKRough(C % denom, M % denom, Y % denom, K % denom);}



unsigned char CMYKExact::RelationMetric = CMYK_RELATE_COMMON;

CMYKExact::CMYKExact() : C(0.0), M(0.0), Y(0.0), K(0.0) {}
CMYKExact::CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k, const double &a) : C(btils::clamp<double>(c, 1.0)), M(btils::clamp<double>(m, 1.0)), Y(btils::clamp<double>(y, 1.0)), K(btils::clamp<double>(k, 1.0)), A(btils::clamp<double>(a, 1.0)) {}
CMYKExact::CMYKExact(const double &c, const double &m, const double &y, const unsigned char &k) {CMYKExact(c, m, y, k, 1.0);}

// CMYKExact::CMYKExact(const RGBRough &color) {CMYKExact(rgb2cmyk(color));}
// CMYKExact::CMYKExact(const RGBExact &color) {CMYKExact(rgb2cmyk(color));}
// CMYKExact::CMYKExact(const HSVRough &color) {CMYKExact(hsv2cmyk(color));}
// CMYKExact::CMYKExact(const HSVExact &color) {CMYKExact(hsv2cmyk(color));}
// CMYKExact::CMYKExact(const HSLRough &color) {CMYKExact(hsl2cmyk(color));}
// CMYKExact::CMYKExact(const HSLExact &color) {CMYKExact(hsl2cmyk(color));}
// CMYKExact::CMYKExact(const CMYKRough &color) : C(color.getC() / 255.0), M(color.getM() / 255.0), Y(color.getY() / 255.0), K(color.getK() / 255.0), A(color.getA() / 255.0) {}
CMYKExact::CMYKExact(const CMYKExact &color) : C(color.getC()), M(color.getM()), Y(color.getY()), K(color.getK()), A(color.getA()) {}

#ifdef SDL_pixels_h_
CMYKExact::CMYKExact(const SDL_Color &color) {CMYKExact(rgb2cmyk(RGBExact(color)));}
SDL_Color CMYKExact::toSDL() const {return cmyk2rgb(*this).toSDL();}
#endif

unsigned char CMYKExact::getRelationMetric() {return RelationMetric;}
unsigned char CMYKExact::setRelationMetric(const unsigned char &metric) {return btils::set<unsigned char>(RelationMetric, metric > 5 || metric < 0 ? CMYK_RELATE_COMMON : metric);}
unsigned char CMYKExact::adjRelationMetric(const unsigned char &amount) {return setRelationMetric(RelationMetric + amount);}

double CMYKExact::getC() const {return C;}
double CMYKExact::getM() const {return M;}
double CMYKExact::getY() const {return Y;}
double CMYKExact::getK() const {return K;}
double CMYKExact::getA() const {return A;}

double CMYKExact::setC(const double &c) {return btils::set<double>(C, btils::clamp<double>(c, 1.0));}
double CMYKExact::setM(const double &m) {return btils::set<double>(M, btils::clamp<double>(m, 1.0));}
double CMYKExact::setY(const double &y) {return btils::set<double>(Y, btils::clamp<double>(y, 1.0));}
double CMYKExact::setK(const double &k) {return btils::set<double>(K, btils::clamp<double>(k, 1.0));}
double CMYKExact::setA(const double &a) {return btils::set<double>(A, btils::clamp<double>(a, 1.0));}

double CMYKExact::adjC(const double &amount) {return setC(C + amount);}
double CMYKExact::adjM(const double &amount) {return setM(M + amount);}
double CMYKExact::adjY(const double &amount) {return setY(Y + amount);}
double CMYKExact::adjK(const double &amount) {return setK(K + amount);}
double CMYKExact::adjA(const double &amount) {return setA(A + amount);}

void CMYKExact::operator=(const CMYKExact &color) {
    C = color.getC();
    M = color.getM();
    Y = color.getY();
    K = color.getK();
    A = color.getA();
}
CMYKExact CMYKExact::operator!() const {return CMYKExact(1.0 - C, 1.0 - M, 1.0 - Y, 1.0 - K, A);}
// CMYKRough CMYKExact::toRough() const {return CMYKRough(std::round(C * 255.0), std::round(M * 255.0), std::round(Y * 255.0), std::round(K * 255.0), std::round(A * 255.0));}

#ifdef _GLIBCXX_STRING
std::string CMYKExact::toString(const bool &includeAlpha = false) const {return std::to_string(C) + ", " + std::to_string(M) + ", " + std::to_string(Y) + ", " + std::to_string(K) + (includeAlpha ? ", " + std::to_string(A) : "");}
#endif

bool CMYKExact::equal(const CMYKExact &color, const unsigned char &metric) const {
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
bool CMYKExact::notequal(const CMYKExact &color, const unsigned char &metric) const {return !equal(color, metric);}
bool CMYKExact::less(const CMYKExact &color, const unsigned char &metric) const {
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
bool CMYKExact::greater(const CMYKExact &color, const unsigned char &metric) const {return color.less(*this, metric);}
bool CMYKExact::lessequal(const CMYKExact &color, const unsigned char &metric) const {return !greater(*this, metric);}
bool CMYKExact::greaterequal(const CMYKExact &color, const unsigned char &metric) const {return !less(*this, metric);}
bool CMYKExact::operator==(const CMYKExact &color) const {return equal(color, RelationMetric);}
bool CMYKExact::operator!=(const CMYKExact &color) const {return notequal(color, RelationMetric);}
bool CMYKExact::operator<(const CMYKExact &color) const {return less(color, RelationMetric);}
bool CMYKExact::operator>(const CMYKExact &color) const {return greater(color, RelationMetric);}
bool CMYKExact::operator<=(const CMYKExact &color) const {return lessequal(color, RelationMetric);}
bool CMYKExact::operator>=(const CMYKExact &color) const {return greaterequal(color, RelationMetric);}

CMYKExact& CMYKExact::operator+=(const CMYKExact &color) {
    C = btils::clamp<double>(C + color.getC(), 1.0);
    M = btils::clamp<double>(M + color.getM(), 1.0);
    Y = btils::clamp<double>(Y + color.getY(), 1.0);
    K = btils::clamp<double>(K + color.getK(), 1.0);
    return *this;
}
CMYKExact& CMYKExact::operator-=(const CMYKExact &color) {
    C = btils::clamp<double>(C - color.getC(), 1.0);
    M = btils::clamp<double>(M - color.getM(), 1.0);
    Y = btils::clamp<double>(Y - color.getY(), 1.0);
    K = btils::clamp<double>(K - color.getK(), 1.0);
    return *this;
}
CMYKExact& CMYKExact::operator*=(const double &scalar) {
    C = btils::clamp<double>(C * scalar, 1.0);
    M = btils::clamp<double>(M * scalar, 1.0);
    Y = btils::clamp<double>(Y * scalar, 1.0);
    K = btils::clamp<double>(K * scalar, 1.0);
    return *this;
}
CMYKExact& CMYKExact::operator/=(const double &scalar) {
    C = btils::clamp<double>(C / scalar, 1.0);
    M = btils::clamp<double>(M / scalar, 1.0);
    Y = btils::clamp<double>(Y / scalar, 1.0);
    K = btils::clamp<double>(K / scalar, 1.0);
    return *this;
}
CMYKExact& CMYKExact::operator%=(const double &denom) {
    C = btils::clamp<double>(std::fmod(C, denom), 1.0);
    M = btils::clamp<double>(std::fmod(M, denom), 1.0);
    Y = btils::clamp<double>(std::fmod(Y, denom), 1.0);
    K = btils::clamp<double>(std::fmod(K, denom), 1.0);
    return *this;
}
CMYKExact CMYKExact::operator+(const CMYKExact &color) const {return CMYKExact(C + color.getC(), M + color.getM(), Y + color.getY(), K + color.getK());}
CMYKExact CMYKExact::operator-(const CMYKExact &color) const {return CMYKExact(C - color.getC(), M - color.getM(), Y - color.getY(), K - color.getK());}
CMYKExact CMYKExact::operator*(const double &scalar) const {return CMYKExact(C * scalar, M * scalar, Y * scalar, K * scalar);}
CMYKExact CMYKExact::operator/(const double &scalar) const {return CMYKExact(C / scalar, M / scalar, Y / scalar, K / scalar);}
CMYKExact CMYKExact::operator%(const double &denom) const {return CMYKExact(std::fmod(C, denom), std::fmod(M, denom), std::fmod(Y, denom), std::fmod(K, denom));}



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
HSLRough rgb2hsl(const RGBRough &rgb) {return hsv2hsl(rgb2hsv(rgb));}
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
CMYKRough hsv2cmyk(const HSVRough &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBRough hsl2rgb(const HSLRough &hsl) {return hsv2rgb(hsl2hsv(hsl));}
HSVRough hsl2hsv(const HSLRough &hsl) {
    const unsigned char v = hsl.getL() + hsl.getS() * (hsl.getL() <= 255 - hsl.getL() ? hsl.getL() : 255 - hsl.getL());
    return HSVRough(hsl.getH(), v == 0 ? 0 : 2 * (255 - hsl.getL() / v), v, hsl.getA());
}
CMYKRough hsl2cmyk(const HSLRough &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
RGBRough cmyk2rgb(const CMYKRough &cmyk) {return RGBRough((255 - cmyk.getC()) * (255 - cmyk.getK()), (255 - cmyk.getM()) * (255 - cmyk.getK()), (255 - cmyk.getY()) * (255 - cmyk.getK()), cmyk.getA());}
HSVRough cmyk2hsv(const CMYKRough &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLRough cmyk2hsl(const CMYKRough &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}

HSVExact rgb2hsv(const RGBExact &rgb) {
    const double min = rgb.getR() < rgb.getG() ? (rgb.getR() < rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() < rgb.getB() ? rgb.getG() : rgb.getB());
    const double max = rgb.getR() > rgb.getG() ? (rgb.getR() > rgb.getB() ? rgb.getR() : rgb.getB()) : (rgb.getG() > rgb.getB() ? rgb.getG() : rgb.getB());

    const double delta = max - min;
    if (delta < 0.00001) {return HSVExact(0.0, 0.0, max, rgb.getA());}
    if (max <= 0.0) {return HSVExact(0.0, 0.0, max, rgb.getA());}

    double h = 0.0;
    if (rgb.getR() >= max) {h = (rgb.getG() - rgb.getB()) / delta;}
    else if (rgb.getG() >= max) {h = 2.0 + (rgb.getB() - rgb.getR()) / delta;}
    else {h = 4.0 + (rgb.getR() - rgb.getG()) / delta;}

    if ((h *= 60.0) < 0.0) {h += 360.0;}
    return HSVExact(h, delta / max, max, rgb.getA());
}
HSLExact rgb2hsl(const RGBExact &rgb) {return hsv2hsl(rgb2hsv(rgb));}
CMYKExact rgb2cmyk(const RGBExact &rgb) {
    const double k = 1.0 - ((rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) >= rgb.getB() ? (rgb.getR() >= rgb.getG() ? rgb.getR() : rgb.getG()) : rgb.getB());
    return CMYKExact((1.0 - rgb.getR() - k) / (1.0 - k), (1.0 - rgb.getG() - k) / (1.0 - k), (1.0 - rgb.getB() - k) / (1.0 - k), k, rgb.getA());
}
RGBExact hsv2rgb(const HSVExact &hsv) {
    if (hsv.getS() <= 0.0) {return RGBExact(hsv.getV(), hsv.getV(), hsv.getV(), hsv.getA());}

    const double h = (hsv.getH() >= 360.0 ? 0.0 : hsv.getH()) / 60.0;
    const long region = (long)h;
    const double p = hsv.getV() * (1.0 - hsv.getS());
    const double q = hsv.getV() * (1.0 - (hsv.getS() * (h - region)));
    const double t = hsv.getV() * (1.0 - (hsv.getS() * (1.0 - (h - region))));

    switch (region) {
        case 0:
            return RGBExact(hsv.getV(), t, p, hsv.getA());
            break;
        case 1:
            return RGBExact(q, hsv.getV(), p, hsv.getA());
            break;
        case 2:
            return RGBExact(p, hsv.getV(), t, hsv.getA());
            break;
        case 3:
            return RGBExact(p, q, hsv.getV(), hsv.getA());
            break;
        case 4:
            return RGBExact(t, p, hsv.getV(), hsv.getA());
            break;
        default:
            return RGBExact(hsv.getV(), p, q, hsv.getA());
            break;
    }
}
HSLExact hsv2hsl(const HSVExact &hsv) {
    const double l = hsv.getV() * (1.0 - hsv.getS() / 2.0);
    return HSLExact(hsv.getH(), l <= 0.0 || l >= 1.0 ? 0.0 : ((hsv.getV() - l) / (l <= 1.0 - l ? l : 1.0 - l)), l, hsv.getA());
}
CMYKExact hsv2cmyk(const HSVExact &hsv) {return rgb2cmyk(hsv2rgb(hsv));}
RGBExact hsl2rgb(const HSLExact &hsl) {return hsv2rgb(hsl2hsv(hsl));}
HSVExact hsl2hsv(const HSLExact &hsl) {
    const double v = hsl.getL() + hsl.getS() * (hsl.getL() <= 1.0 - hsl.getL() ? hsl.getL() : 1.0 - hsl.getL());
    return HSVExact(hsl.getH(), v == 0.0 ? 0.0 : 2 * (1.0 - hsl.getL() / v), v, hsl.getA());
}
CMYKExact hsl2cmyk(const HSLExact &hsl) {return rgb2cmyk(hsl2rgb(hsl));}
RGBExact cmyk2rgb(const CMYKExact &cmyk) {return RGBExact((1.0 - cmyk.getC()) * (1.0 - cmyk.getK()), (1.0 - cmyk.getM()) * (1.0 - cmyk.getK()), (1.0 - cmyk.getY()) * (1.0 - cmyk.getK()), cmyk.getA());}
HSVExact cmyk2hsv(const CMYKExact &cmyk) {return rgb2hsv(cmyk2rgb(cmyk));}
HSLExact cmyk2hsl(const CMYKExact &cmyk) {return rgb2hsl(cmyk2rgb(cmyk));}
