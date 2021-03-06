#pragma once
#include <btBulletCollisionCommon.h>
class OpenGLMotionState : public btDefaultMotionState
{
public:
	OpenGLMotionState(const btTransform& transform) : btDefaultMotionState(transform){};

	/// <summary>
	/// transformを行列に変換
	/// </summary>
	/// <param name="transform"></param>
	void GetWorldTransform(btScalar* transform)
	{
		btTransform trans;
		getWorldTransform(trans);
		trans.getOpenGLMatrix(transform);
	}
};

