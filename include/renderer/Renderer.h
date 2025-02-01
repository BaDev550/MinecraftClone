#pragma once

#include <core/Window.h>
#include <core/Camera.h>
#include <core/Game/Chunk.h>
#include <core/Game/World.h>
#include <core/Ray.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <string>

class Renderer
{
private:
	unsigned int w_Width{ 800 };
	unsigned int w_Height{ 800 };
	std::string w_Title = "OpenGL Window";
	Window window;

	int wfb_Width{};
	int wfb_Height{};

	Shader core_shader;
	Texture texture_atlas;
	Raycaster raycaster;

	int viewDistance = 8;

	FrustumCulling frustum;
	World world;

	float deltaTime;
	float lastFrame;
public:
	Renderer() {}
	~Renderer();

	void start(unsigned int width, unsigned int height, const char* title);
	void update();
	void drawDebugGUI();
	void render();
	void processInput();
};

