#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <malloc.h>
#include <unistd.h>

//  обработчик ошибок из man'а pthread_create()
#define handle_error(msg) do {perror(msg); exit(EXIT_FAILURE); } while(0)

#define MAX_NEEDS 15000           //  нужды покупателя
#define MIN_LOADING 1500          //  нижний порог загрузки магазина
#define MAX_LOADING 10000         //  верхний порог загрузки магазина
#define BUYERS_QUANTITY 5         //  число покупателей
#define STORES_QUANTITY 3         //  число магазинов

pthread_mutex_t mutex[STORES_QUANTITY];

int stores[STORES_QUANTITY];            //  массив с заполненностью магазинов
int buyer_finish = BUYERS_QUANTITY;     //  счетчик оставшихся покупателей

//  Простая структура для инфы о покупателях
struct Buyer
{
  int id;
  int need;
};

struct Buyer buyers_list[BUYERS_QUANTITY];

//  Старт магазина
//  Заполнение массива магазинов случайными значениями
void store_start(){
  for (int i = 0; i < STORES_QUANTITY; i++)
  {
    stores[i] = rand() % (MAX_LOADING - MIN_LOADING + 1) +MIN_LOADING;
    printf("Магазин %d пополнил запасы до %d\n", i, stores[i]);
  }
}





//  Погрузчик (отдельный поток)
//  При заходе в поток  
//  погрузчик пополняет этот магазин, 
//  засыпает на x секунды  
//  и повторяется заново пока живы покупатели.
void *loader(){
  while(buyer_finish != 0){
      int target_store = rand() % STORES_QUANTITY;
      pthread_mutex_lock(&mutex[target_store]);
      printf("Магазин () %d ):  %d  -->  ", target_store, stores[target_store]);
      stores[target_store] += rand() % (MAX_LOADING - MIN_LOADING + 1) +MIN_LOADING;
      printf("%d\n", stores[target_store]);
      pthread_mutex_unlock(&mutex[target_store]);
      sleep(1);
  }
  pthread_exit(0);
}

//  Вызывается для создания потока погрузчика
void loader_start(pthread_t loader_thread){
  pthread_create(&loader_thread, NULL, loader, NULL);
}

//  Покупатель (отдельный поток на каждого) 
//  Если заходит в поток забирает себе весь товар
//  из целевого магазина, 
//  проверяет неудовлетворенность потребностей, 
//  если больше товаров не надо - умирает, 
//  иначе - засыпает на x секунды и по новой.
void *buyer(void *arg){
  struct Buyer *buyers_list = (struct Buyer *)arg;
  while (buyers_list->need > 0)                                 //  Пока нужды покупателя больше 0 работаем
  {
    int target_store = rand() % STORES_QUANTITY;                //  Целевой магаз выбираем рандомом
    if (stores[target_store] > 0){
      pthread_mutex_lock(&mutex[target_store]);                 //  Лок мьютекса для целевого магазина
      printf("Покупатель ( %d ) Магазин (%d) : %d  -->  ", buyers_list->id, target_store, buyers_list->need);
      buyers_list->need -= stores[target_store];                //  Удовлетворяем нужды покупателя
      stores[target_store] = 0;                                 //  Обнуляем ассортимент
      printf("%d\n", buyers_list->need);
      pthread_mutex_unlock(&mutex[target_store]);               //  Анлок мьютекса
      sleep(3);
    }
    else
    {
      printf("Покупатель ( %d ) : В магазине ( %d ) нет товара\n", buyers_list->id, target_store);
    }
    sleep(3);
  }
  printf("Покупатель ( %d ) : Конец!\n", buyers_list->id);
  buyer_finish--;
  pthread_exit(0);
}

//  Старт покупателей
//  Заводит на каждого покупателя структуру
//  с номером покупателя и максимальной потребностью
void buyer_start(pthread_t *buyers_thread){

  for (int i = 0; i < BUYERS_QUANTITY; i++){
    buyers_list[i].id = i;
    buyers_list[i].need = MAX_NEEDS;
  }
  for (int i = 0; i < BUYERS_QUANTITY; i++)
  {
    int s;
    void *buyers_buff = &buyers_list[i];
    s = pthread_create(&buyers_thread[i], NULL, buyer, buyers_buff);
    if (s != 0)
    {
      handle_error("pthread_create");
    }
    
  }
}

int main(){
  printf("ЧЕРНАЯ ПЯТНИЦА В М.ВИДЕО\n");
  //pthread_t loader;
  pthread_t loader_thread;                    //  pthread для погрузчика
  pthread_t buyers_thread[BUYERS_QUANTITY];   //  pthread для покупателей
  store_start();
  loader_start(loader_thread);
  buyer_start(buyers_thread);
  pthread_join(loader_thread, NULL);
  for (int i = 0; i < BUYERS_QUANTITY; i++)
  {
    pthread_join(buyers_thread[i], NULL);
  }
  for (int i = 0; i < STORES_QUANTITY; i++)
  {
    pthread_mutex_destroy(&mutex[i]);
  }
  
  printf("все дороги ведут в галамарт....\n");
  return 0;
}