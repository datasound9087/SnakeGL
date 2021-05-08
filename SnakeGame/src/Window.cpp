#include "Window.h"

#include <glad/glad.h>

Window::Window(std::string name, int width, int height, bool vsync)
	: m_Name(name), m_Width(width), m_Height(height), m_Vsync(vsync)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Error initialising window");
	}

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	m_Win = SDL_CreateWindow(m_Name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);

	m_Context = SDL_GL_CreateContext(m_Win);

	setVsync(m_Vsync);

	gladLoadGLLoader(SDL_GL_GetProcAddress);
	
	int w, h;
	SDL_GetWindowSize(m_Win, &w, &h);
	glViewport(0, 0, w, h);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Window::~Window()
{
	dispose();
}

void Window::dispose()
{
	m_ShouldClose = true;
	setVisible(false);
	SDL_GL_DeleteContext(m_Context);
	SDL_DestroyWindow(m_Win);
	SDL_Quit();
}

void Window::setVisible(bool visible)
{
	m_Visible = visible;

	if (m_Visible)
	{
		SDL_ShowWindow(m_Win);
	}
	else
	{
		SDL_HideWindow(m_Win);
	}
}

void Window::setVsync(bool swap)
{
	if (swap)
	{
		if (SDL_GL_SetSwapInterval(-1) < 0)
		{
			SDL_GL_SetSwapInterval(1);
		}
	}
	else
	{
		SDL_GL_SetSwapInterval(0);
	}
}

void Window::pollEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:

			m_ShouldClose = true;
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:

			if (m_KeyCallack != NULL)
			{
				m_KeyCallack(this, event.key.keysym.sym, event.key.state == SDL_RELEASED ? true : false);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			if (m_MouseButtonCallback != NULL)
			{
				m_MouseButtonCallback(this, event.button.button, event.button.state == SDL_RELEASED ? true : false);
			}
			break;

		case SDL_MOUSEMOTION:

			if (m_MouseMotionCallback != NULL)
			{
				m_MouseMotionCallback(this, event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			}
			break;

		default:
			break;
		}
	}
}

bool Window::shouldClose()
{
	return m_ShouldClose;
}

void Window::setKeyCallback(const KeyFunction &func)
{
	m_KeyCallack = func;
}

void Window::setMouseButtonCallback(const MouseButtonFunction &func)
{
	m_MouseButtonCallback = func;
}

void Window::setMouseMotionCallback(const MouseMotionFunction &func)
{
	m_MouseMotionCallback = func;
}

void Window::swapBuffers()
{
	SDL_GL_SwapWindow(m_Win);
}

SDL_Window* Window::getSDLWindow() const
{
	return m_Win;
}
