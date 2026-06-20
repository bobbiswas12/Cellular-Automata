
/* This is an implementation of the Conway's game of life program in C using Raylib. */
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
#define screen_height 600
#define rows 100
#define columns 100
#define fps 120

float random_value(float value){

  return (((float) rand()/(float) RAND_MAX)*(float) value);
}

typedef struct{

  bool state;

}cell;

typedef struct{

  cell cellular[rows][columns];

} model;

void update_state(cell *cellular, int count) {

  if (cellular->state == 1) {
	  if (count < 2)
	    cellular->state = 0;
	  else if (count == 2 || count == 3)
	    cellular->state = 1;
	  else
            cellular->state = 0;
  }
  else {
          if (count == 3)
            cellular->state = 1;
  }
}


model update_model(model cells){

  model new_cells = cells;
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){

      int count = 0;
      
      if (i == 0 && j == 0) {

        count += cells.cellular[i + 1][j].state +
                 cells.cellular[i][j + 1].state +
                 cells.cellular[i + 1][j + 1].state;
	
      } else if (i == 0 && j == columns - 1) {

        count += cells.cellular[i + 1][j].state +
                 cells.cellular[i + 1][j - 1].state +
                 cells.cellular[i][j - 1].state;
	
      } else if (i == rows - 1 && j == 0) {

        count += cells.cellular[i - 1][j].state +
                 cells.cellular[i - 1][j + 1].state +
                 cells.cellular[i][j + 1].state;
	
      } else if (i == rows - 1 && j == columns - 1) {

        count += cells.cellular[i - 1][j].state +
                 cells.cellular[i - 1][j - 1].state +
                 cells.cellular[i][j - 1].state;

      } else if (i == 0) {
	
        count += cells.cellular[i][j - 1].state +
                 cells.cellular[i + 1][j - 1].state +
                 cells.cellular[i + 1][j].state +
                 cells.cellular[i + 1][j + 1].state +
                 cells.cellular[i][j + 1].state;
	
      } else if (i == rows - 1) {
	
        count += cells.cellular[i - 1][j].state +
                 cells.cellular[i - 1][j - 1].state +
                 cells.cellular[i - 1][j + 1].state +
                 cells.cellular[i][j + 1].state +
                 cells.cellular[i][j - 1].state;

      } else if (j == 0) {

        count += cells.cellular[i+1][j].state +
                 cells.cellular[i + 1][j + 1].state +
                 cells.cellular[i][j + 1].state +
                 cells.cellular[i - 1][j + 1].state +
                 cells.cellular[i - 1][j].state;

      } else if (j == columns - 1) {
	
        count += cells.cellular[i - 1][j].state +
                 cells.cellular[i - 1][j - 1].state +
                 cells.cellular[i][j - 1].state +
                 cells.cellular[i + 1][j - 1].state +
                 cells.cellular[i + 1][j].state;
      }
      else {
        count += cells.cellular[i][j - 1].state +
                 cells.cellular[i + 1][j - 1].state +
                 cells.cellular[i + 1][j].state +
                 cells.cellular[i + 1][j + 1].state +
                 cells.cellular[i][j + 1].state +
                 cells.cellular[i - 1][j + 1].state +
                 cells.cellular[i - 1][j].state +
                 cells.cellular[i - 1][j - 1].state;
      }
      update_state(&new_cells.cellular[i][j], count);
    }
  }
  return new_cells;
}


int population_model(model cells){

  int population = 0;
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < columns; j++){

      if (cells.cellular[i][j].state){
	population++;
      }
    }
  }

  return population;
}

void draw_cell(int i, int j) {
  
  Vector2 top_left = (Vector2) {100 + (((float) screen_width-200)/columns) * j, 100 + ((float) (screen_height - 200)/rows) * i};
  DrawRectangleV(top_left,(Vector2) {((float) screen_width-200)/columns,((float) screen_height - 200)/rows},WHITE);

}
	
int main(){

  InitWindow(screen_width,screen_height,"window");
  SetTargetFPS(fps);
  srand(time(0));

  model cells = {0};

  int x = rows / 2;
  int y = columns / 2;

  /* /\* Die Hard *\/ */
  /* cells.cellular[x][y].state = 1; */
  /* cells.cellular[x - 1][y].state = 1; */
  /* cells.cellular[x + 4][y + 1].state = 1; */
  /* cells.cellular[x + 5][y + 1].state = 1; */
  /* cells.cellular[x + 6][y + 1].state = 1; */
  /* cells.cellular[x + 5][y - 1].state = 1; */

  /* R-Pentomino */

  cells.cellular[x][y].state = 1;
  cells.cellular[x][y - 1].state = 1;
  cells.cellular[x][y + 1].state = 1;
  cells.cellular[x - 1][y].state = 1;
  cells.cellular[x + 1][y - 1].state = 1;
  
  int stage = 0;
  while (!WindowShouldClose()){

    int population = 0;
    BeginDrawing();
    ClearBackground(BLACK);

    /* for (int i = 100; i <= screen_width - 100; i+= (screen_width-200)/columns){ */

    /*   Vector2 initial = (Vector2) {i,100}; */
    /*   Vector2 final = (Vector2) {i,screen_height - 100}; */
    /*   DrawLineV(initial,final,DARKBLUE); */

    /* } */

    /* for (int i = 100; i <= screen_height - 100; i += (screen_height - 200)/rows){ */

    /*   Vector2 initial = (Vector2) {100,i}; */
    /*   Vector2 final = (Vector2) {screen_width - 100,i}; */
    /*   DrawLineV(initial,final,DARKBLUE); */
    /* } */

    for (int i = 0; i < rows; i++){
      for (int j = 0; j < columns; j++){
	if (cells.cellular[i][j].state){
	  draw_cell(i,j);
	}
      }
    }
    population = population_model(cells);
    cells = update_model(cells);
    DrawFPS(10,10);
    DrawText(TextFormat("Gen: %d", stage),10,50,30,RED);
    DrawText(TextFormat("Population share : %f %c", (float)population*100/(rows*columns),'%'),300,50,30,RED);
    stage++;
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
