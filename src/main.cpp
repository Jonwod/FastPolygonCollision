#include <SFML/Graphics.hpp>
#include "CollisionPoly.h"
#include "../tests/TestCollisionPoly.h"
#include "../tests/SpeedTest.h"
#include "ContiguousDataManager.h"
#include <iostream>

// To find COM of convex poly:
// 1) Triangulate by drawing segments from an arbitrary vertex
// 2) Find COM of triangles with simple mean
// 3) Sum the area-weighted COMs of the triangles and divide by the sum of their areas.
// 4) That's the answer


sf::Vector2f meanOfVertices(const sf::ConvexShape& shape){
    sf::Vector2f sum{0.f, 0.f};
    for(std::size_t i = 0; i < shape.getPointCount(); ++i){
        sum += shape.getPoint(i);
    }
    return sum / float(shape.getPointCount());
}


void convexShapeTest() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::ConvexShape shape;
    shape.setPointCount(3);
    shape.setPoint(0, sf::Vector2f(0, 0));
    shape.setPoint(1, sf::Vector2f(100, 100));
    shape.setPoint(2, sf::Vector2f(200, 0));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(meanOfVertices(shape));
    shape.setPosition(200, 200);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        shape.setRotation(shape.getRotation() + 1.f);
        window.clear();
        window.draw(shape);
        window.display();
    }
}


int main()
{
    // testCollisionPoly();
    polySpeedTest();

    return 0;
}
