/*
  Rules of the game: 
  (source) https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
  
  Transitions uniquely depends on the state of the neighbours cells in that generation 

  - Any live cell with fewer than 2 live neighbours dies, as if by underpopulation. 
  - Any live cell with 2 or 3 live neighbours lives on to the next generation.
  - Any live cell with more than 3 live neighbours dies, as if by overpopulation.
  - any dead cell with exactly 3 live neigbours becomes a live cell, as if by reproduction. 
*/
#include <stdio.h>
#include <unistd.h>

#define COLS 70
#define ROWS 30
char *ALIVE= "■";
// #define ALIVE '*'
#define DEAD ' '
#define DEBUG 0 
#define AUTO 1 

void init(int *grid);
void setup(int *grid);
void update(int *grid);
void draw(int *grid);
void clear();

int cellindex(int x, int y);
void setcell(int *grid, int x, int y, int state);
int getcell(int *grid, int x, int y);
int count_alive_neighbors(int *grid, int x, int y);
void printgrid(int *grid);

int main(void) {
  int grid[COLS * ROWS];
  // Initial Grid initialize and draw
  init(grid);
  setup(grid);
  draw(grid);  

  if (AUTO){
    // Automatically update
    while(1){
      usleep(30000);
      update(grid);
      draw(grid);
    }
  }
  else{
    // Update when pressing Enter key
    char ch;
    while ((ch = getchar()) == 10) {
      update(grid);
      draw(grid);
    }
  }
  return 0;
}

/**
  Map an (x,y) position to a grid index to be used in  
  grid[index].
  Any x or y can be provided, they will be converted in the 
  grid coorinates using modular arithmetic  
*/
int cellindex(int x, int y){
  x = (x+COLS)%COLS; 
  y = (y+ROWS)%ROWS;
  return y * COLS + x;
}

/*
  Set the state of a given (x,y) cell in the grid
*/
void setcell(int *grid, int x, int y, int state) { 
  grid[cellindex(x,y)] = state; 
}

/*
  Get the state of a given (x,y) cell in the grid
*/
int getcell(int *grid, int x, int y) { 
  return grid[cellindex(x,y)]; 
}

/*
  Initialize the grid to all zeros.
*/
void init(int *grid) {
  for (size_t i = 0; i < COLS * ROWS; i++) grid[i] = 0;
}

/*
  Clear the terminal
 */
void clear() { printf("\e[1;1H\e[2J\n"); }

/*
  Copies the content of a source grid to a dest grid
*/
void copygrid(int *source, int *dest) {
  for (size_t i = 0; i < ROWS*COLS; i++) dest[i] = source[i];
}

/*
  There are 8 neighbors for each cell in the grid. 
  The cells that are on the edge will look at the other side
  of the grid and consider these cells as neighbours.
  We can represent C as the middle cell and 
  n(north) s(south) e(east) w(west) nw(North-West) ne(North-East) sw(South-West) se(South-East)
  as the neighbour cells. 

                      nw  n  ne
                       w  C  e 
                      sw  s  se 
*/
int count_alive_neighbors(int *grid, int x, int y) { 
  int nw = getcell(grid, x-1 , y-1);
  int n = getcell(grid, x, y-1);
  int ne = getcell(grid, x+1, y-1);
  int w= getcell(grid, x-1, y);
  int e = getcell(grid, x+1, y);
  int sw = getcell(grid, x-1, y+1);
  int s = getcell(grid, x, y+1);
  int se= getcell(grid, x+1, y+1);

  return nw + n + ne + w + e + sw + s + se;
}

/*
  Update the grid in agreement with the rules of the Game.
*/
void update(int *grid) {
  int gridcpy[COLS*ROWS];
  copygrid(grid, gridcpy);

  for (int y = 0; y < ROWS; y++) {
    for (int x = 0; x < COLS; x++) {
      int alive_neighbors = count_alive_neighbors(grid, x, y);
      if (getcell(grid, x, y)) {
        // the cell is ALIVE 
        if (alive_neighbors < 2 || alive_neighbors > 3) {
          // dies 
          setcell(gridcpy, x, y, 0);
        }
      } else {
        // the cell is DEAD 
        if (alive_neighbors == 3) {
          // revives 
          setcell(gridcpy, x, y, 1);
        }
      }
    }
  }

  copygrid(gridcpy, grid);
}

/*
  Just draw the grid.
  The grid is composed of ALIVE and DEAD cells.
  When DEBUG is active, the printgrid function will be used to 
  print the grid instead of the symbols ALIVE and DEAD 
 */
void draw(int *grid) {
  clear();
  if (DEBUG) {
    printgrid(grid);
    return;
  }

  for (size_t y = 0; y < ROWS; y++) {
    for (size_t x = 0; x < COLS; x++) {
      if (grid[cellindex(x,y)] == 1) {
        printf("%s ", ALIVE);
      } else {
        printf("%c ", DEAD);
      }
    }
    printf("\n");
  }
}

/*
  Just print the grid with zeros (DEAD cells) and ones (ALIVE cells)
 */
void printgrid(int *grid){
  for (size_t y = 0; y < ROWS; y++) {
    for (size_t x = 0; x < COLS; x++){
      printf("%d ",  getcell(grid, x, y));
    }
    printf("\n");
  }
}

/*
  Create a setup for the cells to start the simulation
 */
void setup(int *grid){
  // 45 * 19 119P4H1V0
  // Todo: read from a file and use the standard notation to make the setup at runtime
  grid[cellindex(20,15)] = 1;
  grid[cellindex(21, 13)] = 1;
  grid[cellindex(21, 14)] = 1;
  grid[cellindex(21, 16)] = 1;
  grid[cellindex(21, 17)] = 1;
  
  grid[cellindex(24, 12)] = 1;
  grid[cellindex(24, 13)] = 1;
  grid[cellindex(24, 14)] = 1;
  grid[cellindex(24, 16)] = 1;
  grid[cellindex(24, 17)] = 1;
  grid[cellindex(24, 18)] = 1;

  grid[cellindex(25, 12)] = 1;
  grid[cellindex(25, 13)] = 1;
  grid[cellindex(25, 17)] = 1;
  grid[cellindex(25, 18)] = 1;

  grid[cellindex(26, 8)] = 1;
  grid[cellindex(26, 9)] = 1;
  grid[cellindex(26, 10)] = 1;
  grid[cellindex(26, 20)] = 1;
  grid[cellindex(26, 21)] = 1;
  grid[cellindex(26, 22)] = 1;

  grid[cellindex(28, 8)] = 1;
  grid[cellindex(28, 10)] = 1;
  grid[cellindex(28, 20)] = 1;
  grid[cellindex(28, 22)] = 1;

  grid[cellindex(29, 10)] = 1;
  grid[cellindex(29, 11)] = 1;
  grid[cellindex(29, 19)] = 1;
  grid[cellindex(29, 20)] = 1;

  grid[cellindex(30, 10)] = 1;
  grid[cellindex(30, 20)] = 1;

  grid[cellindex(31, 9)] = 1;
  grid[cellindex(31, 10)] = 1;
  grid[cellindex(31, 20)] = 1;
  grid[cellindex(31, 21)] = 1;

  grid[cellindex(32, 10)] = 1;
  grid[cellindex(32, 20)] = 1;

  grid[cellindex(33, 10)] = 1;
  grid[cellindex(33, 13)] = 1;
  grid[cellindex(33, 17)] = 1;
  grid[cellindex(33, 20)] = 1;

  grid[cellindex(34, 10)] = 1;
  grid[cellindex(34, 13)] = 1;
  grid[cellindex(34, 17)] = 1;
  grid[cellindex(34, 20)] = 1;

  grid[cellindex(35, 8)] = 1;
  grid[cellindex(35, 10)] = 1;
  grid[cellindex(35, 11)] = 1;
  grid[cellindex(35, 19)] = 1;
  grid[cellindex(35, 20)] = 1;
  grid[cellindex(35, 22)] = 1;

  grid[cellindex(36, 7)] = 1;
  grid[cellindex(36, 9)] = 1;
  grid[cellindex(36, 21)] = 1;
  grid[cellindex(36, 23)] = 1;

  grid[cellindex(38, 9)] = 1;
  grid[cellindex(38, 21)] = 1;

  grid[cellindex(39, 9)] = 1;
  grid[cellindex(39, 21)] = 1;

  grid[cellindex(40, 9)] = 1;
  grid[cellindex(40, 21)] = 1;

  grid[cellindex(41, 8)] = 1; 
  grid[cellindex(41, 9)] = 1; 
  grid[cellindex(41, 21)] = 1;
  grid[cellindex(41, 22)] = 1;

  grid[cellindex(42, 8)] = 1; 
  grid[cellindex(42, 9)] = 1; 
  grid[cellindex(42, 21)] = 1;
  grid[cellindex(42, 22)] = 1;

  grid[cellindex(43, 9)] = 1;
  grid[cellindex(43, 11)] = 1;
  grid[cellindex(43, 12)] = 1;
  grid[cellindex(43, 13)] = 1;
  grid[cellindex(43, 17)] = 1;
  grid[cellindex(43, 18)] = 1;
  grid[cellindex(43, 19)] = 1;
  grid[cellindex(43, 21)] = 1;

  grid[cellindex(44, 11)] = 1;
  grid[cellindex(44, 12)] = 1;
  grid[cellindex(44, 13)] = 1;
  grid[cellindex(44, 17)] = 1;
  grid[cellindex(44, 18)] = 1;
  grid[cellindex(44, 19)] = 1;

  grid[cellindex(45, 11)] = 1;
  grid[cellindex(45, 12)] = 1;
  grid[cellindex(45, 18)] = 1;
  grid[cellindex(45, 19)] = 1;

  grid[cellindex(46, 10)] = 1;
  grid[cellindex(46, 11)] = 1;
  grid[cellindex(46, 19)] = 1;
  grid[cellindex(46, 20)] = 1;

  grid[cellindex(47, 12)] = 1;
  grid[cellindex(47, 18)] = 1;

  grid[cellindex(48, 9)] = 1;
  grid[cellindex(48, 21)] = 1;

  grid[cellindex(49, 9)] = 1;
  grid[cellindex(49, 10)] = 1;
  grid[cellindex(49, 20)] = 1;
  grid[cellindex(49, 21)] = 1;

  grid[cellindex(51, 8)] = 1;
  grid[cellindex(51, 10)] = 1;
  grid[cellindex(51, 11)] = 1;
  grid[cellindex(51, 19)] = 1;
  grid[cellindex(51, 20)] = 1;
  grid[cellindex(51, 22)] = 1;

  grid[cellindex(52, 7)] = 1;
  grid[cellindex(52, 10)] = 1;
  grid[cellindex(52, 11)] = 1;
  grid[cellindex(52, 19)] = 1;
  grid[cellindex(52, 20)] = 1;
  grid[cellindex(52, 23)] = 1;

  grid[cellindex(53, 6)] = 1;
  grid[cellindex(53, 10)] = 1;
  grid[cellindex(53, 11)] = 1;
  grid[cellindex(53, 19)] = 1;
  grid[cellindex(53, 20)] = 1;
  grid[cellindex(53, 24)] = 1;

  grid[cellindex(54, 7)] = 1;
  grid[cellindex(54, 23)] = 1;
}