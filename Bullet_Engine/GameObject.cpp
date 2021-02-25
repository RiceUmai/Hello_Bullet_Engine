#include "GameObject.h"

GameObject::GameObject(btCollisionShape* pShape, float mass, const btVector3& color, const btVector3& initPosition, const btQuaternion& initRoation)
: m_pShape(pShape), m_color(color)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(initPosition);
	transform.setRotation(initRoation);

	m_pMotionState = new OpenGLMotionState(transform);
	btVector3 localInertia(0,0,0);
	
	if (mass != 0.0f)
		pShape->calculateLocalInertia(mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo clnfo(mass, m_pMotionState, pShape, localInertia);
	m_pBody = new btRigidBody(clnfo);
}

GameObject::~GameObject()
{
	delete m_pBody;
	delete m_pMotionState;
	delete m_pShape;
}
