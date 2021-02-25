#pragma once

#include <bullet/btBulletDynamicsCommon.h>
#include "OpenGLMotionState.h"

class GameObject
{
public:
	GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, const btVector3 &initPosition = btVector3(0,0,0), const btQuaternion &initRoation = btQuaternion(0,0, 1.1));
	~GameObject();

	btCollisionShape* GetShape() { return m_pShape; }
	btRigidBody* GetRigidBody() { return m_pBody; }
	OpenGLMotionState* GetMotionState() { return m_pMotionState; }

	void GetTransform(btScalar* transform)
	{
		if (m_pMotionState) m_pMotionState->GetWorldTransform(transform);
	}

	btVector3 GetColor() { return m_color; }

protected:
	btCollisionShape* m_pShape;
	btRigidBody* m_pBody;
	OpenGLMotionState* m_pMotionState;
	btVector3 m_color;
};

