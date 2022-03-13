#include "gui.hpp"

void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	CommandService::getInstance()->onKeyPress(key, action);
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
	CommandService::getInstance()->onMouseMove(xpos, ypos);
}

void onWindowSizeChange(GLFWwindow* window, int width, int height) {
	const auto config = Configuration::getInstance();
	glViewport(0, 0, width, height);
	config->getViewport()->width = width;
	config->getViewport()->height = height;
}

void WindowEngine::init() {
}

bool WindowEngine::shouldClose() {
	return !glfwWindowShouldClose(this->window);
}

void WindowEngine::render() {
	gui->render();
	glfwSwapBuffers(this->window);
	glfwPollEvents();
}

WindowEngine::WindowEngine() {
	const auto viewport = Configuration::getInstance()->getViewport();
	glfwInit();
	this->window = glfwCreateWindow(viewport->width, viewport->height, "Mass-spring", NULL, NULL);
	glfwSetKeyCallback(this->window, onKeyPress);
	glfwSetCursorPosCallback(this->window, onMouseMove);
	glfwSetWindowSizeCallback(this->window, onWindowSizeChange);
	glfwMakeContextCurrent(window);
	// glfwSwapInterval(0);
	gl3wInit();
	gui = new GuiRenderer(this->window);
	command = CommandService::getInstance();
}