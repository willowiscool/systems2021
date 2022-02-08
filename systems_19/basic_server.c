#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  printf("\nSending test message\n");
  write(to_client, "hello", 6);
  printf("Reading test message\n");
  char hi[3];
  read(from_client, hi, 3);
  printf(hi);
}
