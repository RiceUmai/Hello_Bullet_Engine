#include "BasicDemo.h"

void BasicDemo::InitPhysics()
{
	m_pCollisionConfigureation = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfigureation);
	m_pBroadphase = new btDbvtBroadphase();
	m_pSolver = new btSequentialImpulseConstraintSolver();
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfigureation);

	CreateObjects();
}

void BasicDemo::ShutdownPysics()
{
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfigureation;
}

void BasicDemo::CreateObjects()
{
	//btBoxShape* pBoxShape = new btBoxShape(btVector3(1.0, 1.0f, 1.0f));
	//
	//btTransform transform;
	//transform.setIdentity();
	//transform.setOrigin(btVector3(0.0f, 10.0f, 0.0f));
	//
	//m_pMotionState = new OpenGLMotionState(transform);

	//btRigidBody::btRigidBodyConstructionInfo rbinfo(1.0f, m_pMotionState, pBoxShape);
	//btRigidBody* pRigidBody = new btRigidBody(rbinfo);
	//m_pWorld->addRigidBody(pRigidBody);

	/*ADD*/	CreateGameObject(new btBoxShape(btVector3(100, 1, 100)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));
	/*ADD*/
	/*ADD*/	// create our original red box
	/*ADD*/	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(1.0f, 0.2f, 0.2f), btVector3(0.0f, 10.0f, 0.0f));
	/*ADD*/
	/*ADD*/	// create a second box
	/*ADD*/	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.2f, 0.8f), btVector3(0.0f, 5.0f, 0.0f));
	///*ADD*/	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0, btVector3(0.0f, 0.2f, 0.8f), btVector3(1.25f, 20.0f, 0.0f));
}