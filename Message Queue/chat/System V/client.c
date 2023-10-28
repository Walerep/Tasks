#include "header.h"
#include "GUI.h"
#include <curses.h>


#define err_exit(msg)   \
  do {                  \
    perror(msg);        \
    exit(EXIT_FAILURE); \
  } while (0)

long client_id;
int shared_room_id;

sem_t sem;


struct GUI GUI;

void Unlock_sem(sem_t* sem) {
  sem_post(sem);
}

void Destroy_sem(sem_t* sem) {
  if (sem_destroy(sem) == -1)
    perror("sem_destroy");
}

//  получение личных \ серверных сообщений
void *reciver_msg(){
  struct message msg;
  int status;
  while(1){
    msgrcv(client_id, &msg, sizeof(struct message), 0, 0);
    switch (msg.mtype)
    {
    case SERVICE_MSG:
      print_to_win(GUI.chat_window_box, 1, 1, "Service - : %s\n", msg.mtext);
      memset(&msg, 0, sizeof(msg));
      break;
    case REGISTER_MSG:
      print_to_win(GUI.chat_window_box, 1, 1, "REG - : %s\n", msg.mtext);
      memset(&msg, 0, sizeof(msg));
      break;
    case GET_USERS_MSG:
      reprint_to_win(GUI.users_window_box, 1, 2, "Users online:\n  %s", msg.mtext);
      //wrefresh(GUI.users_window_box);
      memset(&msg, 0, sizeof(msg));
      break;
    case SEND_ALL_MSG:
      print_to_win(GUI.chat_window_box, 1, 1, "Public:%s\n", msg.mtext);
      memset(&msg, 0, sizeof(msg));
      break;
    default:
      break;
    }
  }
}

//  отправка сообщений
void *sender(){
  struct message msg;
  int cursor_y, cursor_x; //  кординаты курсора
  while (1) {
    reprint_to_win(GUI.text_bar_box, 1, 2,\
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
    
    //scanf("%d", &choice);
    
    //getchar();  // Чтение символа новой строки после ввода числа
    noecho();
    switch (choice) {
      case 1:
        // Отправляем запрос на получение списка активных пользователей
        //strcpy(msg.mtext, "get_users");
        msg.mtype = GET_USERS_MSG;
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd get_users");
        }

        // Получаем список активных пользователей от сервера
        //if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
        //  err_exit("msgrcv get_users");
        //}

        //print_to_win(GUI.users_window_box, 1, 1, "Users online:\n%s\n", msg.mtext);
        //wrefresh(GUI.users_window_box);
        memset(&msg, 0, sizeof(msg));
        break;
      case 2:
        // Отправляем запрос на отправку сообщения всем пользователям
        
        //strcpy(msg.mtext, "send_all ");
        msg.mtype = SEND_ALL_MSG;
        //if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          //err_exit("msgsnd send_all");
        //}
        //if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
        //  err_exit("msgrcv get send_all");
        //}

        //printf("%s", msg.mtext);
        print_to_win(GUI.text_bar_box, 1, 1, "Message to everyone:\n");
        read_from_win(GUI.text_bar_box, msg.mtext, MAX_MSG);
        //msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        //strcpy(msg.mtext, Message_buff);

        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send_all");
        }

        //  Получение отправленного сообщения (проверка)
        //if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
        //  err_exit("msgrcv get send_all");
        //}

        //printf("Сообщение:\n%s\n", msg.mtext);
        memset(&msg, 0, sizeof(msg));
        break;
      case 3:
        
        //strcpy(msg.mtext, "send ");
        msg.mtype = SEND_MSG;
        //  Отправка запроса
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        
        //  Вводим имя получателя
        //printf("%s", msg.mtext);
        print_to_win(GUI.text_bar_box, 1, 1, "Enter reciver name: ");
        read_from_win(GUI.text_bar_box, msg.mtext, MAX_MSG);
        //msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        //  Отправляем имя получателя
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        //  Получаем подтверждение существования получателя
        //if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
        //  err_exit("msgrcv get send");
        //}
        print_to_win(GUI.text_bar_box, 1, 1, "Enter message: ");
        read_from_win(GUI.text_bar_box, msg.mtext, MAX_MSG);
        //msg.mtext[strcspn(msg.mtext, "\n")] = '\0';
        //  Отправляем текст сообщение
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd send");
        }
        //  Получение отправленного сообщения (проверка)
        //if (msgrcv(client_id, &msg, sizeof(struct message), client_id, 0) == -1) {
        //  err_exit("msgrcv get send_all");
        //}

        //printf("Сообщение:\n%s\n", msg.mtext);
        memset(&msg, 0, sizeof(msg));
        break;
      case 4:
        // Отправляем запрос на отключение от чата
        //strcpy(msg.mtext, "quit");
        msg.mtype = QUIT_MSG;
        if (msgsnd(client_id, &msg, sizeof(struct message), 0) == -1) {
          err_exit("msgsnd quit");
        }
        endwin();
        printf("Вы успешно вышли из чата.\n");
        exit(0);
      default:
        print_to_win(GUI.text_bar_box, 1, 1, "Wrong option.\n");
        wrefresh(GUI.text_bar_box);
    }
  }
}

int main() {
  printf("client\n");

  pthread_t recive_tid, send_tid;
  key_t key;

  int status;
  int server_msqid;   //  id очереди сервера
  struct message msg;
  //int client_id = getpid();

  //  id очереди клиента для получения ответов от сервера
  //client_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  //if (client_id == -1) err_exit("msgget client_msqid");

  // Генерируем уникальный ключ
  key = ftok(FTOK_PATH, 3);

  // Создаем очередь сообщений сервера
  server_msqid = msgget(key, 0666);
  if (server_msqid == -1) err_exit("msgget");
  printf("Enter name: ");
  fgets(Username, MAX_USERNAME, stdin);
  Username[strcspn(Username, "\n")] = '\0';
  
  // Отправляем запрос на регистрацию в чате
  //strcpy(msg.mtext, "register ");
  strcpy(msg.muser, Username);
  msg.mtype = REGISTER_MSG;
  printf("SENDING : %s\n", msg.muser);
  //sleep(3);

  
  if ((msgsnd(server_msqid, &msg, sizeof(struct message), 0)) < 0) {
    err_exit("msgsnd register");
  }
  //memset(&msg, 0, sizeof(msg));
  // Получаем ответ от сервера с ID клиента
  if ((msgrcv(server_msqid, &msg, sizeof(struct message), REGISTER_MSG, 0)) < 0) {
    err_exit("msgsnd register");
  }
  client_id = msg.id;
  printf("GETED %s %s (%ld)", msg.mtext, msg.muser, client_id);
  //sleep(2);

  status = sem_init(&sem, 0, 0);
  if (status == -1) {
    endwin();
    err_exit("sem_init");
  }
  //status == atexit(Destroy_sem);
  
  curses_init();
  users_area();
  chat_area();
  text();


  pthread_create(&send_tid, NULL, sender, NULL);
  pthread_create(&recive_tid, NULL, reciver_msg, NULL);
  //shared_room_id = server_msqid;
  //pthread_create(&sreciver_tid, NULL, reciver_msg, NULL);
  
  //if (msgrcv(client_id, &msg, sizeof(struct message), SERVICE_MSG, 0) == -1) {
  //  err_exit("msgrcv register");
  //}
  //printf("id: %ld\n",msg.id);
  //  получаем приветственное сообщение
  //if (msgrcv(client_id, &msg, sizeof(struct message), SERVICE_MSG, 0) == -1) {
  //  err_exit("msgrcv get_greetings");
  //}
  //printf("%s",msg.mtext);
  
  status = sem_wait(&sem);
  if (status == -1)
    err_exit("sem_wait");
  if (pthread_cancel(recive_tid) != 0)
  {
    err_exit("pthread join precv");
  }
  if (pthread_cancel(send_tid) != 0)
  {
    err_exit("pthread join send");
  }
  if (pthread_join(recive_tid, NULL) != 0)
  {
    err_exit("pthread join precv");
  }
  if (pthread_join(send_tid, NULL) != 0)
  {
    err_exit("pthread join send");
  }
  endwin();
  
  return 0;
}