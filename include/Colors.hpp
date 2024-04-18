#ifndef COLORS
#define COLORS

#include <string>
#include <SDL2/SDL_pixels.h>

#include "Utilities.hpp"

class RGBRough {
    private:
        Uint8 R = 255;
        Uint8 G = 255;
        Uint8 B = 255;
        Uint8 A = 255;

    public:
        RGBRough(const Uint8 &r, const Uint8 &g, const Uint8 &b, const Uint8 &a) : R(r), G(g), B(b), A(a) {}
        RGBRough(const Uint8 &r, const Uint8 &g, const Uint8 &b) : R(r), G(g), B(b) {}
        RGBRough(const SDL_Color &color) : R(color.r), G(color.g), B(color.b), A(color.a) {}
        RGBRough(const RGBRough &color) : R(color.getR()), G(color.getG()), B(color.getB()), A(color.getA()) {}

        Uint8 getR() const {return R;}
        Uint8 getG() const {return G;}
        Uint8 getB() const {return B;}
        Uint8 getA() const {return A;}

        Uint8 setR(const Uint8 &r) {return btils::set<Uint8>(R, r);}
        Uint8 setG(const Uint8 &g) {return btils::set<Uint8>(G, g);}
        Uint8 setB(const Uint8 &b) {return btils::set<Uint8>(B, b);}
        Uint8 setA(const Uint8 &a) {return btils::set<Uint8>(A, a);}

        Uint8 adjR(const Uint8 &amount) {return btils::adj<Uint8>(R, amount);}
        Uint8 adjG(const Uint8 &amount) {return btils::adj<Uint8>(G, amount);}
        Uint8 adjB(const Uint8 &amount) {return btils::adj<Uint8>(B, amount);}
        Uint8 adjA(const Uint8 &amount) {return btils::adj<Uint8>(A, amount);}

        void operator=(const RGBRough &color) {
            R = color.getR();
            G = color.getG();
            B = color.getB();
            A = color.getA();
        }
        RGBRough    operator!()                                 const {return RGBRough(255 - R, 255 - G, 255 - B, A);}
        SDL_Color       toSDL()                                 const {return {R, G, B, A};}
        std::string  toString(const bool &includeAlpha = false) const {return std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + (includeAlpha ? ", " + std::to_string(A) : "");}

        bool equal(const RGBRough &color) const {
            return R == color.getR() && G == color.getG() && B == color.getB();
        }
};

class RGBExact {

};

#endif /* COLORS */
