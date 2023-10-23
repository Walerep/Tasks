#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int client_count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_read <= 0) {
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < client_count; i++) {
                if (clients[i] == client_socket) {
                    clients[i] = clients[client_count-1];
                    client_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
            close(client_socket);
            break;
        }

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            if (clients[i] != client_socket) {
                send(clients[i], buffer, bytes_read, 0);
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    int server_socket;
    struct sockaddr_in server_address;
    pthread_t thread_id;

    // Создание сокета
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Настройка адреса сервера
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    // Привязка сокета к адресу и порту
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Ошибка привязки сокета");
        exit(1);
    }

    // Ожидание подключений
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Ошибка ожидания подключений");
        exit(1);
    }

    printf("Сервер запущен. Ожидание подключений...\n");

    while (1) {
        // Принятие нового подключения
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Ошибка при принятии подключения");
            exit(1);
        }

        pthread_mutex_lock(&mutex);
        clients[client_count++] = client_socket;
        pthread_mutex_unlock(&mutex);

        // Создание потока для обработки клиента
        if (pthread_create(&thread_id, NULL, handle_client, &client_socket) != 0) {
            perror("Ошибка создания потока");
            exit(1);
        }
    }

    close(server_socket);

    return 0;
}
