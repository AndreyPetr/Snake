#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

#define vertical 30
#define horizontal 60
#define down 115
#define up 119
#define left 97
#define right 100

typedef struct Snake
{
    char symbol;
    int size;
    char direction;
    char prev_direction;
    int tail_X;
    int tail_Y;
    int head_X;
    int head_Y;

}snake;


typedef struct snake_pos
{
    int Y[vertical*horizontal];
    int X[vertical*horizontal];

}snake_pos;


void snake_init(snake *snake1);
void pos_init(snake_pos *pos1);
void food_init(food *food1);
void gotoxy(int,int);
void snake_place(snake *snake1, snake_pos *pos1);
void snake_move(snake *snake1, snake_pos *pos1, food *food1, int*);
void move_tail(snake *snake1, snake_pos *pos1);
void move_head(snake *snake1, snake_pos *pos1);
void food_print(food *food1);
int game_over(snake *snake1, snake_pos *pos1);
void set_borders();
void print_score(int*);
int kbhit(void);

typedef struct food
{
    int X;
    int Y;
    char symbol;

}food;


void set_borders();
void gotoxy(int,int);

int main()
{

      /* set initial field */

      system("clear");
      system("stty -echo");
      curs_set(0);                    // doesn't work for some reason
      set_borders();

     
      /* use system call to set terminal behaviour to more normal behaviour */
      system ("/bin/stty cooked");
      system("stty echo");
      system("clear");


      return 0;
}

void set_borders()
{
    int i;
    for (i=0; i<=vertical; ++i)
    {
        gotoxy(0,i);
        printf("|");
        gotoxy(horizontal,i);
        printf("|");
    }

    for (i=0; i<=horizontal; ++i)
        {
            gotoxy(i,0);
            printf("—");
            gotoxy(i,vertical);
            printf("—");
        }
}

void gotoxy(int x,int y)
{
    printf("%c[%d;%df",0x1B,y,x);
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
/*
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}*/
