#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define screen_width 800
#define screen_height 800
#define rows 120
#define columns 120
#define fps 10

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

void draw_grid(int no_of_rows,int no_of_columns,int screenheight,int screenwidth){

  for (int i = 100; i <= screenwidth - 100; i+= (screenwidth-200)/no_of_columns){

      Vector2 initial = (Vector2) {i,100};
      Vector2 final = (Vector2) {i,screenheight - 100};
      DrawLineV(initial,final,WHITE);

    }

    for (int i = 100; i <= screenheight - 100; i += (screenheight - 200)/no_of_rows){

      Vector2 initial = (Vector2) {100,i};
      Vector2 final = (Vector2) {screenwidth - 100,i};
      DrawLineV(initial,final,WHITE);
    }
}
typedef struct{

  float prob;
  bool state;

}cell;

typedef struct{

  cell cellular[rows][columns];

}model;

model initial_model(model cells,int column){

  for (int i = 0; i < column; i++){
    float k = random_value(1);
    if (k >= 0.5){
      cells.cellular[0][i].state = true;
    }
  }
  return cells;
}

bool transition(int left, int center, int right,int rule[8]){

  
  if (left == 1 && center == 1 && right == 1){
    return rule[0];
  }
  else if(left == 1 && center == 1 && right == 0){
    return rule[1];
  }
  else if(left == 1 && center == 0 && right == 1){
    return rule[2];
  }
  else if(left == 1 && center == 0 && right == 0){
    return rule[3];
  }
  else if(left == 0 && center == 1 && right == 1){
    return rule[4];
  }
  else if(left == 0 && center == 1 && right == 0){
    return rule[5];
  }
  else if(left == 0 && center == 0 && right == 1){
    return rule[6];
  }
  else{
    return rule[7];
  }
}
void draw_model(model cells){

  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){
      if (cells.cellular[i][j].state){
	Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},WHITE);
      }
    }
  }
}

model update_model(model cells){

  model new = {0};
  int rule[8] = {0,1,0,0,0,0,1,1};
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){
      if (j == 0 || j == columns - 1){
	new.cellular[i][j].state = false;
      }
      else{
	new.cellular[i][j].state = transition((int) cells.cellular[i-1][j-1].state,(int) cells.cellular[i-1][j].state, (int) cells.cellular[i-1][j+1].state,rule);
      }
    }
  }
  return new;
}
int main(){

  InitWindow(screen_width,screen_height,"window");
  SetTargetFPS(fps);
  srand(time(0));
  model cells = {0};
  cells = initial_model(cells,columns);
  int counter = 0;
  while (!WindowShouldClose()){

    BeginDrawing();
    ClearBackground(BLACK);
    counter ++;
    /*draw_grid(rows,columns,screen_height,screen_width);*/
    cells = update_model(cells);
    draw_model(cells);
    DrawFPS(10,10);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
