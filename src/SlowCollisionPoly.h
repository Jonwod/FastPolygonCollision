//
// Created by jon on 26/07/2019.
//

#ifndef FASTCOLLISION_SLOWCOLLISIONPOLY_H
#define FASTCOLLISION_SLOWCOLLISIONPOLY_H

#include <vector>
#include "Collision.h"


template<typename T>
class SlowCollisionPoly {
    template<typename T2>
    friend class SlowCollisionPolyManager;
public:
    explicit SlowCollisionPoly(const std::vector<T> & verts) {
        _vertices = verts;
    }

    bool isColliding() {
        return _isColliding;
    }

    inline const std::vector<T> & verts() { return _vertices; };
private:
    std::vector<T> _vertices;
    bool _isColliding = false;
};



template<typename VERT_T>
class SlowCollisionPolyManager {
public:
    SlowCollisionPolyManager(unsigned int capacity)
        :_capacity(capacity)
    {
        _polys.reserve(capacity);
    }

    SlowCollisionPoly<VERT_T> * add(std::vector<VERT_T> verts) {
        if(_polys.size() == _capacity) {
            return nullptr;
        }

        _polys.push_back(SlowCollisionPoly<VERT_T>(verts));
        return &_polys.back();
    }

    void runCollisionCheck() {
        for(int i = 0; i < _polys.size(); ++i) {
            _polys[i]._isColliding = false;
        }

        for(int i = 0; i < _polys.size(); ++i) {
            for(int j = i + 1; j < _polys.size(); ++j) {
                const bool result = areColliding(_polys[i].verts().size(), _polys[i].verts().data(),
                        _polys[j].verts().size(), _polys[j].verts().data());
                if(result) {
                    _polys[i]._isColliding = result;
                    _polys[j]._isColliding = result;
                }
            }
        }
    }

    SlowCollisionPolyManager() = delete;
private:
    std::vector<SlowCollisionPoly<VERT_T>> _polys;
    const unsigned int _capacity;
};



#endif //FASTCOLLISION_SLOWCOLLISIONPOLY_H
