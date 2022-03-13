#pragma once
#pragma GCC optimize("O1")
#pragma warning(disable:6011)

#include <iostream>
#include "gui/gui.hpp"
#include "scene/scene.hpp"

class Application {
private:
	GLuint vertexArray;
	WindowEngine* window;
public:
	void loop();
	void detroy();
	Application();
};