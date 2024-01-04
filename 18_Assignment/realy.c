// relay.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RELAY_PORT 5000

int main() {
    // Creating the relay socket
    int relay_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (relay_socket == -1) {
        perror("Error creating relay socket");
        exit(EXIT_FAILURE);
    }

    // Configuring relay address
    struct sockaddr_in relay_addr;
    relay_addr.sin_family = AF_INET;
    relay_addr.sin_addr.s_addr = INADDR_ANY;
    relay_addr.sin_port = htons(RELAY_PORT);

    // Binding the relay socket
    if (bind(relay_socket, (struct sockaddr *)&relay_addr, sizeof(relay_addr)) == -1) {
        perror("Error binding relay socket");
        exit(EXIT_FAILURE);
    }

    // Listening on the relay socket
    listen(relay_socket, 5);
    printf("INFO: Created relay socket\nLISTENING: Relay server is listening\n");

    // Accepting a client connection
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_socket = accept(relay_socket, (struct sockaddr *)&client_addr, &client_len);

    if (client_socket == -1) {
        perror("Error accepting connection in relay server");
        exit(EXIT_FAILURE);
    }

    printf("ACCEPTED: Relay server accepted connection\n");

    close(relay_socket);
    return client_socket;
}
