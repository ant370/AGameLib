#ifndef _PHYSICS
#define _PHYSICS

#include "Math.h"
#include <stdbool.h>

typedef float timeSeconds;
 
typedef struct PhysicsWorld 
{
    Vec4 gravity;

} PhysicsWorld;

typedef struct PhysicsAABB
{
    Vec4 center;
    Vec4 dimensions;
} PhysicsAABB;

typedef struct PhysicsObject
{
    int id;
    int state;
    PhysicsAABB box;

} PhysicsObject;

PhysicsWorld PhysicsStepWorld(timeSeconds t);

bool PhysicsCollide(PhysicsObject a, PhysicsObject b);

#endif