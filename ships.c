#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 5

char board[BOARD_SIZE][BOARD_SIZE];
int playerCol = 0;
int playerRow = 0;

typedef struct
{
  char symbol;
  int  size;
  int isSunk;
} Ship;

typedef struct
{
  char board[BOARD_SIZE][BOARD_SIZE];
  Ship ships[NUM_SHIPS];
  int hits;
  int misses;
} Game;

typedef struct
{
  int row;
  int col;
} Coordinates;

void cursor_moove()
{
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);
  
  int ch = getch();
  switch(ch)
  {
    case KEY_UP:
      if (playerRow <= 0)
      {
        playerRow = 0;
      }
      else
      {
        playerRow -= 1;
      }
      break;
    case KEY_DOWN:
      if (playerRow >= BOARD_SIZE)
      {
        playerRow = BOARD_SIZE;
      }
      else
      {
        playerRow += 1;
      }
      break;
    case KEY_LEFT:
      if (playerCol <= 0)
      {
        playerCol = 0;
      }
      else
      {
        playerCol -= 1;
      }
      break;
    case KEY_RIGHT:
      if (playerCol >= BOARD_SIZE)
      {
        playerCol = BOARD_SIZE;
      }
      else
      {
        playerCol += 1;
      }
  }
  endwin();
}


void print_board(char board[BOARD_SIZE][BOARD_SIZE]/*, int playerRow, int playerCol*/)
{
  clear();
  for (int i = 0; i < BOARD_SIZE; ++i)
    {
      for (int j = 0; j < BOARD_SIZE; ++j)
        {
          if (i == playerRow && j == playerCol)
          {
           board[i][j] = '0'; 
          }
          else
          {
            board[i][j] = '_';
          }
        }
    }
  cursor_moove();
}

int can_place_ship(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int isVertical)
{
  if (isVertical)
    {
      if ((row + size) > BOARD_SIZE)
        {
          return 0;
        }
      for (int i = row; i < (row + size); ++i)
        {
          if (board[i][col] != ('_' | '0'))
            {
              return 0;
            }
        }
    }
  else
  {
    if ((col + size) > BOARD_SIZE)
      {
        return 0;
      }
    for (int j = col; j < (col + size); ++j)
      {
        if (board[row][j] != ('_' | '0'))
          {
            return 0;
          }
      }
  }
  return 1;
}

void place_ships(char board[BOARD_SIZE][BOARD_SIZE], int row, int col, int size, int isVertical)
{
  if(isVertical)
    {
      for (int i = row; i < (row + size); ++i)
        {
          board[i][col] = 'X';
        }
    }
  else
    {
      for (int j = col; j < (col + size); ++j)
        {
          board[row][j] = 'X';
        }
    }
}

void generate_random_board(char board[BOARD_SIZE][BOARD_SIZE], Ship ships[], int numShips)
{
  srand(time(NULL));

  for (int i = 0; i < numShips; ++i)
    {
      int row, col, isVertical;
      do
        {
          row = rand() % BOARD_SIZE;
          col = rand() % BOARD_SIZE;
          isVertical = rand() % 2;
        } while (!can_place_ship(board, row, col, ships[i].size, isVertical));

      place_ships(board, row, col, ships[i].size, isVertical);
    }
}

void start_screen()
{
  initscr();
  
  addstr("Hi, it's my seabattle game\n\n");
  addstr("Enter [SPACE] for shot\n");
  addstr("Use [] for move on map\n");
  addstr(" \n");
  addstr(" \n");
  addstr(" \n");
  addstr(" \n");
  addstr(" \n");
  addstr("Press [ANY_KEY] to continue\n");
  
  getch();
  endwin();
}


// Generation board for to hide ships
void shield_board(const char board[BOARD_SIZE][BOARD_SIZE])
{
  initscr();

  addstr("    1 2 3 4 5 6 7 8 9 10\n");
  addstr(" _______________________\n");
  addstr("|");

  for (int i = 0; i < BOARD_SIZE; ++i)
    {
      printw("%c |", 'A' + i);
      for (int j = 0; j < BOARD_SIZE; ++j)
        {
          printw("%c", board[i][j]);
          addstr("|");
        }

      if (i < (BOARD_SIZE - 1))
        {
          addstr("\n|");
        }
    }

  getch();
  endwin();
}

void shots()
{

  int x = 0;
  int y = 0;
  int ch = getch();

  switch(ch)
    {
    case KEY_DOWN:
      if (y > 0) y--;
      break;
    case KEY_UP:
      if (y < 9) y++;
      break;
    case KEY_LEFT:
      if (x > 0) x--;
      break;
    case KEY_RIGHT:
      if (x < 9) x++;
      break;
    case 32:

      break;
    }
}

// Main function and menu
int main()
{
  char board[BOARD_SIZE][BOARD_SIZE];

  Ship ships[] =
    {
      {'C', 4, 0},
      {'B', 3, 0},
      {'R', 3, 0},
      {'S', 2, 0},
      {'D', 1, 0}
    };
  
  initscr();
  curs_set(0);
  keypad(stdscr, TRUE);

  start_screen();
  
  char *menuItems[] = {"[-] START | CONTINUE", "[-] RESTART", "[-] STAT", "[-] EXIT"};
  int totalChoices = sizeof(menuItems) / sizeof(menuItems[0]);
  int choice = 0;

  while(1)
    {
      clear();
      refresh();
      
      for (int i = 0; i < totalChoices; ++i)
        {
          if (i == choice)
            {
              attron(A_REVERSE);
            }

          mvprintw(i + 1, 1, "%s", menuItems[i]);
          attroff(A_REVERSE);
        }

      int ch = getch();

          switch(ch)
            {
            case KEY_UP:
              choice = (choice - 1 + totalChoices) % totalChoices;
              break;
            case KEY_DOWN:
              choice = (choice + 1) % totalChoices;
              break;
            case KEY_LEFT:
              endwin();
              return 0;
            case KEY_RIGHT:
            case 10:
              switch(choice)
                {
                case 0:
                  clear();
                  refresh();
                  for(;;){
                    print_board(board/* playerRow, playerCol*/);
                    shield_board(board);
                    generate_random_board(board, ships, 5);
                  }
                  endwin();
                  break;
                case 1:
                  break;
                case 2:
                  break;
                case 3:
                  endwin();
                  return 0;
                }
              break;
            case 27:
              endwin();
              return 0;
            }
    }
  endwin();
  return 0;
}
