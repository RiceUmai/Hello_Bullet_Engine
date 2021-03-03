#pragma once
#include "GameObject.h"
class Chain : public GameObject
{
public:
	Chain(btCollisionShape* pShape, float mass, const btVector3& color,
		const btVector3& initialPosition = btVector3(0, 0, 0),
		const btQuaternion& initialRotation = btQuaternion(0, 0, 1, 1));
	~Chain(); 
protected:

private:
};

