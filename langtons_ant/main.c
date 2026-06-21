
/* Langton's Ant Implementation in C */

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
#include <stdlib.h>
#include <time.h>

#define screen_width 800
#define screen_height 600
#define rows 200
#define columns 200
#define fps 120

float random_value(float value) {

  return (((float)rand() / (float)RAND_MAX) * (float)value);
}

typedef struct {

  int direction; /* 0 - left, 1 - up, 2 - right, 3 - bottom */
  Vector2 pos;

} Ant;

typedef struct {

  bool state;

} cell;

typedef struct {

  cell cellular[rows][columns];
  Ant ant;

} model;

void update_state(cell *cellular) {

  if (cellular->state == 1)
    cellular->state = 0;
  else
    cellular->state = 1;
}

void update_ant_pos(Ant *ant) {

  if (ant->direction == 0) {
    ant->pos.x -= 1;
  } else if (ant->direction == 1) {
    ant->pos.y -= 1;
  } else if (ant->direction == 2) {
    ant->pos.x += 1;
  } else {
    ant->pos.y += 1;
  }
}

void update_model(model *cells) {

  int i = cells->ant.pos.x;
  int j = cells->ant.pos.y;

  if (i == 0 && j == 0) {

    if (cells->cellular[i][j].state == 1 && (cells->ant.direction == 3 ||
					    cells->ant.direction == 0)) {
      cells->ant.direction = 2;
    } else if (cells->cellular[i][j].state == 0 && (cells->ant.direction == 2 ||
						   cells->ant.direction == 1)) {
      cells->ant.direction = 3;
    }

  } else if (i == 0 && j == columns - 1) {

    if (cells->cellular[i][j].state == 1 && (cells->ant.direction == 2 ||
					    cells->ant.direction == 3)) {
      cells->ant.direction = 1;
    } else if (cells->cellular[i][j].state == 0 && (cells->ant.direction == 1 ||
						   cells->ant.direction == 0)) {
      cells->ant.direction = 2;
    }

  } else if (i == rows - 1 && j == 0) {

    if (cells->cellular[i][j].state == 1 && (cells->ant.direction == 0 ||
					    cells->ant.direction == 1)) {
      cells->ant.direction = 3;
    } else if (cells->cellular[i][j].state == 0 && (cells->ant.direction == 3 ||
						   cells->ant.direction == 2)) {
      cells->ant.direction = 0;
    }
  } else if (i == rows - 1 && j == columns - 1) {

    if (cells->cellular[i][j].state == 1 && (cells->ant.direction == 1 ||
					    cells->ant.direction == 2)) {
      cells->ant.direction = 0;
    } else if (cells->cellular[i][j].state == 0 && (cells->ant.direction == 0 ||
						   cells->ant.direction == 3)) {
      cells->ant.direction = 1;
    }
  } else if (i == 0) {

    if (cells->cellular[i][j].state == 1 && cells->ant.direction == 3) {
      cells->ant.direction = 2;
    } else if (cells->cellular[i][j].state == 0 && cells->ant.direction == 1) {
      cells->ant.direction = 2;
    }
  } else if (i == rows - 1) {

    if (cells->cellular[i][j].state == 1 && cells->ant.direction == 1) {
      cells->ant.direction = 0;
    } else if (cells->cellular[i][j].state == 0 && cells->ant.direction == 3) {
      cells->ant.direction = 0;
    }
  } else if (j == 0) {

    if (cells->cellular[i][j].state == 1 && cells->ant.direction == 0) {
      cells->ant.direction = 3;
    } else if (cells->cellular[i][j].state == 0 && cells->ant.direction == 2) {
      cells->ant.direction = 3;
    }
  } else if (j == columns - 1) {

    if (cells->cellular[i][j].state == 1 && cells->ant.direction == 2) {
      cells->ant.direction = 1;
    } else if (cells->cellular[i][j].state == 0 && cells->ant.direction == 0) {
      cells->ant.direction = 1;
    }
  } else {
    if (cells->cellular[i][j].state == 1) {
      cells->ant.direction += 1;
      cells->ant.direction %= 4;
    } else {
      cells->ant.direction += 3;
      cells->ant.direction %= 4;
    }
  }
  update_state(&cells->cellular[i][j]);
  update_ant_pos(&cells->ant);
}

void draw_cell(int i, int j, Color color) {

  Vector2 top_left =
      (Vector2){100 + (((float)screen_width - 200) / columns) * j,
                100 + ((float)(screen_height - 200) / rows) * i};
  DrawRectangleV(top_left,
                 (Vector2){((float)screen_width - 200) / columns,
                           ((float)screen_height - 200) / rows},
                 color);
}

int main() {

  InitWindow(screen_width, screen_height, "window");
  SetTargetFPS(fps);
  srand(time(0));

  int x = rows / 2;
  int y = columns / 2;
  Ant ant;
  ant.pos = (Vector2){x, y};
  ant.direction = 2;
  model cells = {0};
  cells.ant = ant;

  int stage = 0;
  while (!WindowShouldClose()) {

    int population = 0;
    BeginDrawing();
    ClearBackground(BLACK);
    
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (cells.cellular[i][j].state == 0) {
          draw_cell(i, j, BLACK);
        } else
          draw_cell(i, j, WHITE);
      }
    }
    draw_cell(cells.ant.pos.x,cells.ant.pos.y,RED);
    update_model(&cells);
    DrawFPS(10, 10);
    DrawText(TextFormat("Gen: %d", stage), 10, 50, 20, RED);
    stage++;
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
