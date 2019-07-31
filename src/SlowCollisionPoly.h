//
// Created by jon on 26/07/2019.
//

#ifndef FASTCOLLISION_SLOWCOLLISIONPOLY_H
#define FASTCOLLISION_SLOWCOLLISIONPOLY_H

#include <vector>



template<typename T>
class SlowCollisionPoly {
public:
    explicit SlowCollisionPoly(const std::vector<T> & verts) {
        _vertices = verts;
    }

    bool isColliding = false;
private:
    std::vector<T> _vertices;
};



template<typename VERT_T>
class SlowCollisionPolyManager {
public:
    SlowCollisionPoly<VERT_T> * add(std::vector<VERT_T> verts) {
        _polys.push_back(SlowCollisionPoly<VERT_T>(verts));
        return &_polys.back();
    }
private:
    std::vector<SlowCollisionPoly<VERT_T>> _polys;
};



#endif //FASTCOLLISION_SLOWCOLLISIONPOLY_H
