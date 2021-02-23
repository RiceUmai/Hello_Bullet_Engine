#pragma once

#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include <bullet/btBulletDynamicsCommon.h>

class App
{
public:
	App();
	~App();
	void init();

	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void KeyboardUp(unsigned char key, int x, int y);
	virtual void Special(int key, int x, int y);
	virtual void SpeciaUp(int key, int x, int y);
	virtual void Reshape(int w, int h);
	virtual void Idle();
	virtual void Mouse(int button, int state, int x, int y);
	virtual void PassiveMotion(int x, int y);
	virtual void Motion(int x, int y);
	virtual void Display();

	void UpadateCamera();

protected:
	btVector3 m_cameraPosition;
	btVector3 m_cameraTarget;
	float m_nearPlane;
	float m_farPlane;
	btVector3 m_upVector;

	int m_screenWidth;
	int m_screenHeight;
};

