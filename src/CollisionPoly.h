//
// Created by jon on 24/07/2019.
//

#ifndef FASTCOLLISION_COLLISIONPOLY_H
#define FASTCOLLISION_COLLISIONPOLY_H

#include <cstddef>
#include <vector>
#include <array>
#include <cassert>
#include <cstdint>
#include "ContiguousDataManager.h"
#include "SFML/System/Vector2.hpp"


struct CollisionData {
    bool isColliding = false;
};

typedef ContiguousDataReference<sf::Vector2f, CollisionData> CollisionPolyReference;


class CollisionPolyManager: public ContiguousDataManager<sf::Vector2f, CollisionData> {
public:
    using ContiguousDataManager::ContiguousDataManager;
    void runCollisionCheck();
};



#endif //FASTCOLLISION_COLLISIONPOLY_H
