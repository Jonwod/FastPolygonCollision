//
// Created by jon on 01/08/2019.
//

#ifndef FASTCOLLISION_POLYENTITY_H
#define FASTCOLLISION_POLYENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "SlowCollisionPoly.h"

class PolyEntity {
public:
    // Requests it's own collision poly from the manager
    PolyEntity(const std::vector<sf::Vector2f> & verts, SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager) {
        // Visual
        _shape.setPointCount(verts.size());
        for(std::vector<sf::Vector2f>::size_type n = 0; n < verts.size(); ++n) {
            _shape.setPoint(n, verts[n]);
        }
        _shape.setFillColor(sf::Color::Green);
        // ~~~~~

        // Collision
        _collision = collisionPolyManager.add(verts);
        // ~~~~~~~~~
    }

    void update() {
        if(_collision) {
            _shape.setFillColor( _collision->isColliding() ? sf::Color::Red : sf::Color::Green);
        }
    }

    void draw(sf::RenderWindow & window) const {
        window.draw(_shape);
    }
private:
    sf::ConvexShape _shape;
    SlowCollisionPoly<sf::Vector2f> * _collision = nullptr;
};

#endif //FASTCOLLISION_POLYENTITY_H
