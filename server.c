#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0){
        perror("Socket failure");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }
    printf("server listening on port: %d \n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }
    while(1){
        bzero(buffer, BUFFER_SIZE);
        read(client_fd,buffer, BUFFER_SIZE);
        printf("Client: %s", buffer);

        printf("You: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_fd, buffer, strlen(buffer), 0);
    }
    close(server_fd);
    return 0;
}