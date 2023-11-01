#ifndef GUI_H
#define GUI_H
//#define HEADER_H

#include <dirent.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include "header.h"

#define err_msg(msg)    \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

#define MAXROW 100
#define MAXCOL 50
#define BORDER_OF_MENU 10
#define LISTING_MAX 128

//  struct for anything in current directory
struct LISTING {
  int position;        // position of smth in that struct
  char name[128];      //  name of folder  / file in struct
  unsigned char type;  // type of smth in that struct
};

struct GUI {
  WINDOW* text_bar_box;  // указатель на окно меню
  WINDOW* text_bar;
  WINDOW* chat_window_box;
  WINDOW* chat_window;
  WINDOW* window_box;
  WINDOW* users_window_box;
  WINDOW* users_window;
};

void curses_init();  //  Инициализация ncurses
void navigation();  //  навигация по окну курсором и F кнопками
void text();  // Окно навигации / ввода сообщения
void users_area();  //  Окно со списком пользователей
void chat_area();   //  Окно сообщений чата
void highlight();   //  Подсветка пункта меню
void sig_winch(int signo);  //  Размер окна по количеству строк и столбцов
void print_to_addr(char* target_addr, int target_sem,
                   const char* msg_buf);  //  Печать в адресное пространство
                                          //  addr из буффера msg
void print_to_win(int target_sem, WINDOW* target_win, const char* format,
                  ...);  //  Печатать в окно с конца прошлого текста
void reprint_to_win(int target_sem, WINDOW* target_win, int y, int x,
                    const char* format,
                    ...);  //  Печать в окно с координат 1, 1 окна
void read_from_win(WINDOW* target_win, char* format,
                   int n);  // Чтение строки с окна
#endif