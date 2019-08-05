//
// Created by jon on 02/08/2019.
//

#include "Collision.h"


template<typename V>
V perpendicularRight(const V& vec) {
    return V(vec.y, -vec.x);
}


bool areColliding(int polyASize, const sf::Vector2f* polyA, int polyBSize, const sf::Vector2f* polyB) {
    for(int i = 0; i < polyASize; ++i) {
        struct Edge {
            const sf::Vector2f& start;
            const sf::Vector2f& end;
        } edge1{polyA[i], polyA[(i + 1) % polyASize]};

        const sf::Vector2f edgeNormal = perpendicularRight(edge1.end - edge1.start);

        float a_min = std::numeric_limits<float>::infinity();
        float a_max = -std::numeric_limits<float>::infinity();
        for(int vert_i = 0; vert_i < polyASize; ++vert_i) {
            const sf::Vector2f& vert = polyA[vert_i];
            const float vertProj = Vec2::dotProduct(edgeNormal, vert);
            if(vertProj < a_min) { a_min = vertProj; }
            if(vertProj > a_max) { a_max = vertProj; }
        }

        float b_min = std::numeric_limits<float>::infinity();
        float b_max = -std::numeric_limits<float>::infinity();
        for(int vert_i = 0; vert_i < polyBSize; ++vert_i) {
            const sf::Vector2f& vert = polyB[vert_i];
            const float vertProj = Vec2::dotProduct(edgeNormal, vert);
            if(vertProj < b_min) { b_min = vertProj; }
            if(vertProj > b_max) { b_max = vertProj; }
        }

        if(a_max < b_min  ||  a_min > b_max) {
            return false;
        }
    }

    return true;
}