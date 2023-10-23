#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(12345);
    if (inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr)) <= 0) {
        perror("Ошибка настройки адреса сервера");
        exit(1);
    }

    // Подключение к серверу
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Ошибка подключения к серверу");
        exit(1);
    }

    printf("Подключение к серверу успешно\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("Введите сообщение: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Отправка сообщения на сервер
        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Ошибка отправки сообщения");
            exit(1);
        }
    }

    close(client_socket);

    return 0;
