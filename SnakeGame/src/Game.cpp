#include "Game.h"

#include <iostream>
#include <ctime>

#include <glm/gtc/matrix_transform.hpp>

#include "Timer.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::run()
{
	init();
	Timer timer;

	float time = 0.0f;
	float updateTimer = 0.0f;
	float updateTick = 1.0f / 6.0f;

	unsigned int frames = 0;
	unsigned int updates = 0;

	while (!mWin->shouldClose())
	{
		mWin->pollEvents();

		if (timer.timeElapsed() - updateTimer > updateTick)
		{
			update();
			updates++;
			updateTimer += updateTick;
		}

		draw();
		mWin->swapBuffers();

		frames++;
		if (timer.timeElapsed() - time > 1.0f)
		{
			//tick();
			time += 1.0f;
			//m_FPS = frames;
			//m_UPS = updates;

			updates = 0;
			frames = 0;
		}
	}

	mWin->dispose();
}

void Game::keyCallback(Window * win, int keycode, bool released)
{
	if (!released && keycode == SDLK_w)
	{
		mDirection = DIR_UP;
	}
	else if (!released && keycode == SDLK_s)
	{
		mDirection = DIR_DOWN;
	}
	else if (!released && keycode == SDLK_a)
	{
		mDirection = DIR_LEFT;
	}
	else if (!released && keycode == SDLK_d)
	{
		mDirection = DIR_RIGHT;
	}
}

void Game::init()
{
	mWin = std::make_unique<Window>("Snake Game", WIN_SIZE, WIN_SIZE);
	mWin->setKeyCallback(std::bind(&Game::keyCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	float vertices[] =
	{
		1.0f,  1.0f,
		1.0f,  0.0f,
		0.0f,  0.0f,
		0.0f,  1.0f
	};

	unsigned int indicies[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	mShader = std::make_unique<Shader>(VSHADER_PATH, FSHADER_PATH);
	mShader->use();

	mProj = glm::ortho(0.0f, (float)WIN_SIZE, (float)WIN_SIZE, 0.0f);

	srand(time(NULL));

	mGrid.fill(0);
	mGrid[mHead] = SNAKE_SNAKE;
	mSnakeParts.push(mHead);
	
	genFood();

	mWin->setVisible(true);
}

void Game::close()
{
}

void Game::update()
{

	switch (mDirection)
	{
	case DIR_UP:
		if (mHead < GRID_SIZE)
		{
			mGameOver = true;
		}
		else
		{
			mHead -= GRID_SIZE;
		}
		break;
	case DIR_DOWN:
		if (mHead + GRID_SIZE > GRID_SIZE * GRID_SIZE)
		{
			mGameOver = true;
		}
		else
		{
			mHead += GRID_SIZE;
		}
		break;
	case DIR_LEFT:
		if (mHead % GRID_SIZE == 0)
		{
			mGameOver = true;
		}
		else
		{
			mHead--;
		}
		break;
	case DIR_RIGHT:
		if ((mHead + 1) % GRID_SIZE == 0)
		{
			mGameOver = true;
		}
		else
		{
			mHead++;
		}
		break;
	}
	
	if (mGrid[mHead] == SNAKE_SNAKE)
	{
		mGameOver = true;
	}

	if (mGameOver)
	{
		resetGame();
		return;
	}


	mSnakeParts.push(mHead);

	if (mGrid[mHead] == SNAKE_FOOD)
	{
		genFood();
	}
	else
	{
		int r = mSnakeParts.front();
		mSnakeParts.pop();
		mGrid[r] = SNAKE_NOTHING;
	}

	mGrid[mHead] = SNAKE_SNAKE;

}

void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	int offsetY = 0;
	for (int i = 0; i < GRID_SIZE; i++)
	{
		int offsetX = 0;
		for (int j = 0; j < GRID_SIZE; j++)
		{
			
			if (mGrid[i * GRID_SIZE + j] == SNAKE_FOOD)
			{
				drawQuad(glm::vec2(offsetX, offsetY), glm::vec2(SQUARE_SIZE, SQUARE_SIZE), 0.0f, COL_RED);
			}
			else if (mGrid[i * GRID_SIZE + j] == SNAKE_SNAKE)
			{
				drawQuad(glm::vec2(offsetX, offsetY), glm::vec2(SQUARE_SIZE, SQUARE_SIZE), 0.0f, COL_GREEN);
			}

			offsetX += SQUARE_SIZE + SQUARE_OFFSET;
		}
		offsetY += SQUARE_SIZE + SQUARE_OFFSET;
	}
}

void Game::drawQuad(const glm::vec2 & pos, const glm::vec2 & size, float rot, const glm::vec3 & col)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(pos.x, pos.y, 0.0f));
	model = glm::translate(model, glm::vec3(size.x / 2.0f, size.y / 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-size.x / 2.0f, -size.x / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(size.x, size.y, 1.0f));
	mShader->setMat4("mp", mProj * model);

	mShader->setVec3f("col", col);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Game::genFood()
{

	while (true)
	{
		int num = rand() % ((GRID_SIZE * GRID_SIZE) - 1);
		if (mGrid[num] == SNAKE_NOTHING)
		{
			mGrid[num] = SNAKE_FOOD;
			break;
		}
	}
}

void Game::resetGame()
{
	mGrid.fill(0);
	while (!mSnakeParts.empty()) mSnakeParts.pop();
	mHead = (GRID_SIZE / 2) * GRID_SIZE + (GRID_SIZE / 2);
	mDirection = DIR_RIGHT;
	mGrid[mHead] = SNAKE_SNAKE;
	mSnakeParts.push(mHead);
	srand(time(NULL));
	genFood();
	mGameOver = false;
}



///////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	Game game;
	game.run();

	return 0;
}
