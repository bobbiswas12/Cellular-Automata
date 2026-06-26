
/* Implementation of a "Stochastic Cellular Automata", which unlike Conway's */
/* Game of life, isn't deterministic. That is the Rules governing the transition */
/* from one particular generation to the next are probabilistic. This particular */
/* implementation is based on Toom's rule wherein, each cell has a spin +1 or -1 */
/* and the transition probability Pi,j where i,j = {-1,1} = f(S(east),S(north)) */
/* where f is some function.*/

/* Copyright (C) 2026  Tanmay Rai */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/*    (at your option) any later version. */

/*    This program is distributed in the hope that it will be useful, */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/*    GNU General Public License for more details. */

/*    You should have received a copy of the GNU General Public License */
/*    along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#define screen_width 800
#define screen_height 800
#define rows 80
#define columns 80
#define fps 30

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

typedef struct{

  int spin;

}cell;

typedef struct{

  cell cellular[rows][columns];

}model;

int neighbouring_spin_count(int i, int j, model *cells) {

  int neighbouring_spin = 0;
  
  if (i == 0 && j == 0) {
    neighbouring_spin += cells->cellular[i+1][j].spin + cells->cellular[i][j+1].spin;
  }
  else if(i== 0 && j == columns - 1){
    neighbouring_spin += cells->cellular[1][columns - 1].spin + cells->cellular[0][columns - 2].spin;
  }
  else if(i == rows - 1 && j == 0){
    neighbouring_spin += cells->cellular[i-1][j].spin + cells->cellular[i][j+1].spin;
  }
  else if(i == rows - 1 && j == columns - 1){
    neighbouring_spin += cells->cellular[rows - 2][columns -1].spin + cells->cellular[rows -1][columns -2].spin;
  }
  else if(i == 0){
    neighbouring_spin += cells->cellular[0][j-1].spin + cells->cellular[1][j].spin;
  }
  else if(i == rows - 1){
    neighbouring_spin += cells->cellular[i][j-1].spin + cells->cellular[i-1][j].spin;
  }
  else if(j == 0){
    neighbouring_spin += cells->cellular[i+1][0].spin + cells->cellular[i][1].spin;
  }
  else if(j == columns - 1){
    neighbouring_spin += cells->cellular[i+1][j].spin + cells->cellular[i][j-1].spin;
  }
  else {
    neighbouring_spin += cells->cellular[i+1][j].spin + cells->cellular[i][j-1].spin;
  }
  return neighbouring_spin;
}

void update_cell(cell *some_cell, int total_spin) {

  float p = random_value(1);
  float q = random_value(1);
  
  if (total_spin > 0) {
    if (p > 0.88) {
      some_cell->spin = -1;
    }
    else{
      some_cell->spin = 1;
    }
  } else {
    if (q > 0.12) {
      some_cell->spin = 1;
    } else {
      some_cell->spin = -1;
    }
  }
}

void update_model(model *cells){

  model new = {0};
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){

      int current_spin = cells->cellular[i][j].spin;
      int total_spin = current_spin + neighbouring_spin_count(i,j,cells);
      update_cell(&new.cellular[i][j],total_spin);
    }
  }
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++) {
      cells->cellular[i][j].spin = new.cellular[i][j].spin;
    }
  }
}

void draw_cell(int i, int j, cell cell){

  if (cell.spin == 1){
    Vector2 top_left = (Vector2) {100 + (((float) screen_width-200)/columns) * j, 100 + (((float) screen_height - 200)/rows) * i};
    DrawRectangleV(top_left,(Vector2) {((float) screen_width-200)/columns,((float) screen_height - 200)/rows},BLACK);
  } else if(cell.spin == -1) {
    Vector2 top_left = (Vector2) {100 + (((float) screen_width-200)/columns) * j, 100 + (((float) screen_height - 200)/rows) * i};
    DrawRectangleV(top_left,
		   (Vector2){((float) screen_width - 200) / columns,
			     ((float) screen_height - 200) / rows},
		   WHITE);
  }

}
	
int main(){

  InitWindow(screen_width,screen_height,"window");
  SetTargetFPS(fps);
  srand(time(0));

  model cells = {0};

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {

      int spin = 0;
      float random = random_value(2);
      if (random > 0.5) {
        spin = 1;
      } else {
        spin = -1;
      }
      cells.cellular[i][j].spin = spin;
    }
  }
  
  int stage = 0;
  while (!WindowShouldClose()){

    int population = 0;
    BeginDrawing();
    ClearBackground(BLACK);

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
      for (int j = 0; j < columns; j++) {
	draw_cell(i,j,cells.cellular[i][j]);
      }
    }
    update_model(&cells);
    DrawFPS(10,10);
    DrawText(TextFormat("Gen: %d", stage),10,50,30,RED);
    stage++;
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
