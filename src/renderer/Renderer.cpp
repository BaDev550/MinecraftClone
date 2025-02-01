#include <renderer/Renderer.h>

float lastX;
float lastY;
bool firstClick = true;
bool hit = false;

double mouseX, mouseY;
glm::vec3 hitPosition;

Camera camera;
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
	texture_atlas.setTexture("textures/stay_true.png");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");

	world.generateWorld(chunkSize, chunkSize);
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

		if (!world.w_chunks.empty()) {
			render();
		}
		drawDebugGUI();

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

	glm::mat4 projection = glm::perspective(glm::radians(90.f), (float)wfb_Width / (float)wfb_Height, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 vp = projection * view;

	frustum.updateFrustumPlanes(vp);
	world.updateChunksVisibility(frustum);

	core_shader.setMat4("projection", projection);
	core_shader.setMat4("view", view);
	core_shader.setVec3("cameraPos", camera.Position);

	raycaster.init(projection, view);
	glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);
	float closestT = std::numeric_limits<float>::max();
	Ray ray = raycaster.castFromCamera(static_cast<float>(mouseX), static_cast<float>(mouseY), window.getFBWidth(), window.getFBHeight(), camera);
	float maxDistance = 100.0f;

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	world.renderNearChunks(camera.Position, texture_atlas, core_shader, viewDistance);

	for (const auto& block : world.current_chunk->blocks) {
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

	if (!world.w_chunks.empty()) {
		if (window.getInput().isMouseButtonClicked(GLFW_MOUSE_BUTTON_RIGHT)) {
			if (hit) {
				glm::ivec3 chunkCoords = glm::floor(hitPosition);

				world.current_chunk->addBlock(chunkCoords.x, chunkCoords.y, chunkCoords.z, bType, camera.Position, camera.Front);
			}
		}

		if (window.getInput().isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT)) {
			if (hit) {
				glm::ivec3 chunkCoords = glm::floor(hitPosition);

				world.current_chunk->removeBlock(chunkCoords.x, chunkCoords.y, chunkCoords.z);
			}
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
	if (ImGui::Begin("Debugger") && !world.w_chunks.empty()) {
		ImGui::SetWindowSize(ImVec2(300, 400));
		ImGui::SetWindowPos(ImVec2(1.5f, ((window.getFBHeight() * -1.0f) / 200.f) + 1.5f ));

		frameCounter++;
		if (frameCounter >= 30) {
			FPS = floor(1 /deltaTime);
			frameCounter = 0;
		}
		ImGui::Text("FPS: %d", FPS);

		int activeChunks{0};
		for (int x = 0; x < 8; x++) {
			for (int z = 0; z < 8; z++) {
				if (world.w_chunks[x][z].bVisibleOnScreen)
					activeChunks++;
			}
		}
		ImGui::Text("Active chunks: %d", activeChunks);
		ImGui::Text("Blocks rendered: %d", (int)world.current_chunk->blocks.size());
		if (ImGui::InputInt("View Distance", &viewDistance));

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

		if (ImGui::Button("Clear World")) {
			world.w_chunks.clear();
		}

		ImGui::Text("SHIFT+M To cursor mode");
		ImGui::Text("CTRL+M To game mode");

		ImGui::Text("SHIFT+F To wireframe mode");
		ImGui::Text("CTRL+F To fill mode");
	}
	else if (world.w_chunks.empty()) {
		if (ImGui::InputInt("Chunk size", &chunkSize));
		if (ImGui::InputInt("Seed", &world.seed));
		if (ImGui::Button("Generate World")) {
			world.generateWorld(chunkSize, chunkSize);
		}
	}
	ImGui::End();
}