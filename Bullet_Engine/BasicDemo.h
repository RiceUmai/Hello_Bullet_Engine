#pragma once

#include "App.h"
#include <btBulletDynamicsCommon.h>

class BasicDemo : public App
{
public:
	virtual void InitPhysics() override;
	virtual void ShutdownPhysics() override;
};

