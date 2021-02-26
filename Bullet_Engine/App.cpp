#include "App.h"

#define RADIANS_PER_DEGREE 0.01745329f
#define CAMERA_STEP_SIZE 5.0f

App::App() :
	m_cameraPosition(10.0f, 5.0f, 0.0f),
	m_cameraTarget(0.0f, 0.0f, 0.0f),
	m_cameraDistance(15.0f),
	m_cameraPitch(20.0f),
	m_cameraYaw(0.0f),
	m_upVector(0.0f, 1.0f, 0.0f),
	m_nearPlane(1.0f),
	m_farPlane(1000.0f),
	//===================
	//Bullet Engine init variable
	m_pBroadphase(0),
	m_pCollisionConfiguration(0),
	m_pDispatcher(0),
	m_pSolver(0),
	m_pWorld(0)
{
}

App::~App()
{
	ShutdownPhysics();
}

void App::init()
{
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(0.6f, 0.65f, 0.85f, 0);
	InitPhysics();
}

void App::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z': 
		ZoomCamera(+CAMERA_STEP_SIZE);
		break;
	case 'x':
		ZoomCamera(-CAMERA_STEP_SIZE);
		break;
	default:
		break;
	}
}

void App::KeyboardUp(unsigned char key, int x, int y)
{
}

void App::Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		RotateCamera(m_cameraYaw, +CAMERA_STEP_SIZE);
		break;
	case GLUT_KEY_RIGHT:
		RotateCamera(m_cameraYaw, -CAMERA_STEP_SIZE);
		break;
	case GLUT_KEY_UP:
		RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE);
		break;
	case GLUT_KEY_DOWN:
		RotateCamera(m_cameraPitch,-CAMERA_STEP_SIZE);
		break;
	default:
		break;
	}
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
	UpadateCamera();

	DrawBox(btVector3(5, 5, 5));
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

	float pitch = m_cameraPitch * RADIANS_PER_DEGREE;
	float yaw = m_cameraYaw * RADIANS_PER_DEGREE;

	btQuaternion rotation(m_upVector, yaw);

	btVector3 cameraPosition(0, 0, 0);
	cameraPosition[2] = -m_cameraDistance;

	btVector3 forward(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (forward.length2() < SIMD_EPSILON)
	{
		forward.setValue(1.0f, 0.0f, 0.0f);
	}
	btVector3 right = m_upVector.cross(forward);
	btQuaternion roll(right, -pitch);
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;

	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();

	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
		m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2],
		m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());

	
}

void App::RotateCamera(float& angle, float value)
{
	angle -= value;
	if (angle < 0) angle += 360;
	if (angle >= 360) angle -= 360;
	UpadateCamera();
}

void App::ZoomCamera(float distance)
{
	m_cameraDistance -= distance;
	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;
	UpadateCamera();
}

void App::DrawBox(const btVector3& halfSize, const btVector3& color)
{
	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();

	glColor3f(color.x(), color.y(), color.z());

	btVector3 vertices[8] = {
	btVector3(halfWidth,halfHeight,halfDepth),
	btVector3(-halfWidth,halfHeight,halfDepth),
	btVector3(halfWidth,-halfHeight,halfDepth),
	btVector3(-halfWidth,-halfHeight,halfDepth),
	btVector3(halfWidth,halfHeight,-halfDepth),
	btVector3(-halfWidth,halfHeight,-halfDepth),
	btVector3(halfWidth,-halfHeight,-halfDepth),
	btVector3(-halfWidth,-halfHeight,-halfDepth) };

	static int indices[36] = {
		0,1,2,
		3,2,1,
		4,0,6,
		6,0,2,
		5,1,4,
		4,1,0,
		7,3,1,
		7,1,5,
		5,4,7,
		7,4,6,
		7,2,3,
		7,6,2 };

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < 36; i += 3)
	{
		const btVector3& vert1 = vertices[indices[i]];
		const btVector3& vert2 = vertices[indices[i + 1]];
		const btVector3& vert3 = vertices[indices[i + 2]];

		btVector3 normal = (vert3 - vert1).cross(vert2 - vert1);
		normal.normalize();

		glNormal3f(normal.x(), normal.y(), normal.z());

		glVertex3f(vert1.x(), vert1.y(), vert1.z());
		glVertex3f(vert2.x(), vert2.y(), vert2.z());
		glVertex3f(vert3.x(), vert3.y(), vert3.z());
	}
	glEnd();
}
