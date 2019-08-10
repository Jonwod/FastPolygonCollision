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


std::vector<std::vector<sf::Vector2f>> generateTestPolys();


std::vector<PolyEntity_Slow> generateTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager);


void polySpeedTest();

#endif //FASTCOLLISION_SPEEDTEST_H
