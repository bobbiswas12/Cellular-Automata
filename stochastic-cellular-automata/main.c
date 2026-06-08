#include <stdio.h>
#include "raylib.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define screen_width 800
#define screen_height 800
#define rows 80
#define columns 80
#define fps 10

float random_value(float value){

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

  model new = {0};
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){
      
      if (i == 0 && j == 0){
	new.cellular[i][j].prob = (cells.cellular[i+1][j].prob + cells.cellular[i][j+1].prob)/2;
      }
      else if(i== 0 && j == columns - 1){
	new.cellular[0][columns - 1].prob = (cells.cellular[1][columns - 1].prob*cells.cellular[0][columns - 2].prob)/2;
      }
       else if(i == rows - 1 && j == 0){
	 new.cellular[i][j].prob = (cells.cellular[i-1][j].prob * cells.cellular[i][j+1].prob)/2;
      }
      else if(i == rows - 1 && j == columns - 1){
	new.cellular[rows - 1][columns - 1].prob = (cells.cellular[rows - 2][columns -1].prob*cells.cellular[rows -1][columns -2].prob)/2;
      }
      else if(i == 0){
	new.cellular[0][j].prob = (cells.cellular[0][j-1].prob*cells.cellular[0][j+1].prob*cells.cellular[1][j].prob)/3;
      }
      else if(i == rows - 1){
	new.cellular[i][j].prob = (cells.cellular[i][j-1].prob * cells.cellular[i][j+1].prob * cells.cellular[i-1][j].prob)/3;
      }
      else if(j == 0){
	new.cellular[i][0].prob = (cells.cellular[i-1][0].prob*cells.cellular[i+1][0].prob*cells.cellular[i][1].prob)/3;
      }
      else if(j == columns - 1){
	new.cellular[i][j].prob = (cells.cellular[i-1][j].prob * cells.cellular[i+1][j].prob * cells.cellular[i][j-1].prob)/3;
      }
      else {
	new.cellular[i][j].prob = (cells.cellular[i-1][j].prob*cells.cellular[i+1][j].prob*cells.cellular[i][j-1].prob*cells.cellular[i][j+1].prob)/4;
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

model noise(model cells,float noise_volume){

  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){

      cells.cellular[i][j].prob += random_value(noise_volume) - noise_volume/2;
      if (cells.cellular[i][j].prob < 0){
	cells.cellular[i][j].prob = 0;
      }
      else if(cells.cellular[i][j].prob > 1){
	cells.cellular[i][j].prob = 1;
      }
    }
  }

  return cells;
}

int population_model(model cells){

  int population = 0;
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){

      if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.0001){
	population++;
      }
    }
  }

  return population;
}
	
int main(){

  InitWindow(screen_width,screen_height,"window");
  SetTargetFPS(fps);
  srand(time(0));

  model cells = {0};

  int x = (int) random_value(rows);
  int y = (int) random_value(columns);
  

  for (int i = 0; i < x; i++){
    for (int j = 0; j < y; j++){
      
      int i = (int) random_value(rows);
      int j = (int) random_value(columns);

    cells.cellular[i][j].prob = random_value(1);
    cells.cellular[i][j].state = true;
    }
  }
  
  int stage = 0;
  while (!WindowShouldClose()){

    int population = 0;
    BeginDrawing();
    ClearBackground(WHITE);

    /* for (int i = 100; i <= screen_width - 100; i+= (screen_width-200)/columns){ */

    /*   Vector2 initial = (Vector2) {i,100}; */
    /*   Vector2 final = (Vector2) {i,screen_height - 100}; */
    /*   DrawLineV(initial,final,WHITE); */

    /* } */

    /* for (int i = 100; i <= screen_height - 100; i += (screen_height - 200)/rows){ */

    /*   Vector2 initial = (Vector2) {100,i}; */
    /*   Vector2 final = (Vector2) {screen_width - 100,i}; */
    /*   DrawLineV(initial,final,WHITE); */
    /* } */

    for (int i = 0; i < rows; i++){
      for (int j = 0; j < columns; j++){
	if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.2){
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},MAROON);
	}
	else if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.15){
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},RED);
	}
	else if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.10){
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},ORANGE);
	}
	else if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.05){
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},GOLD);
	}
	else if (cells.cellular[i][j].state && cells.cellular[i][j].prob >= 0.001){
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},YELLOW);
	}
	else{
	  Vector2 top_left = (Vector2) {100 + ((screen_width-200)/columns) * j, 100 + ((screen_height - 200)/rows) * i};
	  DrawRectangleV(top_left,(Vector2) {(screen_width-200)/columns,(screen_height - 200)/rows},BLACK);
	}
      }
    }
    population = population_model(cells);
    cells = update_model(cells);
    cells = noise(cells,1);
    DrawFPS(10,10);
    DrawText(TextFormat("Gen: %d", stage),10,50,30,RED);
    DrawText(TextFormat("Population share : %f %c", (float)population*100/(rows*columns),'%'),300,50,30,RED);
    stage++;
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
