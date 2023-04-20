#include <stdio.h>
#define N 3

void print_task1 (int arr_target [N] [N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
        {
            printf ("%d ", arr_target[i][j]);
        }
        printf("\n");
    }
}

void print_task2 (int arr_target [N]) {
    for (int i = 0; i < N; i++) {
        printf ("%d ", arr_target[i]);
    }
    printf ("\n");
}


void task1 () {
    int array_size_n [N] [N]; // Array size of N
    int counter = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            array_size_n [i] [j] = counter++;
        }
    }
    print_task1(array_size_n);
}

void task2 () {
    int array_size_n [N]; // Array size of N
    int array_temp [N];  // Temporary array
    for (int i = 0; i < N; i++) {
        array_size_n [i] = i + 1;
    }
    for (int i = 0; i < N; i++) {
        array_temp [i] = array_size_n [N - i - 1];
    }
    for (int i = 0; i < N; i++) {
        array_size_n [i] = array_temp [i];
    }
    print_task2(array_size_n);
}

void task3 () {
    int array_size_n [N] [N] = {0} ; // Array size of N
    int count = N;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < count; j++)
        {
            array_size_n [i] [j] = 1;
        }
        count--;
    }
    print_task1 (array_size_n);
}


int main () {
    printf ("task 1:\n");
    task1 ();
    printf ("task 2:\n");
    task2 ();
    printf ("task 3:\n");
    task3 ();
    return 0;
}