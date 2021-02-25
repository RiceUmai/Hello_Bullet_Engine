#include "App.h"

App::App() : 

m_cameraPosition(10.0f, 5.0f, 0.0f),
m_cameraTarget(0.0f, 0.0f, 0.0f),
m_upVector(0.0f, 1.0f, 0.0f),
m_nearPlane(1.0f),
m_farPlane(1000.0f),
m_pBroadphase(0),
m_pCollisionConfigureation(0),
m_pDispatcher(0),
m_pSolver(0),
m_pWorld(0)
{
}

App::~App()
{
	ShutdownPysics();
}

void App::init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.6f, 0.65f, 0.85f, 0);

	InitPhysics();
}

void App::Keyboard(unsigned char key, int x, int y)
{
}

void App::KeyboardUp(unsigned char key, int x, int y)
{
}

void App::Special(int key, int x, int y)
{
}

void App::SpeciaUp(int key, int x, int y)
{
}

void App::Reshape(int w, int h)
{
	m_screenWidth = w;
	m_screenHeight = h;

	glViewport(0, 0, w, h);
	UpadateCamera();
}

void App::Idle()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float dt = m_clock.getTimeMilliseconds();
	m_clock.reset();
	UpdateScene(dt / 1000);
	UpadateCamera();
	RenderScene();

	glutSwapBuffers();
}

void App::Mouse(int button, int state, int x, int y)
{
}

void App::PassiveMotion(int x, int y)
{
}

void App::Motion(int x, int y)
{
}

void App::Display()
{
}

void App::RenderScene()
{
	btScalar transform[16];
	//if (m_pMotionState)
	//{
	//	m_pMotionState->GetWorldTransform(transform);

	//	glPushMatrix();
	//	glMultMatrixf(transform);
	//	glutSolidCube(1);
	//	glPushMatrix();
	//}

	for (GameObjects::iterator i = m_objects.begin(); i != m_objects.end(); i++)
	{
		GameObject* pObj = *i;

		pObj->GetTransform(transform);
		DrawShape(transform, pObj->GetShape(), pObj->GetColor());
	}
}

void App::UpdateScene(float dt)
{
	if (m_pWorld)
	{
		m_pWorld->stepSimulation(dt);
	}
}

void App::UpadateCamera()
{
	if (m_screenWidth == 0 && m_screenHeight == 0)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspectRatio = (float)m_screenWidth / (float)m_screenHeight;
	glFrustum(-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
		m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2],
		m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
}

void App::DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3& color)
{
	glColor3f(color[0], color[1], color[2]);

	glPushMatrix();
	glMultMatrixf(transform);

	switch (pShape->getShapeType())
	{
		case BOX_SHAPE_PROXYTYPE:
		{
			const btBoxShape* box = static_cast<const btBoxShape*>(pShape);
			btVector3 halfSize = box->getHalfExtentsWithMargin();
			glutSolidCube(5);
			break;
		}
		default:
		{
			break;
		}
	}
	glPopMatrix();
}

GameObject* App::CreateGameObject(btCollisionShape* pShape, const float& mass, const btVector3& color, const btVector3& initPosition, const btQuaternion& initRotaion)
{
	GameObject* pObject = new GameObject(pShape, mass, color, initPosition, initRotaion);
	m_objects.push_back(pObject);

	if (m_pWorld)
	{
		m_pWorld->addRigidBody(pObject->GetRigidBody());
	}
	return pObject;
}