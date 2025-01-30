#include <renderer/Renderer.h>

Camera camera;
float lastX;
float lastY;
bool firstClick = true;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

Renderer::~Renderer()
{
}

void Renderer::start(unsigned int width, unsigned int height, const char* title)
{
	this->w_Width = width;
	this->w_Height = height;
	this->w_Title = title;
	window.CreateWindow(w_Width, w_Height, w_Title.c_str());
	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	core_shader.setupShader("shaders/core_vertex.glsl", "shaders/core_fragment.glsl");
	texture_atlas.setTexture("textures/texture-atlas-minecraft.png");

	chunk.init(2, 2, 2);
}

void Renderer::update()
{
	while(!window.ShouldClose()){
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.ClearScreen();

		wfb_Width = window.getFBWidth();
		wfb_Height = window.getFBHeight();

		render();

		processInput();
		window.SwapBuffers();
	}
}

void Renderer::render()
{
	core_shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)wfb_Width / (float)wfb_Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	core_shader.setMat4("projection", projection);
	core_shader.setMat4("view", view);

	chunk.renderChunk(core_shader, texture_atlas, projection, view);
}

void Renderer::processInput()
{
	window.processInput();
	if (window.getInput().isKeyPressed(GLFW_KEY_W))
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (window.getInput().isKeyPressed(GLFW_KEY_S))
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (window.getInput().isKeyPressed(GLFW_KEY_A))
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (window.getInput().isKeyPressed(GLFW_KEY_D))
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstClick)
	{
		lastX = xpos;
		lastY = ypos;
		firstClick = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}