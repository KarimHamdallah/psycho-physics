#include <SandBox/SandBox.h>

int main(int argc, char* argv[])
{

	SandBox* sandbox = new SandBox();
	sandbox->run();
	delete sandbox;

	return 0;
}