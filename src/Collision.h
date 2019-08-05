//
// Created by jon on 02/08/2019.
//

#ifndef FASTCOLLISION_COLLISION_H
#define FASTCOLLISION_COLLISION_H

#include <limits>
#include "lib/Vec2Util.h"
#include "SFML/System/Vector2.hpp"


bool areColliding(int polyASize, const sf::Vector2f* polyA, int polyBSize, const sf::Vector2f* polyB);


#endif //FASTCOLLISION_COLLISION_H
