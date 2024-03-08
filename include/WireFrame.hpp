#ifndef WIREFRAME
#define WIREFRAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "RenderWindow.hpp"
#include "General.hpp"
#include "astr.hpp"

template <typename ArithType> class WireFrame {
    private:
        Coord_3D<ArithType> Origin = Coord_3D<ArithType>(0, 0, 0);
        std::vector<std::vector<Coord_3D<ArithType>>> Points;
        std::vector<std::pair<unsigned int, unsigned int>> Pairs;

        double LastFocalLength = 0;
        double LastAngleX = 0;
        double LastAngleY = 0;
        double LastAngleZ = 0;

    public:
        WireFrame(Coord_3D<ArithType> origin, std::vector<Coord_3D<ArithType>> points, std::vector<std::pair<unsigned int, unsigned int>> pairs) {
            static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");

            Origin = origin;
            
            for (unsigned int i = 0; i < points.size(); i++) {
                Points.emplace_back();
                
                // Add the user-inputted coordinate to the list and make space for a rotated and projected coordinate as well
                Points[i].emplace_back(points[i]);
                Points[i].emplace_back(points[i]);
                Points[i].emplace_back();
            }
            for (unsigned int i = 0; i < pairs.size(); i++) {
                // Check to see that all of the pairs actually reference points that exist
                if (pairs[i].first < 0 || pairs[i].first >= points.size()) {pairs[i].first = 0;}
                if (pairs[i].second < 0 || pairs[i].second >= points.size()) {pairs[i].second = 0;}

                Pairs.emplace_back(pairs[i]);
            }
        }
        WireFrame(Coord_3D<ArithType> origin, std::vector<Coord_3D<ArithType>> points) {WireFrame(origin, points, {});}
        WireFrame(Coord_3D<ArithType> origin) {WireFrame(origin, {}, {});}
        WireFrame(const WireFrame &wireframe) {
            Origin = wireframe.Origin;
            Points = wireframe.Points;
            Pairs = wireframe.Pairs;
            
            LastFocalLength = wireframe.LastFocalLength;
            LastAngleX = wireframe.LastAngleX;
            LastAngleY = wireframe.LastAngleY;
            LastAngleZ = wireframe.LastAngleZ;
        }
        WireFrame() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}

        Coord_3D<ArithType> getBasePoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord_3D<ArithType>(0, 0, 0);}
            return Points[point][0];
        }
        Coord_3D<ArithType> getRotatedPoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord_3D<ArithType>(0, 0, 0);}
            return Points[point][1];
        }
        Coord_3D<ArithType> getProjectedPoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord_3D<ArithType>(0, 0, 0);}
            return Points[point][2];
        }
        Coord_3D<ArithType> getOrigin() {return Origin;}
        ArithType getPosX() {return Origin.getX();}
        ArithType getPosY() {return Origin.getY();}
        ArithType getPosZ() {return Origin.getZ();}

        Coord_3D<ArithType> teleport(Coord_3D<ArithType> point) {
            Coord_3D<ArithType> output = Origin;
            Origin = point;
            return output;
        }
        ArithType moveX(ArithType amount) {
            ArithType output = Origin.getX();
            Origin += Coord_3D<ArithType>(amount, 0, 0);
            return output;
        }
        ArithType moveY(ArithType amount) {
            ArithType output = Origin.getY();
            Origin += Coord_3D<ArithType>(0, amount, 0);
            return output;
        }
        ArithType moveZ(ArithType amount) {
            ArithType output = Origin.getZ();
            Origin -= Coord_3D<ArithType>(0, 0, amount);
            return output;
        }

        void rotateAny(const Vector_3D<ArithType> &rotationAxis, double theta, bool degrees = true) {
            if (degrees) {theta *= M_PI / 180;}

            const Vector_3D<ArithType> axis = rotationAxis.rToUnitVector();
            const ArithType x = axis.getX();
            const ArithType y = axis.getY();
            const ArithType z = axis.getZ();

            LastAngleX = x;
            LastAngleY = y;
            LastAngleZ = z;

            const std::vector<std::vector<double>> R = {
                {std::cos(theta) + std::pow(x, 2) * (1 - std::cos(theta)), x * y * (1 - std::cos(theta)) - z * std::sin(theta), x * z * (1 - std::cos(theta)) + y * std::sin(theta)},
                {y * x * (1 - std::cos(theta)) + z * std::sin(theta), std::cos(theta) + std::pow(y, 2) * (1 - std::cos(theta)), y * z * (1 - std::cos(theta)) - x * std::sin(theta)},
                {z * x * (1 - std::cos(theta)) - y * std::sin(theta), z * y * (1 - std::cos(theta)) + x * std::sin(theta), std::cos(theta) + std::pow(z, 2) * (1 - std::cos(theta))}
            };

            for (unsigned int i = 0; i < Points.size(); i++) {
                const std::vector<ArithType> original = Points[i][1].toVector();
                std::vector<double> rotated = {0, 0, 0};
                for (unsigned char j = 0; j < 3; j++) {
                    for (unsigned char k = 0; k < 3; k++) {
                        rotated[j] += original[k] * R[j][k];
                    }
                }

                if (std::is_floating_point<ArithType>::value) {Points[i][1] = Coord_3D<ArithType>(rotated[0], rotated[1], rotated[2]);}
                else {Points[i][1] = Coord_3D<ArithType>(std::round(rotated[0]), std::round(rotated[1]), std::round(rotated[2]));}
            }
        }

        void project(const Coord_3D<ArithType> &cameraPos) {
            const double focalLength = cameraPos.getX();

            for (unsigned int i = 0; i < Points.size(); i++) {
                const Coord_3D<ArithType> point = Origin + Points[i][1] - Coord_3D<ArithType>(0, cameraPos.getY(), cameraPos.getZ());

                if (focalLength + point.getX() == 0) {continue;}

                Points[i][2].setY((focalLength * point.getY()) / (focalLength + point.getX()));
                Points[i][2].setZ((focalLength * point.getZ()) / (focalLength + point.getX()));
            }
        }

        void display(RenderWindow &window, Color color = DefaultColors[COLOR_WHITE]) {
            for (unsigned int i = 0; i < Pairs.size(); i++) {
                window.drawLine(Origin.getY() + Points[Pairs[i].first][2].getY(), Origin.getZ() + Points[Pairs[i].first][2].getZ(), Origin.getY() + Points[Pairs[i].second][2].getY(), Origin.getZ() + Points[Pairs[i].second][2].getZ(), color);
            }
        }
};

#endif /* WIREFRAME */
