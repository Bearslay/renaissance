#ifndef LINE2D
#define LINE2D

#include <cmath>
#include <string>

#include "Coord2D.hpp"

#ifndef LINE2D_RELATIONS
#define LINE2D_RELATIONS
#define LINE2D_RELATE_COMMON 0
#define LINE2D_RELATE_SLOPE 1
#define LINE2D_RELATE_XINT 2
#define LINE2D_RELATE_YINT 3
#endif /* LINE2D_RELATIONS */

template <typename ArithType> class Line2D {
    private:
        ArithType X = 1;
        ArithType Y = -1;
        ArithType C = 0;
        unsigned char RelationMetric = LINE2D_RELATE_COMMON;

    public:
        Line2D() {static_assert(std::is_arithmetic<ArithType>::value, "ArithType must be an arithmetic type");}
        
};

#endif /* LINE2D */
