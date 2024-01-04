// server_plus.c

//Note:-
//Implement the communication logic between the client, relay server, and the 
//main servers according to your protocol specifications. This includes handling operand input, sending data between servers and clients, and performing the desired arithmetic operation.
//(for addition, you can create similar files for other operations)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PLUS_PORT 6000

int main() {
    // Creating the plus socket
    int plus_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (plus_socket == -1) {
        perror("Error creating plus socket");
        exit(EXIT_FAILURE);
    }

    // Configuring plus address
    struct sockaddr_in plus_addr;
    plus_addr.sin_family = AF_INET;
    plus_addr.sin_addr.s_addr = INADDR_ANY;
    plus_addr.sin_port = htons(PLUS_PORT);

    // Binding the plus socket
    if (bind(plus_socket, (struct sockaddr *)&plus_addr, sizeof(plus_addr)) == -1) {
        perror("Error binding plus socket");
        exit(EXIT_FAILURE);
    }

    // Listening on the plus socket
    listen(plus_socket, 5);
    printf("INFO: Created plus socket\nLISTENING: Plus server is listening\n");

    // Accepting a client connection
    int client_socket;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_socket = accept(plus_socket, (struct sockaddr *)&client_addr, &client_len);

    if (client_socket == -1) {
        perror("Error accepting connection in plus server");
        exit(EXIT_FAILURE);
    }

    printf("ACCEPTED: Accepted a connection\n");

    // Handling client communication (retrieve operands, perform operation, send result)
    // ...

    close(plus_socket);
    return 0;
}
