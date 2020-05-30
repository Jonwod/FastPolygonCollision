cmake_minimum_required(VERSION 3.14)
project(FastCollision)

set(CMAKE_CXX_STANDARD 14)

add_executable(FastCollision src/main.cpp
        src/CollisionPoly.h
        src/CollisionPoly.cpp
        tests/TestCollisionPoly.cpp
        tests/TestCollisionPoly.h
        src/SlowCollisionPoly.h
        src/lib/Vec2Util.h tests/SpeedTest.cpp tests/SpeedTest.h src/ContiguousDataManager.h src/lib/Rand.cpp src/lib/Rand.h src/PolyEntity.cpp src/PolyEntity.h src/Collision.h src/lib/JMath.h src/lib/RandomPolygon.h)

target_link_libraries(FastCollision sfml-graphics)
target_link_libraries(FastCollision sfml-window)
target_link_libraries(FastCollision sfml-system)
target_link_libraries(FastCollision sfml-audio)
target_link_libraries(FastCollision sfml-network)