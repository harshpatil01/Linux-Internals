#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Error creating client socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with server IP
    server_addr.sin_port = htons(PORT);

    // Take user input for command and number of executions
    char command[MAX_BUFFER_SIZE];
    int num_executions;

    printf("Enter any standard command (e.g., date): ");
    fgets(command, sizeof(command), stdin);
    command[strcspn(command, "\n")] = '\0';

    printf("Enter no. of times to be executed: ");
    scanf("%d", &num_executions);

    // Send command and number of executions to server
    sendto(client_socket, command, strlen(command), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    sendto(client_socket, &num_executions, sizeof(num_executions), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Receive and print data from server
    while (1) {
        char buffer[MAX_BUFFER_SIZE];
        int recv_bytes = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);

        if (recv_bytes == -1) {
            perror("Error receiving data from server");
            exit(EXIT_FAILURE);
        }

        buffer[recv_bytes] = '\0';

        // Check for EOF to terminate communication
        if (strcmp(buffer, "EOF") == 0) {
            break;
        }

        printf("%s", buffer);
    }

    printf("[CLIENT] Data received from server, sending acknowledgment\n");
    sendto(client_socket, "ACK", 3, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    close(client_socket);
    return 0;
}
