
#include "BasicDemo.h"
#include "Callback.h"

int main(int argc, char** argv)
{
	BasicDemo basicDemo;
	return glutMain(argc, argv, 1024, 768, "Hello Bullet Physics with Freeglut", &basicDemo);
}