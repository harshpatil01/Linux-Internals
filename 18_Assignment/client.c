// client.c
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
    relay_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with relay server IP
    relay_addr.sin_port = htons(RELAY_PORT);

    // Connecting to the relay server
    if (connect(relay_socket, (struct sockaddr *)&relay_addr, sizeof(relay_addr)) == -1) {
        perror("Error connecting to relay server");
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS: Relay connection successful\n");

    // Handle relay communication and retrieve main server information
    // ...

    close(relay_socket);
    return 0;
}
