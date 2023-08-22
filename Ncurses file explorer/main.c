#include <locale.h>
#include <stdio.h>
#include <curses.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>

  WINDOW * menu_bar_box; // указатель на окно меню
  WINDOW * menu_bar;
  WINDOW * text_window_box;  //рамка окна текста
  WINDOW * text_window; //текстовое окно

void sig_winch(int signo){ //Размер окна по количеству строк и столбцов
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
  resize_term(size.ws_row, size.ws_col);
};

void curses_init(){
  
  initscr();  //инициализация ncurses
  signal(SIGWINCH, sig_winch);
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  refresh();
};


void menu(){ //окно с функциями F1-F3
  //WINDOW * menu_bar_box; // указатель на окно меню
  //WINDOW * menu_bar;
  menu_bar_box = newwin(3,0,0,0);
  box(menu_bar_box,'|','-');
  menu_bar = derwin(menu_bar_box, 1,0,1,2);
  wprintw(menu_bar, "F1 - Open File   F2 - Save file   F3 - Exit");
  wrefresh(menu_bar_box);
};

void open_file(){
  //WINDOW * text_window_box;  //рамка окна текста
  //WINDOW * text_window; //текстовое окно
  FILE * text = fopen("text.txt", "r");
  if (text == NULL)
  {
    perror ("text");
    exit(2);
  }
  char buff[10];  //буффер для символов
  text_window_box = newwin(0,0,4,0);
  box(text_window_box,'|','-');
  text_window = derwin(text_window_box,0,0,1,2);
  //wmove(text_window_box,1,2);
  curs_set(1);
  //idlok(stdscr, TRUE);
  //keypad(stdscr, TRUE);

  while (fgets(buff, 10, text))
  {
    wprintw(text_window,"%s", buff);
  }
  fclose(text);
  move(5, 1);
  wrefresh(text_window_box);
}
/*
void save_file(){
  chtype buff[10];
  FILE * text = fopen("save_test.txt", "w");
  winchstr(stdscr, buff);
  fprintf(stdscr, buff);
  //wininchstr(text_window, );
  fclose(text);
}
*/

void save_file()
{
    FILE *fd = fopen("save_test.txt", "w");
    if (fd == NULL)
    {
      perror ("text");
      exit(2);
    }
    register int x, y, lastnospace;
    for (y = 0; y < LINES; y++)
    {
        for (lastnospace = (-1), x = 0; x < COLS; x++)
            if ((mvwinch(text_window, y, x) & A_CHARTEXT) != ' ')
                lastnospace = x;
        for (x = 0; x <= lastnospace; x++)
        {
            wmove(text_window, y, x);
            putc(winch(text_window) & A_CHARTEXT, fd);
        }
        putc('\n', fd);
    }
    move(0, 0);
    fclose(fd);
}

void ending(){
  endwin();
  exit(EXIT_SUCCESS);
}

void navigation(){
  //menu();
  int cursor_y; //координаты курсора
  int cursor_x;
  getyx(stdscr, cursor_y, cursor_x);
  switch (wgetch(stdscr))
  {
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
  case KEY_F(1):
    open_file();
    delwin(menu_bar_box);
    break;
  case KEY_F(2):
    save_file();
    break;
  case KEY_F(3):
    ending();
    break;
  }
}




int main(){
  //setlocale(LC_ALL, "RUS");
  curses_init();
  menu();
  //char choice;
  while (1)
  {
    navigation();
    //refresh();
  } 

  endwin();
  exit(EXIT_SUCCESS);
};
