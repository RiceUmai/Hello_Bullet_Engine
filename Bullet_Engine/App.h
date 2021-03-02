#pragma once

#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include "OpenGLMotionState.h"
#include "GameObject.h"
#include "DebugDrawer.h"

#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
typedef std::vector<GameObject*> GameObjects;
typedef std::pair<const btRigidBody*, const btRigidBody*> CollisionPair;
typedef std::set<CollisionPair> CollisionPairs;

#include <iostream>

struct RayResult
{
	btRigidBody* pBody;
	btVector3 hitPoint;
};

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
	virtual void MouseWheel(int button, int dir, int x, int y);
	virtual void PassiveMotion(int x, int y);
	virtual void Motion(int x, int y);
	virtual void Display();

	virtual void RenderScene(float dt);
	virtual void UpdateScene(float dt);

	virtual void InitPhysics() {};
	virtual void ShutdownPhysics() {};

	void UpadateCamera();
	void RotateCamera(float& angle, float value);
	void ZoomCamera(float distance);

	void DrawBox(const btVector3& halfSize);
	void DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3& color);
	GameObject* CreateGameObject(btCollisionShape* pShape,
		const float& mass,
		const btVector3& color = btVector3(1.0f, 1.0f, 1.0f),
		const btVector3& initialPotation = btVector3(0.0f, 0.0f, 0.0f),
		const btQuaternion& initialRotation = btQuaternion(0, 0, 1, 1));



	void ShootBox(const btVector3& direction);
	void DestroyGameObject(btRigidBody* pBody);
	GameObject* FindGameObject(btRigidBody* pBody);

	btVector3 GetPickingRay(int x, int y);
	bool Raycast(const btVector3& startPosition, const btVector3& direction, RayResult& output);

	
	void CheckForCollisionEvents();
	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1);
	virtual void SeparationEvent(btRigidBody* pBody0, btRigidBody* pBody1);

	//===========================
	//Custom
	//===========================
	GameObject* FindGameObject(std::string name);

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

	btClock m_clock;

	GameObjects m_objects;
	DebugDrawer* m_pDebugDrawer;

	btVector3 CameraRight;
	btVector3 CameraFront;

	CollisionPairs m_pairsLastUpdate;

	float DeltaTime;

	GameObject* Player;
};

