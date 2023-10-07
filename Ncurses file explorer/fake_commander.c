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
#define BORDER_OF_MENU 3
#define LISTING_MAX 128

WINDOW * menu_bar_box; // указатель на окно меню
WINDOW * menu_bar;
WINDOW * left_window_box;
WINDOW * left_window;
WINDOW * window_box;
WINDOW * right_window_box;
WINDOW * rigth_window;

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

void sig_winch(int signo){ //Размер окна по количеству строк и столбцов
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
  resize_term(size.ws_row, size.ws_col);
};

void highlight(){
  int clinenum;
  attron(A_BOLD);
  clinenum = cmdstartrow + winrow;
  mvaddstr(winrow, 0, cmdoutlines[clinenum]);
  attroff(A_BOLD);
  refresh();
}


/*
//  Вызов ls для получения списка каталогов и файлов
void runls(){
  FILE *p;
  char ln[MAXCOL];
  int row, tmp;
  p = popen("ls -a", "r");  //  Открывает канал на чтение для ls
  for (row = 0; row < MAXROW; row++)
  {
    tmp = fgets(ln, MAXCOL, p); //  Чтение одной строки из канала
    if (tmp == NULL) break;
    strncpy(cmdoutlines[row], ln, COLS);
    cmdoutlines[row][MAXCOL-1] = 0;
  }
  ncmdlines = row;
  close(p);
}


//  отобразить последнюю часть вывода команды сколько поместится на экран
void showlastpart(){
  int row;
  //clear();
  if (ncmdlines <= LINES){  // если весь вывод поместится на экран
                            // LINES - переменная определенная самим ncurses 
                            //  как количество строк на экране
    cmdstartrow = 0;
    nwinlines = ncmdlines;
  } else {                  // если весь вывод не поместится на один экран
    cmdstartrow = ncmdlines - LINES;
    nwinlines = LINES;
  }
  cmdlastrow = cmdstartrow + nwinlines -1;
  //  вывод на экран
  for (row = cmdstartrow, winrow = 0; row <= cmdlastrow; row++, winrow++)
  {
    mvwaddstr(right_window_box, winrow, 0, cmdoutlines[row]);
  }
  refresh();
}
*/

void curses_init(){
  
  initscr();  //инициализация ncurses
  signal(SIGWINCH, sig_winch);  //Размер окна по количеству строк и столбцов
  keypad(stdscr, TRUE);
  cbreak();
  noecho();
  curs_set(1);
  refresh();
};

  //  Вывод содержимого текущего каталога
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


void left_explorer(){
  left_window_box = newwin(LINES-BORDER_OF_MENU,COLS/2,0,0);
  //left_window = derwin(left_window_box, 1, 0, 1, 2);

  whats_inside(left_window_box, ".");

  box (left_window_box,'|','-');
  wrefresh(left_window_box);
}

void right_explorer(){
  right_window_box = newwin(LINES-BORDER_OF_MENU,COLS/2,0,COLS/2);
  whats_inside(right_window_box, ".");
  box (right_window_box,'|','-');
  wrefresh(right_window_box);
}

void menu(){ //окно с функциями F1-F3
  menu_bar_box = newwin(0,0,LINES-BORDER_OF_MENU,0);
  menu_bar = derwin(menu_bar_box, 1,0,1,2);
  wprintw(menu_bar, "F1 - Open File   F2 - Save file   F3 - Exit");
  box(menu_bar_box,'|','-');
  wrefresh(menu_bar_box);
};

void navigation(){
  //menu();
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
  menu();
  left_explorer();
  right_explorer();
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