#include "DebugDrawer.h"
#include "App.h"

void DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	drawLine(startPoint, endPoint, color);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{

	glBegin(GL_LINES);
	glColor3f(color.getX(), color.getY(), color.getZ());
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void DebugDrawer::ToggleDebugFlag(int flag)
{
	if (m_debugMode & flag)
		m_debugMode = m_debugMode & (~flag);
	else
		m_debugMode |= flag;
}	
