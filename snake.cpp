#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
using namespace std;
Color green = {173, 210, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;
Sound eatSound;
Sound GameOverSound;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
			return true;
	}
	return false;
}

class Snake
{
public:
	deque<Vector2> spawn = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
	deque<Vector2> body = spawn;
	Vector2 direction = {1, 0};

	bool addSegment = false;

	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
			DrawRectangleRounded(segment, 0.5, 6, darkgreen);
		}
	}
	void update()
	{

		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
		}
	}
	void Reset()
	{
		body = spawn;
		direction = {1, 0};
	}
};

class Food
{
public:
	Vector2 position = {5, 6};

	Texture2D texture;

	Food(deque<Vector2> snakeBody)
	{

		Image image = LoadImage("resizedapple2.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}

	Vector2 generateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{x, y};
	}

	Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
	{

		Vector2 position = generateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = generateRandomCell();
		}

		return position;
	}
};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool running = true;
	int score = 0;

	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("eat.wav");
		GameOverSound = LoadSound("GameOver.wav");
	}
	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(GameOverSound);
		CloseAudioDevice();
	}
	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{
		if (running)
		{
			snake.update();
			checkCollisionWithFood();
			checkCollisionWithWall();
			checkCollisionWithTail();
		}
	}

	void checkCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score++;
			PlaySound(eatSound);
		}
	}

	void checkCollisionWithWall()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1)
		{
			PlaySound(GameOverSound);
			GameOver();
		}
	}
	void GameOver()
	{

		DrawText("Game Over", 300, 400, 75, darkgreen);
		DrawText("Press Enter to Restart", 300, 490, 30, darkgreen);
		running = false;

		if (IsKeyPressed(KEY_ENTER))
		{
			snake.Reset();
			food.position = food.GenerateRandomPos(snake.body);
			score = 0;
			running = true;
		}
	}
	void checkCollisionWithTail()
	{
		deque<Vector2> copySnakeBody = snake.body;
		copySnakeBody.pop_front();
		if (ElementInDeque(snake.body[0], copySnakeBody))
		{
			PlaySound(GameOverSound);
			GameOver();
		}
	}
};

int main()
{
	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake");
	SetTargetFPS(60);

	Game game = Game();

	while (WindowShouldClose() == false)
	{ // this loop runs 60fps
		BeginDrawing();
		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkgreen);
			DrawText("Retro Snake", offset - 5, 20, 40, darkgreen);
			DrawText(TextFormat("Score : %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkgreen);
		if (game.running)
		{
			if (eventTriggered(0.15))
			{ // to slow down the snake speed
				game.Update();
			}

			// controls
			if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
			{
				game.snake.direction = {0, -1};
			}
			if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
			{
				game.snake.direction = {0, 1};
			}
			if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
			{
				game.snake.direction = {-1, 0};
			}
			if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
			{
				game.snake.direction = {1, 0};
			}

			// drawing

			
			game.Draw();
		}
		else
		{
			game.GameOver();
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
