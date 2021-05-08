#pragma once

#include <memory>
#include <array>
#include <queue>
#include <glm/mat4x4.hpp>

#include "Window.h"
#include "Shader.h"

namespace
{
	const int SQUARE_SIZE = 20;
	const int SQUARE_OFFSET = 2;
	const int GRID_SIZE = 20;
	const int WIN_SIZE = SQUARE_SIZE * GRID_SIZE + SQUARE_OFFSET * GRID_SIZE;

	const int SNAKE_FOOD = 1;
	const int SNAKE_SNAKE = 2;
	const int SNAKE_NOTHING = 0;

	const int DIR_LEFT = 1;
	const int DIR_RIGHT = 2;
	const int DIR_UP = 3;
	const int DIR_DOWN = 4;

	const glm::vec3 COL_DARK_GREEN(0.0f, 0.8f, 0.0f);
	const glm::vec3 COL_GREEN(0.0f, 1.0f, 0.0f);
	const glm::vec3 COL_RED(1.0f, 0.0f, 0.0f);

	const std::string SHADER_PATH = "Shader path";
	const std::string VSHADER_PATH = SHADER_PATH + "Shader.vert";
	const std::string FSHADER_PATH = SHADER_PATH + "Shader.frag";
}

class Game
{
public:
	Game();
	~Game();

	void run();
	void keyCallback(Window* win, int keycode, bool released);
private:

	void init();
	void close();
	void draw();
	void update();
	void drawQuad(const glm::vec2& pos, const glm::vec2& size, float rot, const glm::vec3& col);
	void genFood();
	void resetGame();
	
	std::unique_ptr<Window> mWin;
	std::unique_ptr<Shader> mShader;
	glm::mat4 mProj;

	std::array<int, GRID_SIZE * GRID_SIZE> mGrid;
	std::queue<int> mSnakeParts;
	int mHead = (GRID_SIZE / 2) * GRID_SIZE + (GRID_SIZE / 2);
	int mDirection = DIR_RIGHT;
	bool mGameOver = false;
};

