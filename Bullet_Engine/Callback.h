#pragma once

#include "App.h"

static App* g_pApp;

static void KeyboardCallback(unsigned char key, int x, int y)
{
	g_pApp->Keyboard(key, x, y);
}
static void KeyboardDownCallback(unsigned char key, int x, int y)
{
	g_pApp->Keyboard(key, x, y);
}
static void KeyboardUpCallback(unsigned char key, int x, int y)
{
	g_pApp->KeyboardUp(key, x, y);
}
static void SpecialCallback(int key, int x, int y)
{
	g_pApp->Special(key, x, y);
}
static void SpeciaUpCallback(int key, int x, int y)
{
	g_pApp->SpeciaUp(key, x, y);
}
static void ReshapeCallback(int w, int y)
{
	g_pApp->Reshape(w, y);
}
static void IdleCallback()
{
	g_pApp->Idle();
}
static void MouseCallback(int button, int state, int x, int y)
{
	g_pApp->Mouse(button, state, x, y);
}
static void MouseWheel(int button, int dir, int x, int y)
{
	g_pApp->MouseWheel(button, dir, x, y);
}

static void PassiveMotionCallback(int x, int y)
{
	g_pApp->PassiveMotion(x, y);
}
static void MotionCallback(int x, int y)
{
	g_pApp->Motion(x, y);
}
static void DisplayCallback()
{
	g_pApp->Display();
}

int glutMain(int argc, char** argv, int width, int height, const char* title, App* pApp)
{
	g_pApp = pApp;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutCreateWindow(title);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	if (glewInit() != GLEW_OK)
	{
		std::cout<< "failed glewInit" << std::endl;
		return -1;
	}

	printf("%s  \n", glGetString(GL_VERSION));
	
	g_pApp->init();

	glutKeyboardFunc(KeyboardCallback);
	glutKeyboardUpFunc(KeyboardUpCallback);
	glutSpecialFunc(SpecialCallback);
	glutSpecialUpFunc(SpeciaUpCallback);
	glutReshapeFunc(ReshapeCallback);
	glutIdleFunc(IdleCallback);
	glutMouseFunc(MouseCallback);
	glutMouseWheelFunc(MouseWheel);
	glutPassiveMotionFunc(MotionCallback);
	glutMotionFunc(MotionCallback);
	glutDisplayFunc(DisplayCallback);

	g_pApp->Idle();

	glutMainLoop();
	return 0;
}