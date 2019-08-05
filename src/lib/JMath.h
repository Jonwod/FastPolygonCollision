//
// Created by jon on 05/08/2019.
//

#ifndef FASTCOLLISION_JMATH_H
#define FASTCOLLISION_JMATH_H


namespace JMath {

    template<typename T>
    T lerp(const T& a, const T& b, float alpha) {
        return (1.f - alpha) * a + alpha * b;
    }

}


#endif //FASTCOLLISION_JMATH_H
