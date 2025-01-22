#include <iostream>
#include <renderer/Renderer.h>

int main() {
	Renderer renderer;
	renderer.start(800, 800, "OpenGL Window");
	renderer.update();
	return 0;
}