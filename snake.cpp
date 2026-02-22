#include<deque>
#include<iostream>
#include<raylib.h>
#include<raymath.h>
using namespace std;
Color green={173,210,96,255};
Color darkgreen={43,51,24,255};

int cellSize=30;
int cellCount=25;

double lastUpdateTime=0;

bool eventTriggered(double interval){
	double currentTime=GetTime();
	if(currentTime-lastUpdateTime>=interval){
		lastUpdateTime=currentTime;
		return true;
	}
	return false;
}

class Snake{
	public:
		deque<Vector2>body={Vector2{6,9},Vector2{5,9},Vector2{4,9}};
		Vector2 direction={1,0};
		void Draw(){
			for(unsigned i=0;i<body.size();i++){
				float x=body[i].x;
				float y=body[i].y;
				Rectangle segment=Rectangle{x*cellSize,y*cellSize,(float)cellSize,(float)cellSize};
				DrawRectangleRounded(segment,0.5,6, darkgreen);
			}
		}
		void update(){
			body.pop_back();
			body.push_front(Vector2Add(body[0],direction));
		}
};

class Food{
public:
	Vector2 position={5,6};
	
	Texture2D texture;
	
	Food(){
	
		Image image=LoadImage("resizedapple2.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos();
	}
	
	~Food(){
		UnloadTexture(texture);
	}
	
	void Draw(){
		DrawTexture(texture, position.x*cellSize, position.y*cellSize, WHITE);
	}
	
	Vector2 GenerateRandomPos(){
		float x=GetRandomValue(0,cellCount-1);
		float y=GetRandomValue(0,cellCount-1);
		return Vector2{x,y};
	}

};


int main(){
	InitWindow(cellSize*cellCount,cellSize*cellCount,"Snake");
	SetTargetFPS(60);
	
	Food food=Food();
	Snake snake=Snake();
	
	while(WindowShouldClose()==false){
		BeginDrawing();
		if(eventTriggered(0.2)){//to slow down the snake speed
			snake.update();
		}

		//controls
		if(IsKeyPressed(KEY_UP) && snake.direction.y!=1){
			snake.direction={0,-1};
		}
		if(IsKeyPressed(KEY_DOWN)&& snake.direction.y!=-1){
			snake.direction={0,1};
		}
		if(IsKeyPressed(KEY_LEFT)&& snake.direction.x!=1){
			snake.direction={-1,0};
		}
		if(IsKeyPressed(KEY_RIGHT)&& snake.direction.x!=-1){
			snake.direction={1,0};
		}




		//drawing
		ClearBackground(green);
		food.Draw();
		snake.Draw();
		
		EndDrawing();
	
	}
	
	CloseWindow();
	return 0;
}
