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
#include "../src/VectorUtils.h"
#include "../src/ContiguousDataManager.h"

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

    for(std::size_t i = 0; i < testPolygons.size(); ++i){
        std::cout<<"_____________Polygon "<<i<<"_____________\n";
        for(std::size_t j = 0; j < testPolygons[i].size(); ++j){
            sf::Vector2f srcPolyPoint = testPolygons[i][j];
            sf::Vector2f managedPolyPoint = managedPolyReferences[i].getDatum(j);
            std::cout << "    Point " << j << ":   src: " << Vec2::toStr(srcPolyPoint) + "   managed: " << Vec2::toStr(managedPolyPoint)<<"\n";
            assert(srcPolyPoint == managedPolyPoint);
        }
    }

    auto failedPoly = manager.add( CollisionData(), {{1.f, 2.f}, {10.f, 5432.f}, {2.f, 4.f}});
    assert( ( !failedPoly.isValid() ) );

    std::cout<<"All CollisionPoly tests passed!"<<std::endl;
}