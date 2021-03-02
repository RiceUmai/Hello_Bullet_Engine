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
	m_pBox = m_Obj;

	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 10.0f, 0.0f));
	CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1.0f, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 20.0f, 0.0f));

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
	if (pBody0 == m_pBox->GetRigidBody() && pBody1 == m_pTrigger ||
		pBody1 == m_pBox->GetRigidBody() && pBody0 == m_pTrigger)
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
	case 'w': {
		//m_objects.back()->SetPosition(btVector3(10.0f, 100.0f, 10.0f));
		btVector3 velocity = (CameraFront) * 50;
		//m_objects.at(1)->GetRigidBody()->setLinearVelocity(velocity);
		Player->GetRigidBody()->applyCentralForce(velocity);

		break;
	}
	case 's': {
		btVector3 velocity = (-CameraFront) * 50;
		Player->GetRigidBody()->applyCentralForce(velocity);
		break;
	}

	case 'a': {
		btVector3 velocity = (-CameraRight) * 50;
		Player->GetRigidBody()->applyCentralForce(velocity);
		break;
	}

	case 'd': {
		btVector3 velocity = (CameraRight) * 50;
		Player->GetRigidBody()->applyCentralForce(velocity);
		break;
	}
	//space bar
	case ' ': {
		btVector3 velocity = btVector3(0.0f, 50.0f, 0.0f);
		Player->GetRigidBody()->applyCentralForce(velocity);
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
		break;
	default:
		break;
	}
}
