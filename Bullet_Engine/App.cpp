#include "App.h"

App::App() : m_cameraPosition(10.0f, 5.0f, 0.0f), m_cameraTarget(0.0f, 0.0f, 0.0f), 
			 m_upVector(0.0f, 1.0f, 0.0f), m_nearPlane(1.0f), m_farPlane(1000.0f)
{
}

App::~App()
{
}

void App::init()
{
	glClearColor(0.6f, 0.65f, 0.85f, 0);
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
	UpadateCamera();
	
	glutSolidCube(1);
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
	gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2],
			  m_cameraTarget[0],   m_cameraTarget[1],   m_cameraTarget[2],
			  m_upVector.getX(),   m_upVector.getY(),   m_upVector.getZ());
}
