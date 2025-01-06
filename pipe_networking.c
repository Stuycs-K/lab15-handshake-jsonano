#include "pipe_networking.h"

int err() {
    printf("error %d: %s\n", errno, strerror(errno));
    exit(1);
}

// UPSTREAM = to the server / from the client
// DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
    // 1. Server making the WKP pipe
    mkfifo(WKP, 0666);                          // 1. Server making the WKP.

    // 2. Server opening the WKP [BLOCKS SERVER]
    int from_client = open(WKP, O_RDONLY);      // 2. Server opening the WKP. [BLOCKS]
    if (from_client == -1) {
        err();
    }



    // 4. Server removing the WKP
    int remove_WKP = remove(WKP);
    if (remove_WKP == -1) {
        err();
    }

    // returning WKP file descriptor
    return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
    // retrieving WKP file descriptor
    int from_client = server_setup();
    char from_client_message[BUFFER_SIZE];

    // 5. Server reading PP (SYN) from WKP
    printf("Server | reading from client...\n");
    int read_from_client_status = read(from_client, from_client_message, BUFFER_SIZE);   
    if (read_from_client_status == -1) {
        err();
    }
    // int private_pipe = strtol(from_client_message, NULL, 10); // convert buffer to int
    printf("Server | received SYN: %s\n", from_client_message);

    // 6. Server opening PP [UNBLOCKS CLIENT]
    *to_client = open(from_client_message, O_WRONLY); 
    if (*to_client == -1) {
        err();
    }

    // 7. Server sending PP (SYN_ACK) to PP (to step 8)
    srand(time(NULL));
    int to_client_acknowledge = rand();
    int write_to_client_status = write(*to_client, &to_client_acknowledge, sizeof(int));                 
    if (write_to_client_status == -1) {
        err();
    }
    printf("Server | sent SYN_ACK: %d\n", to_client_acknowledge);

    // 10. Server reading ACK from WKP (from step 9)
    int from_client_acknowledge;
    int from_client_status = read(from_client, &from_client_acknowledge, sizeof(int));               
    if (from_client_status == -1) {
        err();
    }
    printf("Server | received ACK: %d. Handshake complete.\n", from_client_acknowledge);

    // return from_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
    // 3. Client making PP.
    char private_pipe[BUFFER_SIZE];
    sprintf(private_pipe, "%d", getpid());
    mkfifo(private_pipe, 0666);

    // 3. Client opening WKP. [UNBLOCKS]
    *to_server = open(WKP, O_WRONLY);
    if (*to_server == -1) {
        err();
    }

    // 3. Client writing PP (SYN) TO WKP.
    int pid = getpid();
    printf("Sending SYN: %d\n", pid);
    write(*to_server, &pid, sizeof(int));

    // 3. Client opening PP. [BLOCKS]
    int from_server = open(private_pipe, O_RDONLY);
    if (from_server == -1) {
        err();
    }

    // BLOCK

    // 8. Client reading SYN_ACK from PP (from step 7)
    int from_server_acknowledge;
    int read_from_server_status = read(from_server, &from_server_acknowledge, sizeof(int));
    if (read_from_server_status == -1) {
        err();
    }
    printf("Received SYN_ACK: %d\n", from_server_acknowledge);

    // 8. Client deleting PP
    int remove_private_pipe = remove(private_pipe);
    if (remove_private_pipe == -1) {
        err();
    }
    
    // 9. Client sending ACK to WKP (to step 10)
    int ack = from_server_acknowledge + 1;
    write(*to_server, &ack, sizeof(int));   

    return from_server;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
    int to_client = 0;
    return to_client; 
}
