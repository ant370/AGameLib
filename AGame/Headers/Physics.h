#ifndef _PHYSICS
#define _PHYSICS

#include "Math.h"

typedef float timeSeconds;

typedef struct PhysicsBody
{
    float mass;
    Vec4 position;
    Vec4 velocity;
    Vec4 acceleration;
} PhysicsBody;

typedef struct PhysicsGeom
{
    

} PhysicsGeom;

typedef struct PhysicsWorld 
{
    Vec4 gravity;

} PhysicsWorld;


PhysicsWorld PhysicsStepWorld(timeSeconds t);


#endif