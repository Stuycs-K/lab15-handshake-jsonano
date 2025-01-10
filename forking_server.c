#include <signal.h>
#include "pipe_networking.h"

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("Detected SIGINT (CTRL+C). Server closed.\n");
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

    srand(time(NULL));

    while (1) {
        int to_client;
        int from_client;

        from_client = server_setup();

        pid_t child;
        child = fork();

        if (child == 0) {
            server_handshake_half(&to_client, from_client);

            while (1) {
                int to_client_message = rand() % 100;
                int write_to_client_status = write(to_client, &to_client_message, sizeof(int));
                if (write_to_client_status == -1) {
                    err();
                }
                printf("Server successfully wrote to client: %d\n", to_client_message);

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
    }

    int remove_server_status = remove(WKP);
    if (remove_server_status == -1) {
        err();
    }

    return 0;
}