#ifndef RAY2D
#define RAY2D

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "MathCoord.hpp"
#include "MathVector.hpp"
#include "LineSeg2D.hpp"

template <typename ArithType> class Ray2D {
    private:
        Coord2D<ArithType> Position = Coord2D<ArithType>(0, 0);
        Vector2D<ArithType> View = Vector2D<ArithType>(0, 0);

    public:
        Ray2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        Ray2D(Coord2D<ArithType> position, Vector2D<ArithType> view) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

            Position = position;
            View = view;
        }
        Ray2D(Coord2D<ArithType> position) {Ray2D(position, Vector2D<ArithType>(0, 0));}
        Ray2D(Vector2D<ArithType> view) {Ray2D(Coord2D<ArithType>(0, 0), view);}

        Coord2D<ArithType> getPos() const {return Position;}
        ArithType getPosX() const {return Position.getX();}
        ArithType getPosY() const {return Position.getY();}

        Vector2D<ArithType> getView() const {return View;}
        ArithType getViewX() const {return View.getX();}
        ArithType getViewY() const {return View.getY();}
        ArithType getViewMag() const {return View.getMag();}
        double getViewAngle(bool degrees = true) const {return View.getAngle(degrees);}

        Coord2D<ArithType> setPos(Coord2D<ArithType> position) {
            Coord2D<ArithType> output = Position;
            Position = position;
            return output;
        }
        ArithType setX(ArithType x) {
            ArithType output = Position.getX();
            Position.setX(x);
            return output;
        }
        ArithType setY(ArithType y) {
            ArithType output = Position.getY();
            Position.setY(y);
            return output;
        }

        ArithType moveX(ArithType amount = 1) {
            ArithType output = Position.getX();
            Position.setX(output + amount);
            return output;
        }
        ArithType moveRight(ArithType amount = 1) {return moveX(amount);}
        ArithType moveLeft(ArithType amount = 1) {return moveX(-amount);}
        ArithType moveY(ArithType amount = 1) {
            ArithType output = Position.getY();
            Position.setY(output + amount);
            return output;
        }
        ArithType moveUp(ArithType amount = 1) {return moveY(amount);}
        ArithType moveDown(ArithType amount = 1) {return moveY(-amount);}

        Vector2D<ArithType> setView(Vector2D<ArithType> view) {
            Vector2D<ArithType> output = View;
            View = view;
            return output;
        }
        ArithType setViewMag(ArithType magnitude) {
            ArithType output = View.getMag();
            View.setMag(magnitude);
            return output;
        }
        double setViewAngle(double angle, bool degrees = true) {
            double output = View.getAngle(degrees);
            View.setAngle(angle, degrees);
            return output;
        }
        ArithType setViewX(ArithType x) {
            ArithType output = View.getX();
            View.setX(x);
            return output;
        }
        ArithType setViewY(ArithType y) {
            ArithType output = View.getY();
            View.setY(y);
            return output;
        }

        ArithType changeZoom(ArithType amount = 1) {
            ArithType output = View.getMag();
            View.setMag(output + amount);
            return output;
        }
        ArithType zoomOut(ArithType amount = 1) {return changeZoom(amount);}
        ArithType zoomIn(ArithType amount = 1) {return changeZoom(-amount);}
        double moveView(double amount = 1, bool degrees = true) {
            double output = View.getAngle(degrees);
            View.setAngle(output + amount, degrees);
            return output;
        }
        double lookCCW(double amount = 1, bool degrees = true) {return moveView(amount, degrees);}
        double lookCW(double amount = 1, bool degrees = true) {return moveView(-amount, degrees);}

        Coord2D<ArithType> getIntersection(const LineSeg2D<ArithType> &line) {
            ArithType x1 = Position.getX();
            ArithType y1 = Position.getY();
            ArithType x2 = Position.getX() + View.getX();
            ArithType y2 = Position.getY() + View.getY();
            ArithType x3 = line.getPos1X();
            ArithType y3 = line.getPos1Y();
            ArithType x4 = line.getPos2X();
            ArithType y4 = line.getPos2Y();

            ArithType denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (denominator == 0) {return Coord2D<ArithType>(std::sqrt(-1), std::sqrt(-1));}

            ArithType t = 0, tNum = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
            ArithType u = 0, uNum = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3));
            if (std::is_integral<ArithType>::value) {
                t = std::round(tNum / denominator);
                u = std::round(uNum / denominator);
            } else {
                t = tNum / denominator;
                u = uNum / denominator;
            }

            if (t < 0 || t > 1 || u < 0 || u > 1) {return Coord2D<ArithType>(std::sqrt(-1), std::sqrt(-1));}
            return Coord2D<ArithType>(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
        }
        Coord2D<ArithType> getIntersection(const std::vector<LineSeg2D<ArithType>> &lines) {
            if (lines.size() < 1) {return Coord2D<ArithType>(std::sqrt(-1), std::sqrt(-1));}
            
            Coord2D<ArithType> output = Coord2D<ArithType>(std::sqrt(-1), std::sqrt(-1));
            Coord2D<ArithType> current;

            unsigned long index = 0;
            for (unsigned long i = 0; i < lines.size(); i++) {
                if ((current = getIntersection(lines[i])).valid()) {
                    output = current;
                    break;
                }
                index++;
            }
            double minDistance = std::sqrt(std::pow(output.getX() - Position.getX(), 2) + std::pow(output.getY() - Position.getY(), 2));
            double currentDistance = 0;

            for (unsigned long i = index; i < lines.size(); i++) {
                if ((current = getIntersection(lines[i])).valid()) {
                    if ((currentDistance = std::sqrt(std::pow(current.getX() - Position.getX(), 2) + std::pow(current.getY() - Position.getY(), 2))) < minDistance) {
                        minDistance = currentDistance;
                        output = Coord2D<ArithType>(current.getX(), current.getY());
                    }
                }
            }
            return output;
        }
};

#endif /* RAY2D */
