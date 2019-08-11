//
// Created by jon on 26/07/2019.
//

#include "SpeedTest.h"
#include <chrono>
#include <iostream>
#include "../src/lib/RandomPolygon.h"

const sf::FloatRect spawnArea{0.f, 0.f, 800.f, 800.f};

const float maxPolyRadius = 20.f;
const float minPolyRadius = 1.f;
const unsigned int minSides = 3;
const unsigned int maxSides = 10;


std::vector<sf::Vector2f> randTri() {
    std::vector<sf::Vector2f> tri;
    const sf::Vector2f vert1 {randFloat(0.f, triangleTestRange), randFloat(0.f, triangleTestRange)};
    tri.push_back(vert1);
    tri.push_back(vert1 + sf::Vector2f{randFloat(-maxTriLength, maxTriLength), randFloat(-maxTriLength, maxTriLength)});
    tri.push_back(vert1 + sf::Vector2f{randFloat(-maxTriLength, maxTriLength), randFloat(-maxTriLength, maxTriLength)});
    return tri;
}


std::vector<std::vector<sf::Vector2f>> generateTestPolys(int num) {
    std::vector<std::vector<sf::Vector2f>> testPolys;
    // Just use triangles for now
    for(int i = 0; i < num; ++i) {
        const unsigned int sides = randInt(minSides, maxSides);
        const sf::Vector2f origin{randFloat(spawnArea.left, spawnArea.left + spawnArea.width),
                                  randFloat(spawnArea.top, spawnArea.top + spawnArea.height)};
        testPolys.push_back(randomConvexPoly( sides, origin, minPolyRadius, maxPolyRadius));
    }

    return testPolys;
}


std::vector<PolyEntity_Slow> generateRandomTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager, int num) {
    std::vector<std::vector<sf::Vector2f>> polys = generateTestPolys(num);
    std::vector<PolyEntity_Slow> entities;
    for(const auto& poly: polys){
        entities.push_back(PolyEntity_Slow(poly, collisionPolyManager));
    }
    return entities;
}


std::vector<PolyEntity_Slow> generateSpecificTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager) {
    std::vector<std::vector<sf::Vector2f>> polys;
    polys.push_back({ {0, 0}, {200, 0}, {100, 100} });
    polys.push_back({ {100, 50}, {100, 300}, {300, 300} });
    polys.push_back({ {500, 200}, {500, 400}, {600, 600} });

    std::vector<PolyEntity_Slow> entities;
    for(const auto& poly: polys){
        entities.push_back(PolyEntity_Slow(poly, collisionPolyManager));
    }
    return entities;
}



using namespace std::chrono;


int contiguousSpeedTest(const std::vector<std::vector<sf::Vector2f>> & polys) {
    CollisionPolyManager manager(CollisionPolyReference::requiredBytes(maxSides) * polys.size());
    for(const auto & poly: polys) {
       if(!manager.add(CollisionData{}, poly).isValid()) {
           std::cout<<"Error. Failed to add poly to contiguous manager"<< std::endl;
       } // Just making sure the add worked
    }

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    manager.runCollisionCheck();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Contiguous polygon collision check for "<< polys.size() <<" polygons took " << time_span.count() << " seconds.";
    std::cout << std::endl;

    int count = 0;
    for(auto poly = manager.first(); poly.isValid(); poly = manager.next(poly)) {
        count += poly.getMetaData().isColliding ? 1 : 0;
    }
    return count;
}


int noncontiguousSpeedTest(const std::vector<std::vector<sf::Vector2f>>& polys) {

    SlowCollisionPolyManager<sf::Vector2f> slowManager(polys.size());
    for(const auto& poly: polys) {
        if(!slowManager.add(poly)) {
            std::cout<<"Error. Failed to add poly to slow manager"<< std::endl;
        } // Just making sure the add worked
    }

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    slowManager.runCollisionCheck();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Noncontiguous polygon collision check for "<< polys.size() <<" polygons took " << time_span.count() << " seconds.";
    std::cout << std::endl;

    int count = 0;
    for(const auto & poly: slowManager.getPolys()) {
        count += poly.isColliding() ? 1 : 0;
    }
    return count;
}


void polySpeedTest() {
    const unsigned int numPolys = 10000;
    auto testPolys = generateTestPolys(numPolys);

    const int nCollisions = noncontiguousSpeedTest(testPolys);

    const int cCollisions = contiguousSpeedTest(testPolys);
}

