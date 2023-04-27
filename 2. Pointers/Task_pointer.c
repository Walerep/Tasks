
void print_int_by_byte(int a) {
  char *ptr;
  ptr = &a;
  for (int i = 0; i < sizeof(int); i++) {
    printf("%d byte of a = ", i);
    printf("%d\n", *ptr);
    ptr++;
  }
}

int main() {
  int a = 270533154;
  char *ptr;
  ptr = &a;
  print_int_by_byte(a);
  *(ptr+2) = 0;
  printf("a = %d\n", a);
  print_int_by_byte(a);
}
