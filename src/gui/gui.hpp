#pragma once
#include "vgl.h"
#include <imgui.h>
#include <vector>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include "../camera/camera.hpp"
#include "../configuration/config.hpp"
#include "../simulation/simulation.hpp"

using std::vector;

class CommandStrategy {
protected:
	Camera* camera;
	bool isPressing(int action);
public:
	CommandStrategy(Camera* camera);
	virtual bool matches(int key, int action) = 0;
	virtual void apply() = 0;
};

class MoveLeft : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveLeft(Camera* camera) : CommandStrategy(camera) {};
};

class MoveRight : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveRight(Camera* camera) : CommandStrategy(camera) {};
};

class MoveFront : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveFront(Camera* camera) : CommandStrategy(camera) {};
};

class MoveBack : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveBack(Camera* camera) : CommandStrategy(camera) {};
};

class MoveUp : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveUp(Camera* camera) : CommandStrategy(camera) {};
};

class MoveDown : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	MoveDown(Camera* camera) : CommandStrategy(camera) {};
};

class YawLeft : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	YawLeft(Camera* camera) : CommandStrategy(camera) {};
};

class YawRight : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	YawRight(Camera* camera) : CommandStrategy(camera) {};
};

class PitchUp : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	PitchUp(Camera* camera) : CommandStrategy(camera) {};
};

class PitchDown : public CommandStrategy {
public:
	bool matches(int key, int action);
	void apply();
	PitchDown(Camera* camera) : CommandStrategy(camera) {};
};

class CommandService {
private:
	vector<CommandStrategy*> strategies;
	static CommandService* instance;
	Camera* camera;
	CommandService();
public:
	static CommandService* getInstance();
	void onKeyPress(int, int);
	void onMouseMove(double, double);
	void setCamera(Camera* camera);
};

class GuiRenderer {
public:
	void render();
	GuiRenderer(GLFWwindow* window);
};

class WindowEngine {
private:
	GLFWwindow* window;
	GuiRenderer* gui;
	CommandService* command;
public:
	void init();
	void render();
	bool shouldClose();
	WindowEngine();
};
