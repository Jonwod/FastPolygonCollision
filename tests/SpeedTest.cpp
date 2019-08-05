//
// Created by jon on 26/07/2019.
//

#include "SpeedTest.h"

const sf::FloatRect spawnArea{0.f, 0.f, 800.f, 800.f};

const float maxPolyRadius = 20.f;
const float minPolyRadius = 1.f;


std::vector<sf::Vector2f> randTri() {
    std::vector<sf::Vector2f> tri;
    const sf::Vector2f vert1 {randFloat(0.f, triangleTestRange), randFloat(0.f, triangleTestRange)};
    tri.push_back(vert1);
    tri.push_back(vert1 + sf::Vector2f{randFloat(-maxTriLength, maxTriLength), randFloat(-maxTriLength, maxTriLength)});
    tri.push_back(vert1 + sf::Vector2f{randFloat(-maxTriLength, maxTriLength), randFloat(-maxTriLength, maxTriLength)});
    return tri;
}


std::vector<std::vector<sf::Vector2f>> generateTestPolys(int num) {
    const unsigned int minSides = 3;
    const unsigned int maxSides = 10;

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


std::vector<PolyEntity> generateRandomTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager, int num) {
    std::vector<std::vector<sf::Vector2f>> polys = generateTestPolys(num);
    std::vector<PolyEntity> entities;
    for(const auto& poly: polys){
        entities.push_back(PolyEntity(poly, collisionPolyManager));
    }
    return entities;
}


std::vector<PolyEntity> generateSpecificTestEntities(SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager) {
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


void polySpeedTest() {
    const unsigned int numPolys = 200;
    SlowCollisionPolyManager<sf::Vector2f> slowManager(numPolys);
    auto entities = generateRandomTestEntities(slowManager, numPolys);

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

        slowManager.runCollisionCheck();

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
