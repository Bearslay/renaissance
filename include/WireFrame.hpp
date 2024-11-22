#ifndef WIREFRAME
#define WIREFRAME

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

#include "Coord3D.hpp"
#include "Vector3D.hpp"
#include "RenderWindow.hpp"
#include "PresetColors.hpp"
#include "astr.hpp"

template <typename ArithType> class WireFrame {
    private:
        Coord3D<ArithType> Origin = Coord3D<ArithType>(0, 0, 0);
        std::vector<std::vector<Coord3D<ArithType>>> Points;
        std::vector<std::pair<unsigned int, unsigned int>> Pairs;

        double LastFocalLength = 0;
        double LastAngleX = 0;
        double LastAngleY = 0;
        double LastAngleZ = 0;

    public:
        WireFrame(Coord3D<ArithType> origin, std::vector<Coord3D<ArithType>> points, std::vector<std::pair<unsigned int, unsigned int>> pairs) {
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
        WireFrame(Coord3D<ArithType> origin, std::vector<Coord3D<ArithType>> points) {WireFrame(origin, points, {});}
        WireFrame(Coord3D<ArithType> origin) {WireFrame(origin, {}, {});}
        WireFrame(const WireFrame &WireFrame) {
            Origin = WireFrame.Origin;
            Points = WireFrame.Points;
            Pairs = WireFrame.Pairs;
            
            LastFocalLength = WireFrame.LastFocalLength;
            LastAngleX = WireFrame.LastAngleX;
            LastAngleY = WireFrame.LastAngleY;
            LastAngleZ = WireFrame.LastAngleZ;
        }
        WireFrame() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}

        Coord3D<ArithType> getBasePoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord3D<ArithType>(0, 0, 0);}
            return Points[point][0];
        }
        Coord3D<ArithType> getRotatedPoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord3D<ArithType>(0, 0, 0);}
            return Points[point][1];
        }
        Coord3D<ArithType> getProjectedPoint(unsigned int point) {
            if (point < 0 || point >= Points.size()) {return Coord3D<ArithType>(0, 0, 0);}
            return Points[point][2];
        }
        Coord3D<ArithType> getOrigin() {return Origin;}
        ArithType getPosX() {return Origin.getX();}
        ArithType getPosY() {return Origin.getY();}
        ArithType getPosZ() {return Origin.getZ();}

        Coord3D<ArithType> teleport(const Coord3D<ArithType> &point) {
            Coord3D<ArithType> output = Origin;
            Origin = point;
            return output;
        }
        ArithType moveX(ArithType amount) {
            ArithType output = Origin.getX();
            Origin += Coord3D<ArithType>(amount, 0, 0);
            return output;
        }
        ArithType moveY(ArithType amount) {
            ArithType output = Origin.getY();
            Origin += Coord3D<ArithType>(0, amount, 0);
            return output;
        }
        ArithType moveZ(ArithType amount) {
            ArithType output = Origin.getZ();
            Origin -= Coord3D<ArithType>(0, 0, amount);
            return output;
        }

        void rotateOriginAny(const Vector3D<ArithType> &rotationAxis, double theta, bool degrees = true) {
            if (degrees) {theta *= M_PI / 180;}

            const Vector3D<ArithType> axis = rotationAxis.unit();
            const ArithType x = axis.getX();
            const ArithType y = axis.getY();
            const ArithType z = axis.getZ();

            LastAngleX = x;
            LastAngleY = y;
            LastAngleZ = z;

            const double sine = std::sin(theta);
            const double cosine = std::cos(theta);
            const std::vector<std::vector<double>> R = {
                {cosine + x * x * (1 - cosine), x * y * (1 - cosine) - z * sine, x * z * (1 - cosine) + y * sine},
                {y * x * (1 - cosine) + z * sine, cosine + y * y * (1 - cosine), y * z * (1 - cosine) - x * sine},
                {z * x * (1 - cosine) - y * sine, z * y * (1 - cosine) + x * sine, cosine + z * z * (1 - cosine)}
            };

            for (unsigned int i = 0; i < Points.size(); i++) {
                const std::vector<ArithType> original = Points[i][1].toVector();
                std::vector<double> rotated = {0, 0, 0};
                for (unsigned char j = 0; j < 3; j++) {
                    for (unsigned char k = 0; k < 3; k++) {
                        rotated[j] += original[k] * R[j][k];
                    }
                }

                if (std::is_floating_point<ArithType>::value) {Points[i][1] = Coord3D<ArithType>(rotated[0], rotated[1], rotated[2]);}
                else {Points[i][1] = Coord3D<ArithType>(std::round(rotated[0]), std::round(rotated[1]), std::round(rotated[2]));}
            }
        }

        void project(const Coord3D<ArithType> &cameraPos) {
            const double focalLength = cameraPos.getX();

            for (unsigned int i = 0; i < Points.size(); i++) {
                const Coord3D<ArithType> point = Origin + Points[i][1] - Coord3D<ArithType>(0, cameraPos.getY(), cameraPos.getZ());

                if (focalLength + point.getX() == 0) {continue;}

                Points[i][2].setY((focalLength * point.getY()) / (focalLength + point.getX()));
                Points[i][2].setZ((focalLength * point.getZ()) / (focalLength + point.getX()));
            }
        }

        void display(RenderWindow &window, const SDL_Color &color = PresetColors[COLOR_WHITE]) {
            for (unsigned int i = 0; i < Pairs.size(); i++) {
                window.drawLine(Origin.getY() + Points[Pairs[i].first][2].getY(), Origin.getZ() + Points[Pairs[i].first][2].getZ(), Origin.getY() + Points[Pairs[i].second][2].getY(), Origin.getZ() + Points[Pairs[i].second][2].getZ(), color);
            }
        }
};

#endif /* WIREFRAME */
