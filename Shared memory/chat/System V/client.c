#include <curses.h>

#include "GUI.h"
#include "header.h"

#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

long client_id;
int shared_room_id;

struct GUI GUI;


//  получение личных \ серверных сообщений
void *reciver_msg() {
  while (1) {
    reprint_to_win(chat_sem, GUI.chat_window_box, 1, 1, "%s\n", chat_addr);
    reprint_to_win(userlist_sem, GUI.users_window_box, 1, 1, "%s\n", userlist_addr);
    sleep(2);
  }
}

//  отправка сообщений
void *sender() {
  int cursor_y, cursor_x;  //  кординаты курсора
  while (1) {
    reprint_to_win(chat_sem, GUI.text_bar_box, 1, 2,
                   "Choose option:\n\
  1. Get users list\n\
  2. Send public message\n\
  3. Send private message\n\
  4. Quit chat\n\
  Enter option num: ");

    echo();

    wrefresh(GUI.text_bar_box);
    int choice;

    wscanw(GUI.text_bar_box, "%d", &choice);

    // scanf("%d", &choice);

    // getchar();  // Чтение символа новой строки после ввода числа
    noecho();
    switch (choice) {
      case 1:
        // Отправляем запрос на получение списка активных пользователей

        // Получаем список активных пользователей от сервера
        // if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) ==
        // -1) {
        //  err_exit("msgrcv get_users");
        //}

        break;
      case 2:
        // Отправляем запрос на отправку сообщения всем пользователям

        char msg_buf[MAX_MSG];
        print_to_win(chat_sem, GUI.text_bar_box, "Message to everyone:\n");
        read_from_win(GUI.text_bar_box, msg_buf, MAX_MSG);
        print_to_addr(chat_addr, chat_sem, msg_buf);

        break;
      case 3:

        //  Отправка запроса
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }

        //  Вводим имя получателя

        print_to_win(GUI.text_bar_box, 1, 1, "Enter reciver name: ");
        read_from_win(GUI.text_bar_box, msg.mtext, MAX_MSG);

        //   Отправляем имя получателя
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }

        print_to_win(chat_sem, GUI.text_bar_box, "Enter message: ");
        read_from_win(GUI.text_bar_box, msg.mtext, MAX_MSG);

        //   Отправляем текст сообщение
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }

        // printf("Сообщение:\n%s\n", msg.mtext);
        break;
      case 4:
        // Отправляем запрос на отключение от чата
        endwin();
        printf("Вы успешно вышли из чата.\n");
        exit(0);
      default:
        print_to_win(chat_sem, GUI.text_bar_box, "Wrong option.\n");
        wrefresh(GUI.text_bar_box);
    }
  }
}

int main() {
  printf("client\n");

  pthread_t recive_tid, send_tid;

  // Генерируем уникальный ключ
  userlist_key = ftok(FTOK_PATH_ULK, 3);
  chat_key = ftok(FTOK_PATH_CK, 3);

  // Создаем очередь сообщений сервера
  userlist_shm = shmget(userlist_key, MAX_MSG, 0);
  if (userlist_shm == -1) err_exit("shmget userlist");
  chat_shm = shmget(chat_key, MAX_MSG, 0);
  if (chat_shm == -1) err_exit("shmget chat");

  //  Привязываем рзделенную память в адресное пространство
  userlist_addr = shmat(userlist_shm, NULL, 0);
  if (userlist_addr == (char *)-1) err_exit("shmat userlist");
  chat_addr = shmat(chat_shm, NULL, 0);
  if (chat_addr == (char *)-1) err_exit("shmat chat");

  //  Генерируем семафоры
  //  Семфор чата
  sem_chat_key = ftok(FTOK_PATH_CK, 1);
  if (sem_chat_key == -1) err_exit("ftok");
  chat_sem = semget(sem_chat_key, 1, 0);
  if (chat_sem == -1) err_exit("ftok");

  // Семафор списка пользователей
  sem_userlist_key = ftok(FTOK_PATH_ULK, 2);
  if (sem_userlist_key == -1) err_exit("ftok");
  userlist_sem = semget(sem_userlist_key, 1, 0);
  if (userlist_sem == -1) err_exit("semget");

  printf("Enter name: ");
  fgets(Username, MAX_USERNAME, stdin);

  // Отправляем запрос на регистрацию в чате

  // sleep(3);

  curses_init();
  users_area();
  chat_area();
  text();

  sender();

  endwin();

  return 0;
}