#include "Chain.h"

Chain::Chain(btCollisionShape* pShape, float mass, const btVector3& color, const btVector3& initialPosition, const btQuaternion& initialRotation)
:GameObject(pShape, mass, color, initialPosition, initialRotation)
{
}

Chain::~Chain()
{
}
