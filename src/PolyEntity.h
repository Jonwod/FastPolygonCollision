//
// Created by jon on 01/08/2019.
//

#ifndef FASTCOLLISION_POLYENTITY_H
#define FASTCOLLISION_POLYENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "SlowCollisionPoly.h"
#include "CollisionPoly.h"


class PolyEntity {
public:
    PolyEntity(const std::vector<sf::Vector2f> & verts, CollisionPolyManager & collisionPolyManager)
    :_collision(nullptr)
    {
        // Visual
        _shape.setPointCount(verts.size());
        for(std::vector<sf::Vector2f>::size_type n = 0; n < verts.size(); ++n) {
            _shape.setPoint(n, verts[n]);
        }
        _shape.setFillColor(sf::Color::Green);
        // ~~~~~

        _collision = collisionPolyManager.add(CollisionData{}, verts);
    }

    void update() {
        if(_collision.isValid()) {
            _shape.setFillColor( _collision.getMetaData().isColliding ? sf::Color::Red : sf::Color::Green);
        }
    }

    void draw(sf::RenderWindow & window) const {
        window.draw(_shape);
    }
private:
    sf::ConvexShape _shape;
    CollisionPolyReference _collision;
};


class PolyEntity_Slow {
public:
    // Requests it's own collision poly from the manager
    PolyEntity_Slow(const std::vector<sf::Vector2f> & verts, SlowCollisionPolyManager<sf::Vector2f> & collisionPolyManager) {
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
