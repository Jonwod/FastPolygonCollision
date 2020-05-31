//
// Created by jon on 25/07/2019.
//

#include "TestCollisionPoly.h"

#include <initializer_list>
#include <vector>
#include <iostream>
#include <cassert>
#include <string>
#include "../src/CollisionPoly.h"
#include "SFML/System/Vector2.hpp"
#include "../src/lib/Vec2Util/src/Vec2Util.h"
#include "../src/ContiguousDataManager.h"
#include "SFML/Graphics.hpp"
#include "../src/PolyEntity.h"
#include "../src/lib/Rand.h"
#include "../src/lib/RandomPolygon.h"


void testFastCollisionPoly();


void testCollisionPoly(){
    testFastCollisionPoly();
}


void testFastCollisionPoly() {
    std::cout<<"Testing Collision Poly system."<<std::endl;

    std::vector<std::vector<sf::Vector2f>> testPolygons;

    testPolygons.push_back( { {1.f, 1.f}, {2.f, 2.f}, {3.f, 3.f} } );

    testPolygons.push_back( { {-4.f, 10.f}, {-5.f, 100.f}, {-6.f, 1000.f}, {-7.f, 10000.f}} );

    testPolygons.emplace_back();
    for(int i = 0; i < 1000; ++i){
        testPolygons.back().emplace_back(sf::Vector2f(i, i * i));
    }

    std::size_t requiredBytes = 0;
    std::size_t numPolys = 0;
    std::size_t numVerts = 0;
    for(const auto& vec: testPolygons){
        requiredBytes += CollisionPolyReference::requiredBytes(vec.size());
        ++numPolys;
        numVerts += vec.size();
    }

    std::cout<<"Required "<<requiredBytes<<" bytes to store "<<numPolys<<" polygons with a total of "<<numVerts<<" vertices."<<std::endl;

    CollisionPolyManager manager(requiredBytes);

    std::vector<CollisionPolyReference> managedPolyReferences;
    for(const auto & poly: testPolygons){
        CollisionPolyReference newPoly = manager.add( CollisionData(), poly);
        assert( newPoly.isValid() );    // We should have allocated enough space to hold all of the test polys so this should be valid
        managedPolyReferences.push_back( newPoly );
    }

    auto itrRef = manager.first();
    for(std::size_t i = 0; i < testPolygons.size(); ++i){
        std::cout<<"_____________Polygon "<<i<<"_____________\n";
        for(std::size_t j = 0; j < testPolygons[i].size(); ++j){
            sf::Vector2f srcPolyPoint = testPolygons[i][j];
            sf::Vector2f managedPolyPoint = managedPolyReferences[i].getDatum(j);
            sf::Vector2f itrPoint = itrRef.getDatum(j);
            std::cout << "    Point " << j << ":   src: " << Vec2::toString(srcPolyPoint)  << "   managed: " << Vec2::toString(managedPolyPoint)
            <<" Managed by itr: "<<Vec2::toString(itrPoint)<<"\n";
            assert(srcPolyPoint == managedPolyPoint);
            assert(srcPolyPoint == itrPoint);
        }

        itrRef = manager.next(itrRef);
    }

    auto failedPoly = manager.add( CollisionData(), {{1.f, 2.f}, {10.f, 5432.f}, {2.f, 4.f}});
    assert( ( !failedPoly.isValid() ) );

    std::cout<<"All CollisionPoly tests passed!"<<std::endl;
}


std::vector<PolyEntity> generateSpecificTestEntities_Fast(CollisionPolyManager & collisionPolyManager) {
    std::vector<std::vector<sf::Vector2f>> polys;
    polys.push_back({ {0, 0}, {200, 0}, {100, 100} });
    polys.push_back({ {100, 50}, {100, 300}, {300, 300} });
    polys.push_back({ {500, 200}, {500, 400}, {600, 600} });

    std::vector<PolyEntity> entities;
    for(const auto& poly: polys){
        entities.push_back(PolyEntity(poly, collisionPolyManager));
    }
    return entities;
}


const sf::FloatRect spawnArea{0.f, 0.f, 800.f, 800.f};
const float maxPolyRadius = 20.f;
const float minPolyRadius = 1.f;
const unsigned int minSides = 3;
const unsigned int maxSides = 10;


std::vector<std::vector<sf::Vector2f>> generateRandomTestPolys(unsigned int num) {
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


std::vector<PolyEntity> makeTestEntities(CollisionPolyManager & collisionPolyManager, const std::vector<std::vector<sf::Vector2f>>& polys) {
    std::vector<PolyEntity> entities;
    for(const auto& poly: polys){
        entities.emplace_back(PolyEntity(poly, collisionPolyManager));
    }
    return entities;
}


void fastPolyVisualTest() {
    const unsigned int numPolys = 10000;

    CollisionPolyManager collisionManager(CollisionPolyReference::requiredBytes(maxSides) * numPolys);
    auto entities = makeTestEntities(collisionManager, generateRandomTestPolys(numPolys));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        collisionManager.runCollisionCheck();

        for(auto& ent: entities) {
            ent.update();
        }

        window.clear();

        for(const auto & ent: entities) {
            ent.draw(window);
        }

        window.display();
    }
}
