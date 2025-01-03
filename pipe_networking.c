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
    mkfifo(WKP, 0666);                          // 1. Server making the WKP.

    int from_client = open(WKP, O_RDONLY);      // 2. Server opening the WKP. [BLOCKS]
    if (from_client == -1) {
        err();
    }

    int remove_WKP = remove(WKP);
    if (remove_WKP == -1) {
        err();
    }
    
    char buffer[256];
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
    int from_client = server_setup();

    // mkfifo(getpid(), 0666);

    // // convert getpid() to string
    // *to_client = open(getpid(), O_RDONLY);      // 3. Client making WKP [UNBLOCK]
    

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
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "%s", getpid());
    mkfifo(buffer, 0666);                   // 3. Client making PP.

    int wkp = open(WKP, O_WRONLY);          // 3. Client opening WKP. [UNBLOCKS]
    if (wkp == -1) {
        err();
    }

    write(wkp, getpid(), sizeof(int));      // 3. Client writing

    int pp = open(buffer, O_RDONLY);        // 3. Client opening PP. [BLOCKS]
    if (pp == -1) {
        err();
    }

    int from_server;
    return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
    int to_client  = 0;
    return to_client;
}
