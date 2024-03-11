#ifndef ENTITYTOPDOWN
#define ENTITYTOPDOWN

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "MathCoord.hpp"
#include "MathVector.hpp"

template <typename ArithType> class EntityTopDown {
    private:
        Coord2D<ArithType> Position(0, 0);
        Vector2D<ArithType> View(0, 0);

        Coord2D<ArithType> MousePos(0, 0);

    public:
        EntityTopDown() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        EntityTopDown(Coord2D<ArithType> position, Vector2D<ArithType> view) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

            Position = position;
            View = view;
        }
        EntityTopDown(Coord2D<ArithType> position) {EntityTopDown(position, Vector2D<ArithType>(0, 0));}
        EntityTopDown(Vector2D<ArithType> view) {EntityTopDown(Coord2D<ArithType>(0, 0), view);}

        Coord2D<ArithType> getPos() {return Position;}
        ArithType getPosX() {return Position.getX();}
        ArithType getPosY() {return Position.getY();}

        Vector2D<ArithType> getView() {return View;}
        ArithType getViewX() {return View.getX();}
        ArithType getViewY() {return View.getY();}
        ArithType getViewMag() {return View.getMag();}
        double getViewAngle() {return View.getAngle();}

        Coord2D<ArithType> setPos(Coord2D<ArithType> position) {
            Coord2D<ArithType> output = Position;
            Position = position;
            return output;
        }
};

#endif /* ENTITYTOPDOWN */
