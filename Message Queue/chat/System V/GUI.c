#include <locale.h>
#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define err_msg(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

#define MAXROW 100
#define MAXCOL 50
#define BORDER_OF_MENU 10
#define LISTING_MAX 128

WINDOW * text_bar_box; // указатель на окно меню
WINDOW * text_bar;
WINDOW * chat_window_box;
WINDOW * chat_window;
WINDOW * window_box;
WINDOW * users_window_box;
WINDOW * users_window;

//  struct for anything in current directory
struct LISTING
{
  int position; // position of smth in that struct
  char name[128]; //  name of folder  / file in struct
  unsigned char type; // type of smth in that struct
};

struct LISTING listing[LISTING_MAX]; 


char cmdoutlines[MAXROW][MAXCOL];
int ncmdlines,    //  Количество строк в cmdoutlines
    nwinlines,     //  количество строк которое займет вывод ls
    winrow,       //  текущий ряд (row) на экране
    cmdstartrow,  //  индекс первого ряда в cmdoutlines для отображения
    cmdlastrow;   //  индекс последнего ряда в cmdoutlines для отображения

//  Размер окна по количеству строк и столбцов
void sig_winch(int signo){ 
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
  resize_term(size.ws_row, size.ws_col);
};

//  Подсветка пункта меню
void highlight(){
  int clinenum;
  attron(A_BOLD);
  clinenum = cmdstartrow + winrow;
  mvaddstr(winrow, 0, cmdoutlines[clinenum]);
  attroff(A_BOLD);
  refresh();
}

//  Инициализация ncurses
void curses_init(){
  
  initscr();  
  signal(SIGWINCH, sig_winch);  //Размер окна по количеству строк и столбцов
  keypad(stdscr, TRUE);
  cbreak();
  noecho();
  curs_set(1);
  refresh();
};
  
  //  (legacy) Вывод содержимого текущего каталога
void whats_inside(WINDOW * window, const char * path) {
  struct dirent * namelist;
  int i = 1;
  int index = 0;
  char * dir_inside[100];
  DIR * dir = opendir(path);
  if (dir == NULL) err_msg("opendir");
  while ((namelist = readdir(dir)) != NULL){
    dir_inside[i] = namelist->d_name;
    strncpy(listing[i].name, dir_inside[i], 64);
    listing[i].type = namelist->d_type;
    listing[i].position = i;
    mvwprintw(window, i++, 1, "%d) %s   %d", listing[i].position, listing[i].name, listing[i].type);
  }
  
  closedir(dir);
}

//  Окно сообщений чата
void chat_area(){ 
  chat_window_box = newwin(LINES-BORDER_OF_MENU,COLS/1.5,0,0);
  //chat_window = derwin(chat_window_box, 1, 0, 1, 2);

  whats_inside(chat_window_box, ".");

  box (chat_window_box,'|','-');
  wrefresh(chat_window_box);
}

//  Окно со списком пользователей
void users_area(){  
  users_window_box = newwin(LINES-BORDER_OF_MENU,COLS/2,0,COLS/1.5);
  whats_inside(users_window_box, ".");
  box (users_window_box,'|','-');
  wrefresh(users_window_box);
}

// Окно навигации / ввода сообщения
void text(){  
  text_bar_box = newwin(0,0,LINES-BORDER_OF_MENU,0);
  text_bar = derwin(text_bar_box, 1,0,1,2);
  box(text_bar_box,'|','-');
  wrefresh(text_bar_box);
};

void navigation(){
  //text();
  int cursor_y; //координаты курсора
  int cursor_x;
  getyx(stdscr, cursor_y, cursor_x);
  switch (getch())
  { 
    /*
  case KEY_UP:
    move(cursor_y - 1, cursor_x);
    break;
  case KEY_RIGHT:
    move(cursor_y, cursor_x + 1);
    break;
  case KEY_DOWN:
    move(cursor_y + 1, cursor_x);
    break;
  case KEY_LEFT:
    move(cursor_y, cursor_x -1 );
    break;
    */
  case KEY_F(3):
    endwin();
    break;
  }
}


int main(){
  char c;
  curses_init();
  text();
  chat_area();
  users_area();
  //runls();
  //showlastpart();
  winrow--;
  highlight();
  while (1)
  {
    c = getch();
    if (c == 'q') break;
  }
  endwin();
  return 0;
}