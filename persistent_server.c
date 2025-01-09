#include <signal.h>
#include "pipe_networking.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("Client ends. Server closed.\n");
        int remove_server_status = remove(WKP);
        if (remove_server_status == -1) {
            err();
        }
        exit(0);
    }
    if (signo == SIGPIPE) {
        printf("Pipe deleted. Server ends.\n");
    }
}

int main() {
    signal(SIGINT, sighandler);     // catch CTRL-C to exit while loop
    signal(SIGPIPE, sighandler);    // catch broken pipe

    while (1) {
        int to_client;
        int from_client;

        from_client = server_handshake(&to_client);

        while (1) {
            char to_client_message[BUFFER_SIZE] = "Hello, client!";
            int write_to_client_status = write(to_client, to_client_message, strlen(to_client_message) + 1);
            if (write_to_client_status == -1) {
                err();
            }
            printf("Server successfully wrote to client: %s\n", to_client_message);

            char from_client_message[BUFFER_SIZE];
            int read_from_client_status = read(from_client, from_client_message, BUFFER_SIZE);
            if (read_from_client_status == -1) {
                err();
            }
            printf("Server successfully read from client: %s\n", from_client_message);

            sleep(1);
        }

        // close old file descriptors
        int close_to_client_status = close(to_client);
        if (close_to_client_status) {
            printf("Server failed to close connection to client. ");
            err();
        }
        int close_from_client_status = close(from_client);
        if (close_from_client_status) {
            printf("Server failed to close connection from client. ");
            err();
        }
    }
    int remove_server_status = remove(WKP);
    if (remove_server_status == -1) {
        err();
    }

    return 0;
}