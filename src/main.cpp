#include <iostream>
#include <renderer/Renderer.h>

int main() {
	Renderer renderer;
	renderer.start(1000, 1000, "OpenGL Window");
	renderer.update();
	return 0;
}