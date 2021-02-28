#pragma once
#include <btBulletDynamicsCommon.h>
#include "OpenGLMotionState.h"
#include <iostream>

class GameObject
{
public:
	GameObject(btCollisionShape* pShape, float mass, const btVector3 &color, 
			const btVector3 &initialPosition = btVector3(0,0,0), 
			const btQuaternion &initialRotation = btQuaternion(0,0,1,1));
	~GameObject();

	btCollisionShape* GetShape() { return m_pShape; }
	btRigidBody* GetRigidBody() { return m_pBody; }
	btMotionState* GetMotionState() { return m_pMotionState; }

	void GetTransform(btScalar* transform)
	{
		if (m_pMotionState) m_pMotionState->GetWorldTransform(transform);
	}

	btVector3 GetColor() { return m_color; }


//=====================================
//Custom
//=====================================
	std::string GetTag() { return Tag; }
	std::string GetName() { return Name; }

	void SetPosition(btVector3 Pos)
	{
		if (m_pBody)
		{
			btTransform transform;
			m_pBody->getMotionState()->getWorldTransform(transform);
			transform.setOrigin(Pos);
			m_pBody->getMotionState()->setWorldTransform(transform);
			m_pBody->setCenterOfMassTransform(transform);
		}
	}

	void SetTag(std::string tag) { Tag = tag; }
	void SetName(std::string name) { Name = name; }
//=====================================
//=====================================

protected:
	btCollisionShape* m_pShape;
	btRigidBody* m_pBody;
	OpenGLMotionState* m_pMotionState;
	btVector3 m_color;

//=====================================
//Custom
//=====================================
	std::string Tag;
	std::string Name;
//=====================================
//=====================================

};

