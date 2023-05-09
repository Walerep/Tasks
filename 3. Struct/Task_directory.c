#include "stdio.h"
#include "string.h"
#define n 5  // количество записей в справочнике

// база для справочника
struct directory {
  int id;
  char firstname[20];
  char surname[30];
  char telephone[13];
};

// вывести весь справочник
void show_all(struct directory directory[n]) {
  printf("id | Имя | Фамилия | Телефон \n\n");
  for (int i = 0; i < n; i++) {
    if (strcmp(directory[i].telephone, "") != 0) {
      printf("%d | %s | %s | %s\n",
        directory[i].id,
        directory[i].firstname,
        directory[i].surname,
        directory[i].telephone);
    }
  }
  printf("\n----------Конец списка---------\n");
}

// добаивть абонента
void add_field(struct directory directory[n]) {
  char firstname[20], surname[30], telephone[13];
  printf("Введите имя: ");
  scanf("%s", firstname);
  printf("Введите фамилию: ");
  scanf("%s", surname);
  printf("Введите телефон: ");
  scanf("%s", telephone);
  for (int i = 0; i < n; i++) {
    if (strcmp(directory[i].telephone, "") == 0) {
      strcpy(directory[i].firstname, firstname);
      strcpy(directory[i].surname, surname);
      strcpy(directory[i].telephone, telephone);
      printf("Абонент добавлен\n");
      break;
    }
  }
}

// удалить абонента
void delete_field(struct directory directory[n]) {
  int option;
  printf("Выберите опцию:\n");
  printf("1. Удалить по id\n");
  printf("2. Удалить по имени\n");
  printf("3. Удалить по фамилии\n");
  scanf("%d", &option);
  switch (option) {
  case 1:
    int id_chose = 0;
    printf("Введите id: ");
    scanf("%d", &id_chose);
    for (int i = 0; i < n; i++) {
      if (directory[i].id == id_chose) {
        strcpy(directory[i].firstname, "");
        strcpy(directory[i].surname, "");
        strcpy(directory[i].telephone, "");
        printf("Абонент удален\n");
        break;
      }
    }
  case 2:
    char name_target[20];
    printf("Введите имя: ");
    scanf("%s", name_target);
    for (int i = 0; i < n; i++) {
      if (strcmp(directory[i].firstname, name_target) == 0) {
        strcpy(directory[i].firstname, "");
        strcpy(directory[i].surname, "");
        strcpy(directory[i].telephone, "");
        printf("Абонент удален\n");
        break;
      }
    }
    case 3:
    char surname_target[20];
    printf("Введите фамилию: ");
    scanf("%s", surname_target);
    for (int i = 0; i < n; i++) {
      if (strcmp(directory[i].surname, surname_target) == 0) {
        strcpy(directory[i].firstname, "");
        strcpy(directory[i].surname, "");
        strcpy(directory[i].telephone, "");
        printf("Абонент удален\n");
        break;
      }
    }
  default:
    printf("Oopsie...\n");
    break;
  }
}

// найти абонента
void search_field(struct directory directory[n]) {
  char firstname_target[30];
  printf("Введите имя для поиска: ");
  scanf("%s", firstname_target);
  for (int i = 0; i < n; i++) {
    if (strcmp(firstname_target, directory[i].firstname) == 0) {
      printf("%d | %s | %s | %s\n",
      directory[i].id,
      directory[i].firstname,
      directory[i].surname,
      directory[i].telephone);
    }
  }
}

int main() {
  int option;
  struct directory directory[n];
  for (int i = 0; i < n; i++) {
    directory[i].id = i+1;
    strcpy(directory[i].firstname, "");
    strcpy(directory[i].surname, "");
    strcpy(directory[i].telephone, "");
  }
  do {
    printf("1. Просмотреть справочник\n");
    printf("2. Добавить абонента\n");
    printf("3. Удалить абонента\n");
    printf("4. Найти абонента\n");
    printf("5. Выход\n");
    scanf("%d", &option);
    switch (option) {
    case 1:
      show_all(directory);
      break;
    case 2:
      add_field(directory);
      break;
    case 3:
      delete_field(directory);
      break;
    case 4:
      search_field(directory);
      break;
    case 5:
      return 0;
    default:
      printf("Ошибка\n");
      break;
    }
  } while ("");
}
