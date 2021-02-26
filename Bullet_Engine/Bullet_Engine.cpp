
#include "App.h"
#include "Callback.h"

int main(int argc, char** argv)
{//
	App app;
	return glutMain(argc, argv, 1024, 768, "Hello Bullet Physics with Freeglut", &app);
}