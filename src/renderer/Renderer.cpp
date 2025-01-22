#include <renderer/Renderer.h>

Renderer::~Renderer()
{
}

void Renderer::start(unsigned int width, unsigned int height, const char* title)
{
	this->w_Width = width;
	this->w_Height = height;
	this->w_Title = title;
	window.CreateWindow(w_Width, w_Height, w_Title.c_str());
}

void Renderer::update()
{
	while(!window.ShouldClose()){
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

}

void Renderer::processInput()
{
	window.processInput();
}
