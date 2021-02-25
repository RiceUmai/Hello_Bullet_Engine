#pragma once
#include"App.h"
#include"bullet/btBulletDynamicsCommon.h"
class BasicDemo : public App
{
public:
	virtual void InitPhysics() override;
	virtual void ShutdownPysics() override;

	void CreateObjects();
};

