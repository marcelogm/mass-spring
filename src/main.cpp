#include "main.hpp"

using namespace std;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	auto app = new Application();
	app->loop();
	app->detroy();
}
