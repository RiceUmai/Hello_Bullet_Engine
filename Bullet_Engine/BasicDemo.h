#pragma once

#include "App.h"
#include <btBulletDynamicsCommon.h>

class BasicDemo : public App
{
public:
	BasicDemo();

	virtual void InitPhysics() override;
	virtual void ShutdownPhysics() override;

	void CreateObjects();

	virtual void CollisionEvent(btRigidBody* pBody0, btRigidBody* pBody1) override;
	virtual void Keyboard(unsigned char key, int x, int y) override;
	virtual void KeyboardUp(unsigned char key, int x, int y) override;
	virtual void UpdateScene(float dt) override;

protected:
	GameObject* m_pBox;
	btCollisionObject* m_pTrigger;

	btVector3 MovementVelocity;

	bool m_bApplyForce;
	bool Movement;
};

