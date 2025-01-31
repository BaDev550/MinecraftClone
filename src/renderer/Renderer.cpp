#include <renderer/Renderer.h>

Camera camera;
float lastX;
float lastY;
bool firstClick = true;

double mouseX, mouseY;
glm::vec3 hitPosition;
bool hit = false;
BlockType bType = DIRT;

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

	core_shader.setupShader("shaders/core_vertex.glsl", "shaders/core_fragment.glsl");
	texture_atlas.setTexture("textures/texture_atlas.png");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	chunk.init(viewDistance, viewDistance, viewDistance);
}

void Renderer::update()
{
	while(!window.ShouldClose()){
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.ClearScreen();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		wfb_Width = window.getFBWidth();
		wfb_Height = window.getFBHeight();

		drawDebugGUI();
		render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		processInput();
		window.SwapBuffers();
	}
}

void Renderer::render()
{
	core_shader.use();
	glDepthMask(GL_TRUE);

	glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)wfb_Width / (float)wfb_Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	core_shader.setMat4("projection", projection);
	core_shader.setMat4("view", view);

	raycaster.init(projection, view);

	glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);

	chunk.renderChunk(core_shader, texture_atlas, camera.Position, renderDistance);
	float closestT = std::numeric_limits<float>::max();

	Ray ray = raycaster.castFromCamera(static_cast<float>(mouseX), static_cast<float>(mouseY), window.getFBWidth(), window.getFBHeight(), camera);
	float maxDistance = 100.0f;

	for (const auto& block : chunk.blocks) {
		glm::vec3 minBounds = block.second.position;
		glm::vec3 maxBounds = block.second.position + glm::vec3(1.0f);

		float t;
		if (raycaster.intersectAABB(ray, minBounds, maxBounds, t)) {
			if (t < closestT && t < maxDistance) {
				closestT = t;
				hitPosition = ray.origin + ray.direction * t;
				hit = true;
			}
		}
	}
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

	if (window.getInput().isKeyPressed(GLFW_KEY_M, true)) {
		firstClick = true;
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (window.getInput().isKeyPressed(GLFW_KEY_M, false, true))
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (window.getInput().isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		camera.MovementSpeed = 10;
	else if (window.getInput().isKeyReleased(GLFW_KEY_LEFT_SHIFT))
		camera.MovementSpeed = camera.SPEED;

	if (window.getInput().isKeyPressed(GLFW_KEY_F, true))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (window.getInput().isKeyPressed(GLFW_KEY_F, false, true))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if(window.getInput().isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)){
		if (hit) {
			glm::ivec3 chunkCoords = glm::floor(hitPosition);

			chunk.addBlock(chunkCoords.x, chunkCoords.y, chunkCoords.z, bType, camera.Position, camera.Front);
		}
	}

	if (window.getInput().isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		if (hit) {
			glm::ivec3 chunkCoords = glm::floor(hitPosition);

			chunk.removeBlock(chunkCoords.x, chunkCoords.y, chunkCoords.z);
		}
	}
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED) return;
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

int frameCounter;
int FPS = 0;

void Renderer::drawDebugGUI() {
	if (ImGui::Begin("Debugger")) {
		ImGui::SetWindowSize(ImVec2(200, 200));
		ImGui::SetWindowPos(ImVec2(1.5f, ((window.getFBHeight() * -1.0f) / 200.f) + 1.5f ));

		frameCounter++;
		if (frameCounter >= 30) {
			FPS = floor(1 /deltaTime);
			frameCounter = 0;
		}
		ImGui::Text("FPS: %d", FPS);

		int renderedBlocks{};
		for (int i = 0; i < chunk.blocks.size(); i++) {
			if (chunk.blocks[i].bVisible) {
				renderedBlocks = i;
			}
		}
		ImGui::Text("Blocks map size: %d", chunk.blocks.size());
		ImGui::Text("Blocks rendered size: %d", renderedBlocks);

		if (ImGui::Button("Oak Wood"))
			bType = OAK_WOOD;
		ImGui::SameLine();
		if (ImGui::Button("Plank"))
			bType = PLANK;
		if (ImGui::Button("Coblestone"))
			bType = COBBLESTONE;
		ImGui::SameLine();
		if (ImGui::Button("Dirt"))
			bType = DIRT;
		if (ImGui::Button("Door"))
			bType = WOOD_DOOR;
	}
	ImGui::End();
}