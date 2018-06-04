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



typedef struct food
{
    int X;
    int Y;
    char symbol;

}food;



void snake_init(snake *snake1);
void pos_init(snake_pos *pos1);
void food_init(food *food1);
void gotoxy(int,int);
void snake_place(snake *snake1, snake_pos *pos1);
void snake_move(snake *snake1, snake_pos *pos1, food *food1, int*);

int main()
{
      int snake_speed=100000;
      int score=0;
      initscr(); curs_set(0);

      /* struct init */

      snake snake1;
      snake_pos pos1;
      food food1;
      snake_init(&snake1);
      pos_init(&pos1);
      food_init(&food1);


      /* set initial field */

      system("clear");
      system("stty -echo");
      curs_set(0);                    // doesn't work for some reason
      snake_place(&snake1,&pos1);
      set_borders();
      food_print(&food1);

      /* use system call to make terminal send all keystrokes directly to stdin */

      system ("/bin/stty raw");

      /* while snake not got collided into itself */

      while(!(game_over(&snake1,&pos1)))
      {

          /* while key not pressed */

          while (!kbhit())
          {
                 usleep(snake_speed);
                 snake_move(&snake1,&pos1,&food1,&score);
		if (game_over(&snake1,&pos1))
                 {
                     break;
                 }

          }
          /* store previous direction and fetch a new one */

          snake1.prev_direction=snake1.direction;
          snake1.direction=getchar();
      }
      /* use system call to set terminal behaviour to more normal behaviour */
      system ("/bin/stty cooked");
      system("stty echo");
      system("clear");
      printf("\n\n Final score: %d \n\n", score);

      return 0;
}

void snake_init(snake *snake1)
{
    snake1->symbol='*';
    snake1->size=5;
    snake1->direction=right;
    snake1->prev_direction=down;
    snake1->tail_X=5;
    snake1->tail_Y=5;
    snake1->head_X=snake1->tail_X+snake1->size-1;
    snake1->head_Y=5;
}

void snake_place(snake *snake1, snake_pos *pos1)
{
    for (int i=0; i<snake1->size; ++i)
    {
        gotoxy(snake1->tail_X,snake1->tail_Y);
        printf("%c",snake1->symbol);
        pos1->X[i]=snake1->tail_X;
        pos1->Y[i]=snake1->tail_Y;
        snake1->tail_X+=1;
    }

}



void snake_move(snake *snake1, snake_pos *pos1, food *food1, int *score)
{

    move_head(snake1,pos1);
	

    if (!((snake1->head_X==food1->X) && (snake1->head_Y==food1->Y)))
    {
        move_tail(snake1,pos1);
    }
    else
    {
        snake1->size++;
        *score=*score+1;
	for (int o = 0; o < 10; o++){
	food_init(food1);
        food_print(food1);
	}
    }
}



void move_head(snake *snake1, snake_pos *pos1)
{
    switch (snake1->direction)
        {
            case right:
                if (snake1->prev_direction==left)
                {
                    snake1->head_X--;
                    break;
                }
                    snake1->head_X++;
                    break;

            case left:
                if (snake1->prev_direction==right)
                {
                    snake1->head_X++;
                    break;
                }
                    snake1->head_X--;
                    break;


            case up:
                if (snake1->prev_direction==down)
                {
                    snake1->head_Y++;
                    break;
                }
                    snake1->head_Y--;
                    break;


            case down:
                if (snake1->prev_direction==up)
                {
                    snake1->head_Y--;
                    break;
                }
                    snake1->head_Y++;
                    break;


            default:
                 break;
        }
        // update tail position
        pos1->X[snake1->size]=snake1->head_X;
        pos1->Y[snake1->size]=snake1->head_Y;

        gotoxy(pos1->X[snake1->size],pos1->Y[snake1->size]);
        printf("%c",snake1->symbol);
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
