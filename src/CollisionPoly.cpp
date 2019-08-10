//
// Created by jon on 24/07/2019.
//

#include "CollisionPoly.h"
#include "Collision.h"


void CollisionPolyManager::runCollisionCheck() {
    for(CollisionPolyReference polyA = first(); polyA.isValid(); polyA = next(polyA)) {
        for (CollisionPolyReference polyB = next(polyA); polyB.isValid(); polyB = next(polyB)) {
            if(areColliding(polyA.numDataPoints(), &polyA.getDatum(0), polyB.numDataPoints(), &polyB.getDatum(0))) {
                polyA.getMetaData().isColliding = true;
                polyB.getMetaData().isColliding = true;
            }
        }
    }
}
