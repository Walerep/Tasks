#include "GUI.h"

struct LISTING listing[LISTING_MAX];
extern struct GUI GUI;

char cmdoutlines[MAXROW][MAXCOL];
int ncmdlines,  //  Количество строк в cmdoutlines
    nwinlines,  //  количество строк которое займет вывод ls
    winrow,       //  текущий ряд (row) на экране
    cmdstartrow,  //  индекс первого ряда в cmdoutlines для отображения
    cmdlastrow;  //  индекс последнего ряда в cmdoutlines для отображения

//  Размер окна по количеству строк и столбцов
void sig_winch(int signo) {
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
  resize_term(size.ws_row, size.ws_col);
};

//  Подсветка пункта меню
void highlight() {
  int clinenum;
  attron(A_BOLD);
  clinenum = cmdstartrow + winrow;
  mvaddstr(winrow, 0, cmdoutlines[clinenum]);
  attroff(A_BOLD);
  refresh();
}

//  Инициализация ncurses
void curses_init() {
  initscr();
  signal(SIGWINCH, sig_winch);  // Размер окна по количеству строк и столбцов
  keypad(GUI.text_bar_box, TRUE);
  cbreak();
  noecho();
  curs_set(1);
  refresh();
};

//  Окно сообщений чата
void chat_area() {
  GUI.chat_window_box = newpad(LINES - BORDER_OF_MENU, COLS / 1.5);
  scrollok(GUI.chat_window_box, TRUE);
  box(GUI.chat_window_box, '|', '-');
  prefresh(GUI.chat_window_box, 0, 0, 0, 0, LINES - BORDER_OF_MENU, COLS / 1.5);
}

//  Окно со списком пользователей
void users_area() {
  GUI.users_window_box =
      newwin(LINES - BORDER_OF_MENU, COLS / 2, 0, COLS / 1.5);
  box(GUI.users_window_box, '|', '-');
  wrefresh(GUI.users_window_box);
}

// Окно навигации / ввода сообщения
void text() {
  GUI.text_bar_box = newwin(0, 0, LINES - BORDER_OF_MENU, 0);
  box(GUI.text_bar_box, '|', '-');
  wrefresh(GUI.text_bar_box);
};

//  Печатать в окно с конца прошлого текста
void print_to_win(WINDOW *target_win, int y, int x, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  vwprintw(target_win, format, arg);
  box(target_win, '|', '-');
  wrefresh(target_win);
  va_end(arg);
}

//  Печать в окно с координат 1, 1 окна
void reprint_to_win(WINDOW *target_win, int y, int x, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  wmove(target_win, y, x);
  vwprintw(target_win, format, arg);
  box(target_win, '|', '-');
  wrefresh(target_win);
  va_end(arg);
}

void read_from_win(WINDOW *target_win, char *format, int n) {
  echo();
  wclrtobot(target_win);
  box(target_win, '|', '-');
  wrefresh(target_win);
  wgetnstr(target_win, format, n);
  format[strcspn(format, "\n")] = '\0';
  noecho();
}

void navigation() {}
