#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);    

    while (1) {
        char from_server_message[BUFFER_SIZE];
        int write_to_client_status = read(from_server, from_server_message, BUFFER_SIZE);
        if (write_to_client_status == -1) {
            err();
        }
        printf("Client successfully read from server: %s\n", from_server_message);

        char to_server_message[BUFFER_SIZE] = "Hello, server!";
        int message_to_server_status = write(to_server, to_server_message, strlen(to_server_message) + 1);
        if (message_to_server_status == -1) {
            err();
        }
        printf("Client successfully wrote to server: %s\n", to_server_message);

        sleep(1);
    }

    return 0;
}
