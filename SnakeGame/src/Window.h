#pragma once

#include <string>
#include <functional>
#include <SDL.h>

class Window
{
private:

	int m_Width;
	int m_Height;
	
	std::string m_Name;

	//void key_callback(window* win, int keycode, bool released)
	typedef std::function<void(Window*, int, bool)> KeyFunction;
	KeyFunction m_KeyCallack = NULL;

	//void mousebutton_callback(windo* win, int keycode, bool relreased)
	typedef std::function<void(Window*, int, bool)> MouseButtonFunction;
	MouseButtonFunction m_MouseButtonCallback = NULL;

	//void mouseMoved_callback(window* win, int x, int y)
	typedef std::function<void(Window*, int, int, int, int)> MouseMotionFunction;
	MouseMotionFunction m_MouseMotionCallback = NULL;

	bool m_Visible = false;
	bool m_Vsync = true;
	bool m_ShouldClose = false;

	SDL_Window* m_Win;
	SDL_GLContext m_Context;

public:
	Window(std::string name, int width, int height, bool vsync = true);
	~Window();

	void dispose();
	void setVisible(bool visible);
	void setVsync(bool swap);

	void pollEvents();

	bool shouldClose();

	void setKeyCallback(const KeyFunction &func);

	void setMouseButtonCallback(const MouseButtonFunction &func);

	void setMouseMotionCallback(const MouseMotionFunction &func);

	void swapBuffers();

	SDL_Window* getSDLWindow() const;
};

