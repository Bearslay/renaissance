#ifndef TEXTURE
#define TEXTURE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture {
    private:
        SDL_Texture* Source = NULL;
        SDL_Point Center;
        SDL_Rect Frame;
        double Angle = 0;
        SDL_RendererFlip Flip = SDL_FLIP_NONE;

        SDL_BlendMode BlendMode = SDL_BLENDMODE_NONE;
        SDL_Color Mods = {255, 255, 255, SDL_ALPHA_OPAQUE};

    public:
        Texture() : Source(NULL), Center({}), Frame({}) {}
        Texture(SDL_Texture* texture) : Source(texture), Center({}), Frame({}) {}
        Texture(SDL_Texture* texture, SDL_Point center, SDL_Rect frame) : Source(texture), Center(center), Frame(frame) {}
        Texture(SDL_Texture* texture, SDL_Rect frame) : Source(texture), Center({frame.w / 2, frame.h / 2}), Frame(frame) {}

        SDL_Texture* getTexture() const {return Source;}
        SDL_Point getCenter() const {return Center;}
        SDL_Rect getFrame() const {return Frame;}
        double getAngle() const {return Angle;}
        SDL_RendererFlip getFlip() const {return Flip;}
        
        SDL_BlendMode getBlending() const {return BlendMode;}
        SDL_Color getColorMod() const {return Mods;}
        Uint8 getOpacity() const {return Mods.a;}
        Uint8 getRedMod() const {return Mods.r;}
        Uint8 getGreenMod() const {return Mods.g;}
        Uint8 getBlueMod() const {return Mods.b;}

        SDL_Texture* setTexture(SDL_Texture* texture) {
            SDL_Texture* output = Source;
            Source = texture;
            SDL_SetTextureBlendMode(Source, BlendMode);
            SDL_SetTextureAlphaMod(Source, Mods.a);
            SDL_SetTextureColorMod(Source, Mods.r, Mods.g, Mods.b);
            return output;
        }
        SDL_Point setCenter(SDL_Point center) {
            SDL_Point output = Center;
            Center = center;
            return output;
        }
        SDL_Rect setFrame(SDL_Rect frame) {
            SDL_Rect output = Frame;
            Frame = frame;
            return output;
        }
        double setAngle(double angle) {
            double output = Angle;
            Angle = angle;
            return output;
        }
        SDL_RendererFlip setFlip(SDL_RendererFlip flip) {
            SDL_RendererFlip output = Flip;
            Flip = flip;
            return output;
        }

        SDL_BlendMode setBlending(SDL_BlendMode blendMode) {
            SDL_BlendMode output = BlendMode;
            BlendMode = blendMode;
            SDL_SetTextureBlendMode(Source, BlendMode);
            return output;
        }
        SDL_Color setMods(SDL_Color mods) {
            SDL_Color output = Mods;
            Mods = mods;
            SDL_SetTextureBlendMode(Source, BlendMode);
            SDL_SetTextureAlphaMod(Source, Mods.a);
            SDL_SetTextureColorMod(Source, Mods.r, Mods.g, Mods.b);
            return output;
        }
        Uint8 setOpacity(Uint8 opacity) {
            Uint8 output = Mods.a;
            Mods.a = opacity;
            SDL_SetTextureAlphaMod(Source, Mods.a);
            return output;
        }
        Uint8 setRedMod(Uint8 redMod) {
            Uint8 output = Mods.r;
            Mods.r = redMod;
            SDL_SetTextureColorMod(Source, Mods.r, Mods.g, Mods.b);
            return output;
        }
        Uint8 setGreenMod(Uint8 greenMod) {
            Uint8 output = Mods.g;
            Mods.g = greenMod;
            SDL_SetTextureColorMod(Source, Mods.r, Mods.g, Mods.b);
            return output;
        }
        Uint8 setBlueMod(Uint8 blueMod) {
            Uint8 output = Mods.b;
            Mods.b = blueMod;
            SDL_SetTextureColorMod(Source, Mods.r, Mods.g, Mods.b);
            return output;
        }
        
};

#endif /* TEXTURE */
