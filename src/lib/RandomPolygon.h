//
// Created by jon on 10/08/2019.
//

#ifndef FASTCOLLISION_RANDOMPOLYGON_H
#define FASTCOLLISION_RANDOMPOLYGON_H

#include <vector>
#include <algorithm>
#include "Vec2Util.h"
#include "Rand.h"


template<typename V>
bool isAngleOfAGreater(const V& a, const V& b) {
    return Vec2::angleRadians(a) > Vec2::angleRadians(b);
}


template<template<class> class V, class S>
std::vector<V<S>> randomConvexPoly(unsigned int numSides, const V<S>& origin, S minExtent, S maxExtent) {
    std::vector<S> randXCoords;
    std::vector<S> randYCoords;

    for(unsigned int i = 0; i < numSides; ++i) {
        randXCoords.push_back(origin.x + static_cast<S>(randFloat(minExtent, maxExtent)));
        randYCoords.push_back(origin.y + static_cast<S>(randFloat(minExtent, maxExtent)));
    }

    std::sort(randXCoords.begin(), randXCoords.end());
    std::sort(randYCoords.begin(), randYCoords.end());


    S minX = randXCoords.front();
    std::vector<S> xChain;
    {
        S maxX = randXCoords.back();

        S lastTop = minX;
        S lastBot = minX;

        for (unsigned int i = 1; i < numSides - 1; ++i) {
            S x = randXCoords[i];

            if (randBool()) {
                xChain.push_back(x - lastTop);
                lastTop = x;
            } else {
                xChain.push_back(lastBot - x);
                lastBot = x;
            }
        }

        xChain.push_back(maxX - lastTop);
        xChain.push_back(lastBot - maxX);
    }

    S minY = randYCoords.front();
    std::vector<S> yChain;
    {
        S maxY = randYCoords.back();

        S lastLeft = minY;
        S lastRight = maxY;

        for (unsigned int i = 1; i < numSides - 1; ++i) {
            S y = randYCoords[i];

            if (randBool()) {
                yChain.push_back(y - lastLeft);
                lastLeft = y;
            } else {
                yChain.push_back(lastRight - y);
                lastRight = y;
            }
        }

        yChain.push_back(maxY - lastLeft);
        yChain.push_back(lastRight - maxY);
    }


    // Randomly pair up into vectors
    std::random_shuffle(yChain.begin(), yChain.end());
    std::vector<V<S>> vec;
    for(unsigned int i = 0; i < yChain.size(); ++i) {
        vec.push_back(V<S>{xChain[i], yChain[i]});
    }

    // Sort by angle.
    std::sort(vec.begin(), vec.end(), isAngleOfAGreater<V<S>>);

    // Lay them end-to-end
    S x = 0;
    S y = 0;
    S minPolygonX = 0;
    S minPolygonY = 0;
    std::vector<V<S>> points{};

    for (unsigned int i = 0; i < numSides; ++i) {
        points.push_back(V<S>(x, y));

        x += vec[i].x;
        y += vec[i].y;

        minPolygonX = std::min(minPolygonX, x);
        minPolygonY = std::min(minPolygonY, y);
    }

    // Move the polygon to the original min and max coordinates
    const V<S> shift{minX - minPolygonX, minY - minPolygonY};
    for (unsigned int i = 0; i < numSides; ++i) {
        points[i] += shift;
    }

    return points;
}



#endif //FASTCOLLISION_RANDOMPOLYGON_H
