#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_BUFFER_SIZE 1024
#define OUTPUT_FILE "output.txt"

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    printf("[SERVER] Server is running and listening on port %d\n", PORT);

    while (1) {
        char buffer[MAX_BUFFER_SIZE];
        int recv_bytes;

        // Receive command and number of executions from client
        recv_bytes = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_bytes == -1) {
            perror("Error receiving data from client");
            exit(EXIT_FAILURE);
        }

        buffer[recv_bytes] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("[SERVER] Server is shutting down\n");
            break;
        }

        // Execute the command and store output in a file
        FILE *output_file = fopen(OUTPUT_FILE, "w");
        for (int i = 0; i < atoi(buffer); ++i) {
            FILE *command_output = popen("date", "r");
            char command_buffer[MAX_BUFFER_SIZE];
            while (fgets(command_buffer, sizeof(command_buffer), command_output) != NULL) {
                fprintf(output_file, "%s", command_buffer);
            }
            pclose(command_output);
        }
        fclose(output_file);

        // Read data from the file and send to client
        output_file = fopen(OUTPUT_FILE, "r");
        while (fgets(buffer, sizeof(buffer), output_file) != NULL) {
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_len);
            usleep(500000);  // Adding a delay to simulate packets being sent separately
        }
        fclose(output_file);

        // Send EOF and flag to indicate the end of communication
        sendto(server_socket, "EOF", 3, 0, (struct sockaddr *)&client_addr, client_len);
    }

    close(server_socket);
    return 0;
}
