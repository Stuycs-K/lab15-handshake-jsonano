#include "pipe_networking.h"

// int err() {
//     printf("error %d: %s\n", errno, strerror(errno));
//     exit(1);
// }

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    char to_client_message[BUFFER_SIZE] = "Hello, client!";
    int write_to_client_status = write(to_client, to_client_message, strlen(to_client_message) + 1);
    if (write_to_client_status == -1) {
        err();
    }
    printf("Server successfully wrote to client.\n");

    char from_client_message[BUFFER_SIZE];
    int read_from_client_status = read(from_client, from_client_message, BUFFER_SIZE);
    if (read_from_client_status == -1) {
        err();
    }
    printf("Server successfully read from client: %s\n", from_client_message);

    return 0;
}
