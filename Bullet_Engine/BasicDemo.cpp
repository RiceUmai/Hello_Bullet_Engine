#include "BasicDemo.h"

void BasicDemo::InitPhysics()
{
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	m_pBroadphase = new btDbvtBroadphase();
	m_pSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	m_pWorld->setGravity(btVector3(0, -10, 0));

	CreateObjects();
}

void BasicDemo::ShutdownPhysics()
{
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

void BasicDemo::CreateObjects()
{
	//btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	//
	//btTransform transform;
	//transform.setIdentity();
	//transform.setOrigin(btVector3(0.0f, 10.0f, 0.0f));

	//m_pMotionState = new OpenGLMotionState(transform);
	//
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0f, m_pMotionState, pBoxShape);
	//btRigidBody* pRigidBody = new btRigidBody(rbInfo);

	//m_pWorld->addRigidBody(pRigidBody);
	
	GameObject* m_Obj;
	m_Obj = CreateGameObject(new btBoxShape(btVector3(1, 50, 50)), 0.0f, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));
	m_Obj->SetTag("Ground");
	m_Obj->SetName("Ground");

	m_Obj = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(0.0f, 1.0f, 0.0f), btVector3(10.0f, 5.0f, 0.0f));
	m_Obj->SetTag("Player");
	m_Obj->SetName("Player");

	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 10.0f, 0.0f));
}
