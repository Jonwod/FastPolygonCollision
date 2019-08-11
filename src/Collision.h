//
// Created by jon on 02/08/2019.
//

#ifndef FASTCOLLISION_COLLISION_H
#define FASTCOLLISION_COLLISION_H

#include <limits>
#include "lib/Vec2Util.h"


template<typename V>
inline V perpendicularRight(const V& vec) {
    return V(vec.y, -vec.x);
}


template<typename V>
inline bool areColliding(int polyASize, const V* polyA, int polyBSize, const V* polyB) {
    for(int i = 0; i < polyASize; ++i) {
        struct Edge {
            const V& start;
            const V& end;
        } edge1{polyA[i], polyA[(i + 1) % polyASize]};

        const V edgeNormal = perpendicularRight(edge1.end - edge1.start);

        float a_min = std::numeric_limits<float>::infinity();
        float a_max = -std::numeric_limits<float>::infinity();
        for(int vert_i = 0; vert_i < polyASize; ++vert_i) {
            const V& vert = polyA[vert_i];
            const float vertProj = Vec2::dotProduct(edgeNormal, vert);
            if(vertProj < a_min) { a_min = vertProj; }
            if(vertProj > a_max) { a_max = vertProj; }
        }

        float b_min = std::numeric_limits<float>::infinity();
        float b_max = -std::numeric_limits<float>::infinity();
        for(int vert_i = 0; vert_i < polyBSize; ++vert_i) {
            const V& vert = polyB[vert_i];
            const float vertProj = Vec2::dotProduct(edgeNormal, vert);
            if(vertProj < b_min) { b_min = vertProj; }
            if(vertProj > b_max) { b_max = vertProj; }
        }

        if(a_max < b_min  ||  a_min > b_max) {
            return false;
        }
    }

    return true;
}

#endif //FASTCOLLISION_COLLISION_H
