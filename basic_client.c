#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    from_server = client_handshake(&to_server);    

    int messages_count = 0;

    while (1) {
        int from_server_message;
        int write_to_client_status = read(from_server, &from_server_message, sizeof(int));
        if (write_to_client_status == -1) {
            err();
        }
        printf("Client successfully read from server: %d\n", from_server_message);

        char to_server_message[BUFFER_SIZE];
        sprintf(to_server_message, "Message %d", messages_count);
        int message_to_server_status = write(to_server, to_server_message, strlen(to_server_message) + 1);
        if (message_to_server_status == -1) {
            err();
        }
        printf("Client successfully wrote to server: %s\n", to_server_message);

        messages_count++;

        sleep(1);
    }

    return 0;
}
