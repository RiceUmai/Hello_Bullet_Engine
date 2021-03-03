#include "GameObject.h"

GameObject::GameObject(btCollisionShape* pShape, float mass, const btVector3& color, const btVector3& initialPosition, const btQuaternion& initialRotation)
{
	m_pShape = pShape;
	m_color = color;

	Tag = "default";

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(initialPosition);
	transform.setRotation(initialRotation);

	m_pMotionState = new OpenGLMotionState(transform);
	btVector3 locallnertia(0, 0, 0);

	if (mass != 0.0f)
		pShape->calculateLocalInertia(mass, locallnertia);

	btRigidBody::btRigidBodyConstructionInfo clnfo(mass, m_pMotionState, pShape, locallnertia);
	m_pBody = new btRigidBody(clnfo);
}

GameObject::~GameObject()
{
	delete m_pBody;
	delete m_pMotionState;
	delete m_pShape;
}
