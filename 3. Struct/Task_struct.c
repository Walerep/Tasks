#include <stdio.h>

struct test {
  char a;
  int b;
};

struct test_packed {
  char a;
  int b;
}__attribute__((packed));

void unpacked_struct() {
  char str[8] = {'A',0,0,0,'B',0,0,0};
  struct test *ptr;
  ptr = &str;
  ptr->a = 'B';
  ptr->b = 12345;
  printf("\nUnpacked struct\n");
  for (int i = 0; i < sizeof(str); i++) {
    printf("%d  ", str[i]);
  }
}

void packed_struct() {
  char str[10] = {0,0,0,0,0,0,0,0,0,0};
  struct test_packed *ptr;
  ptr = &str;
  ptr->a = 'C';
  ptr->b = 12345;
  printf("\nPacked struct\n");
  for (int i = 0; i < sizeof(str); i++) {
    printf("%d  ", str[i]);
  }
}

int main() {
  struct test my_test;
  printf("\nSize of unpacked = %d", sizeof(my_test));
  struct test_packed my_test_packed;
  printf("\nSize of packed = %d", sizeof(my_test_packed));
  unpacked_struct();
  packed_struct();
}
