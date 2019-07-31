//
// Created by jon on 26/07/2019.
//

#ifndef FASTCOLLISION_VECTORUTILS_H
#define FASTCOLLISION_VECTORUTILS_H

/* A set of functions for use on any 2d vector class with a public x and y variables,
 * that support the basic mathematical operations. */
namespace Vec2 {

    template<typename T>
    std::string toStr(const sf::Vector2<T>& vec) {
        return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
    }

}

#endif //FASTCOLLISION_VECTORUTILS_H