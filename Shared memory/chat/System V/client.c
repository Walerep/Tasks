#include <curses.h>

#include "header.h"
#include "GUI.h"
#include "sem_ftok.h"


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
  while (getchar() != 'b') {
    reprint_to_win(chat_sem, GUI.chat_window_box, 1, 2, "%s\n", chat_addr);
    reprint_to_win(userlist_sem, GUI.users_window_box, 2, 2, "%s\n", userlist_addr);
    //sleep(2);
  }
}

//  отправка сообщений
void *sender() {
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
    noecho();

    switch (choice) {
      case 1:
        // Вызов функции получения сообщений

        reciver_msg();

        break;
      case 2:
        // Отправляем сообщение всем пользователям

        char full_msg[MAX_MSG];
        char msg_buf[MAX_TEXT];
        strcat(full_msg, Username);
        reprint_to_win(chat_sem, GUI.text_bar_box, 1, 2, "Message to everyone:\n");
        read_from_win(GUI.text_bar_box, msg_buf, MAX_TEXT);
        msg_buf[strcspn(msg_buf, "\0")] = '\n';
        strcat(full_msg, msg_buf);
        print_to_addr(chat_addr, chat_sem, full_msg);

        break;
      case 3:

        
        break;
      case 4:
        // Отправляем запрос на отключение от чата
        endwin();
        printf("Вы успешно вышли из чата.\n");
        exit(0);
      default:
        reprint_to_win(chat_sem, GUI.text_bar_box, 1, 2, "Wrong option.\n");
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
  printf("userlist_shm %d\n", userlist_shm);
  printf("chat_shm %d\n", chat_shm);

  printf("Enter name: ");
  fgets(Username, MAX_USERNAME, stdin);
  print_to_addr(userlist_addr, userlist_sem, Username);

  // Запуск окон и функции отправки

  curses_init();
  users_area();
  chat_area();
  text();
  sender();

  //  !!каким то образом надо подружить потоки и шареную память!!
  //pthread_create(&send_tid, NULL, &sender, NULL);
  //pthread_create(&recive_tid, NULL, &reciver_msg, NULL);
  
  endwin();

  return 0;
}