#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/uio.h>

#define MAX_BYTES 1500 // Maximum number of bytes per packet
#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT "5000"

int main( int argc, char *argv[] )
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int s;
  char buf[MAX_BYTES];
  int byteCount;
  char *cups;

  if(argc > 1)
  {
    cups = argv[1];
  }

  // Translate host name into peer's IP address
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  // Obtain a socket from the specified server
  if ((s = getaddrinfo(SERVER_NAME, SERVER_PORT, &hints, &result)) != 0 )
  {
    fprintf(stderr, "Client Error: Unable to find host '%s' \n", SERVER_NAME);
    exit(1);
  }

  // Iterate through the address list and try to connect 
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    // Socket creates an endpoint for communication and returns a descriptor
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
      continue;
    // Initiates a connection on the socket "s"
    if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1)
      break;
    close(s);
  }

  // Error checking to make sure a connection was established between client and server
  if (rp == NULL)
  {
    fprintf(stderr, "Client Error: Unable to connect to host '%s' \n",SERVER_NAME);
    close(s);
    exit(1);
  }
  freeaddrinfo(result);

  // Send the number of cups to the server
  send(s,cups,strlen(cups),0);

  // Recieve brew status from the server
  if ((byteCount = recv(s,buf,MAX_BYTES,0)) > 0)
  {
    // Null terminate most significant character
    buf[byteCount] = 0;

    // Print the return value to identify success or possible errors
    printf("%c", buf[0]);
  }
  close(s);
  return 0;
}

