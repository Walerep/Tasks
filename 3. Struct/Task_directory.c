#define n 10 //количество записей в справочнике

//база для справочника
struct directory {
  int id;
  char firstname[30];
  char surname[30];
  char telephone[12];
};

//вывести весь справочник
void show_all(struct directory directory[n]) {
  printf("id | Имя | Фамилия | Телефон \n\n");
  for (int i = 0; i < n; i++) {
    printf("%d | %s | %s | %s\n",
      directory[i].id,
      directory[i].firstname,
      directory[i].surname,
      directory[i].telephone);
  }
  printf("----------Конец списка---------\n");
}

//добаивть абонента
void add_field(struct directory directory[n]) {
  //int id;
  char firstname[30], surname[30], telephone[12];
  printf("Введите имя: ");
  scanf("%s", firstname);
  printf("Введите фамилию: ");
  scanf("%s", surname);
  printf("Введите телефон: ");
  scanf("%s", telephone);
  for (int i = 0; i < n; i++) {
    if (strcmp(directory[i].telephone, "0") == 0) {
      strcpy(directory[i].firstname, firstname);
      strcpy(directory[i].surname, surname);
      strcpy(directory[i].telephone, telephone);
      printf("Абонент добавлен\n");
      break;
    }
  }
}

//удалить абонента
void delete_field(struct directory directory[n]) {
  int option;
  printf("Выберите опцию:\n1. Удалить по id\n2. Удалить по имени\n3. Удалить по фамилии\n");
  scanf("%d", option);
  switch (option) {
  case 1:
    int id_chose = 0;
    scanf("Введите id: %d", &id_chose);
    strcpy(directory[id_chose].firstname, "");
    strcpy(directory[id_chose].surname, "");
    strcpy(directory[id_chose].telephone, "");
    break;
  
  default:
    break;
  }
}

//найти абонента

int main() {
  short int option;
  struct directory directory[n];
  do {
    printf("1. Просмотреть справочник\n");
    printf("2. Добавить абонента\n");
    printf("3. Удалить абонента\n");
    printf("4. Найти абонента\n");
    printf("5. Выход\n");
    scanf("%d", option);
    switch (option)
    {
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
      find_field(directory);
      break;
    case 5:
      return 0;
    default:
      printf("Ошибка\n");
      break;
    }
  } while ("");
}
