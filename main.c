#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define screen_width 800
#define screen_height 600
#define rows 10
#define columns 10


float random_value(int value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

typedef struct{

  float prob;
  bool state;

}cell;

typedef struct{

  cell cellular[rows][columns];

}model;

model update_model(model cells){

  model new;
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){
      if (i == 0 && j == 0){
	new.cellular[i][j].prob = 1 - cells.cellular[i+1][j].prob * cells.cellular[i][j+1].prob;
      }
      else if(i== 0 && j == columns - 1){
	new.cellular[0][columns - 1].prob = 1 - cells.cellular[1][columns - 1].prob*cells.cellular[0][columns - 2].prob;
      }
      else if(i == 9 && j == 9){
	new.cellular[9][9].prob = 1 - cells.cellular[8][9].prob*cells.cellular[9][8].prob;
      }
      else if(i == 0){
	new.cellular[0][j].prob = 1 - cells.cellular[0][j-1].prob*cells.cellular[0][j+1].prob*cells.cellular[1][j].prob;
      }
      else if(j == 0){
	new.cellular[i][0].prob = 1 - cells.cellular[i-1][0].prob*cells.cellular[i+1][0].prob*cells.cellular[i][1].prob;
      }
      else {
	new.cellular[i][j].prob = 1 - cells.cellular[i-1][j].prob*cells.cellular[i+1][j].prob*cells.cellular[i][j-1].prob*cells.cellular[i][j+1].prob;
      }
    }
  }
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){
      new.cellular[i][j].state = cells.cellular[i][j].state;
    }
  }
  return new;
}
	
int main(){

  InitWindow(screen_width,screen_height,"window");
  SetTargetFPS(1);
  srand(time(0));

  model cells = {0};

  int x = (int) random_value(10);
  int y = (int) random_value(10);
  

  for (int i = 0; i < x; i++){
    for (int j = 0; j < x; j++){
      
      int i = (int) random_value(rows);
      int j = (int) random_value(columns);

    cells.cellular[i][j].prob = random_value(1);
    cells.cellular[i][j].state = true;
    }
  }
  
  
  while (!WindowShouldClose()){

    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 100; i <= 700; i+= 60){

      Vector2 initial = (Vector2) {i,100};
      Vector2 final = (Vector2) {i,500};
      DrawLineV(initial,final,WHITE);

    }

    for (int i = 100; i <= 500; i +=40){

      Vector2 initial = (Vector2) {100,i};
      Vector2 final = (Vector2) {700,i};
      DrawLineV(initial,final,WHITE);
    }

    for (int i = 0; i < 10; i++){
      for (int j = 0; j < 10; j++){
	if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.0003){
	  Vector2 top_left = (Vector2) {100 + 60 * i, 100 + 40 * j};
	  DrawRectangleV(top_left,(Vector2) {60,40},GREEN);
	}
      }
    }
    cells = update_model(cells);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
