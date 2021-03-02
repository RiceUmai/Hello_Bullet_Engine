#include "BasicDemo.h"


BasicDemo::BasicDemo() :
	App(),
	m_bApplyForce(false),
	Movement(false),
	MovementVelocity(0, 0, 0)
{
}

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
	CreateGameObject(new btBoxShape(btVector3(1, 100, 100)), 0.0f, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));
	Player = m_Obj = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(0.0f, 1.0f, 0.0f), btVector3(0.0f, 5.0f, -30.0f));
	
	//CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 10.0f, 0.0f));
	//CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 20.0f, 0.0f));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(i, j, 0), btVector3(j * 2, 10+ i * 2, 0));
		}

	}

	//trigger object
	m_pTrigger = new btCollisionObject();
	m_pTrigger->setCollisionShape(new btBoxShape(btVector3(1.0f, 0.25f, 1.0f)));

	btTransform triggerTrans;
	triggerTrans.setIdentity();
	triggerTrans.setOrigin(btVector3(0, 1.5f, 0));
	m_pTrigger->setWorldTransform(triggerTrans);
	m_pTrigger->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
	m_pWorld->addCollisionObject(m_pTrigger);

}

void BasicDemo::CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1)
{
	if (pBody0 == Player->GetRigidBody() && pBody1 == m_pTrigger ||
		pBody1 == Player->GetRigidBody() && pBody0 == m_pTrigger)
	{
		CreateGameObject(new btBoxShape(btVector3(2,2,2)), 1.0f, btVector3(0.3, 0.7, 0.3), btVector3(5, 10, 0));
	}
}

void BasicDemo::Keyboard(unsigned char key, int x, int y)
{
	App::Keyboard(key, x, y);

	switch (key)
	{
		//Player movement
		//Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
	case 'w': {
		MovementVelocity = (CameraFront) * 50;
		Movement = true;
		Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		break;
	}
	case 's': {
		MovementVelocity = (-CameraFront) * 50;
		Movement = true;
		Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		break;
	}

	case 'a': {
		MovementVelocity = (-CameraRight) * 50;
		Movement = true;
		Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		break;
	}

	case 'd': {
		MovementVelocity = (CameraRight) * 50;
		Movement = true;
		Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		break;
	}
	//space bar
	case ' ': {
		//btVector3 velocity = btVector3(0.0f, 50.0f, 0.0f);
		//Player->GetRigidBody()->applyCentralForce(velocity);
		
		m_bApplyForce = true;
		Player->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
		break;
	}
	default:
		break;
	}
}

void BasicDemo::KeyboardUp(unsigned char key, int x, int y)
{
	App::KeyboardUp(key, x, y);
	switch (key)
	{

	case 'w': {
		Movement = false;
		MovementVelocity = btVector3(0, 0, 1);
		Player->GetRigidBody()->forceActivationState(ACTIVE_TAG);
		break;
	}
	case 's': {
		Movement = false;
		MovementVelocity = btVector3(0, 0, -1);
		Player->GetRigidBody()->forceActivationState(ACTIVE_TAG);
		break;
	}

	case 'a': {
		Movement = false;
		MovementVelocity = btVector3(1, 0, 0);
		Player->GetRigidBody()->forceActivationState(ACTIVE_TAG);
		break;
	}

	case 'd': {
		Movement = false;
		MovementVelocity = btVector3(-1, 0, 0);
		Player->GetRigidBody()->forceActivationState(ACTIVE_TAG);
		break;
	}

	case ' ' : {
		m_bApplyForce = false;
		Player->GetRigidBody()->forceActivationState(ACTIVE_TAG);
		break;
	}
		
	default:
		break;
	}
}

void BasicDemo::UpdateScene(float dt)
{
	App::UpdateScene(dt);
	
	if (m_bApplyForce)
	{
		if (!Player) return;
		Player->GetRigidBody()->applyCentralForce(btVector3(0, 30, 0));
	}

	if(Movement)
	{
		Player->GetRigidBody()->applyCentralForce(MovementVelocity);
	}
}
