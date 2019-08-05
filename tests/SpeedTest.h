//
// Created by jon on 26/07/2019.
//

#ifndef FASTCOLLISION_SPEEDTEST_H
#define FASTCOLLISION_SPEEDTEST_H

#include <algorithm>
#include "../src/SlowCollisionPoly.h"
#include "SFML/System/Vector2.hpp"
#include "../src/lib/Rand.h"
#include <SFML/Graphics.hpp>
#include "../src/PolyEntity.h"
#include "../src/lib/JMath.h"


const float triangleTestRange{1000.f};
const float maxTriLength{100.f};


std::vector<sf::Vector2f> randTri();

constexpr float pi = 3.14159265358979323846264338327950288;


template<template<class> class V, class S>
V<S> randPointInRadius(const V<S>& origin, S minRadius, S maxRadius) {
    const V<S> randomDirection = Vec2::vectorFromAngle<V<S>>(randFloat(0, 2 * pi));
    return origin + randomDirection * randFloat(minRadius, maxRadius);
}


// Takes 2 points in a polygon (as indices into points) and initializes the points in between
// them, while preserving convexity.
//template<template<class> class V, class S>
//void expandPolyLine(std::vector<V<S>> & points, unsigned int start, unsigned int end, const V<S>& pointBeforeStart, const V<S>& pointAfterEnd) {
//    if (start > end) {
//        end = points.size() + end;
//    }
//
//    if(end - start > 1) {
//        const unsigned int midpoint = (start + end) / 2;
//        const V<S> alpha = lerp(points[start], points[end], randFloat(0.01f, 0.99f));
//        // Initialize midpoint
//        {
//            // The valid area will be a triangle created by the extension of
//            points[midpoint] =;
//        }
//
//        if (end - start == 2) {
//            // initialize the point in between
//        } else if (end - start > 2) {
//            expandPolyLine();
//        }
//    }
//}


// Makes a random convex polygon with vertices with a distance between minExtent
// and maxExtent from origin.
//template<template<class> class V, class S>
//std::vector<V<S>> randomConvexPoly(unsigned int numSides, const V<S>& origin, S minExtent, S maxExtent) {
//    if(numSides < 3) return std::vector<V<S>>{};
//
//    std::vector<V<S>> poly(numSides, V<S>{0, 0});
//
//    const double spacing = double(numSides) / double(3);
//    const unsigned int tri1 = 0;
//    const unsigned int tri2 = int(spacing);
//    const unsigned int tri3 = int(spacing * 2);
//    poly[tri1] = randPointInRadius(origin, minExtent, maxExtent);
//    poly[tri2] = randPointInRadius(origin, minExtent, maxExtent);
//    poly[tri3] = randPointInRadius(origin, minExtent, maxExtent);
//
//    expandPolyLine(poly, tri1, tri2, poly[tri3], poly[tri3]);
//    expandPolyLine(poly, tri2, tri3, poly[tri1], poly[tri1]);
//    expandPolyLine(poly, tri3, tri1, poly[tri2], poly[tri2]);
//}

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


std::vector<std::vector<sf::Vector2f>> generateTestPolys();


std::vector<PolyEntity> generateTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager);


void polySpeedTest();

#endif //FASTCOLLISION_SPEEDTEST_H
