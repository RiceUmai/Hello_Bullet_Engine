#pragma once

#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include <btBulletDynamicsCommon.h>
#include "OpenGLMotionState.h"

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

	virtual void RenderScene();
	virtual void UpdateScene(float dt);

	virtual void InitPhysics() {};
	virtual void ShutdownPhysics() {};

	void UpadateCamera();
	void RotateCamera(float &angle, float value);
	void ZoomCamera(float distance);

	void DrawBox(const btScalar* transform, const btVector3 &halfSize, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));

protected:
	btVector3 m_cameraPosition;
	btVector3 m_cameraTarget;
	float m_cameraDistance;
	float m_cameraPitch;
	float m_cameraYaw;
	float m_nearPlane;
	float m_farPlane;
	btVector3 m_upVector;

	int m_screenWidth;
	int m_screenHeight;

	btBroadphaseInterface* m_pBroadphase;
	btCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btConstraintSolver* m_pSolver;
	btDynamicsWorld* m_pWorld;

	OpenGLMotionState* m_pMotionState;
	btClock m_clock;
};

