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
#define ERROR_MESSAGE "Server Error (-1)" // Error message if file can't be opened by server 

int main( int argc, char *argv[] )
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int s;
  char buf[MAX_BYTES];
  int byteCount, bytesWritten;
  int fd;
  bool errorCheck = false;
  char test[sizeof(ERROR_MESSAGE)] = ERROR_MESSAGE;
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

  // Send the filename to the server
  send(s,cups,strlen(cups),0);

  // Receive data from the server until error or orderly shutdown occurs
  /*while((byteCount = recv(s,buf,MAX_BYTES,0)) > 0)
  {
    // Null terminate most significant character to prevent garbage
    buf[byteCount] = 0;

    // Check for file error on server side (only check once)
    if(errorCheck == false)
    {
      // Prevent multiple iterations of the error check
      errorCheck = true;
      for(unsigned int i=0; i < sizeof(ERROR_MESSAGE); i++)
      {
        // No error occured so open file and proceed past the error checking 
        if(buf[i] != test[i])
        {
          // Open the file in write mode and set fd as the file descriptor
          // Create the file if it doesnt exist. Overwrite if it does exist
          if( (fd = open(fileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) == -1)
          {
            fprintf(stderr, "Client Error: Unable to open file '%s' \n", fileName);
            close(s);
            exit(1);
          }
          break;
        }

        // Error occured on server side opening the file 
        if(i == sizeof(ERROR_MESSAGE)-1)
        {
          fprintf(stderr, "Server Error: Unable to access file '%s' \n", fileName);
          close(s);
          close(fd);
          exit(1);
        }
      }
    }

    // Write data from server to the file
    if( (bytesWritten = write(fd, &buf, byteCount)) < 0)
    {
      fprintf(stderr, "Client Error: Unable to write to file '%s' \n", fileName);
      close(s);
      close(fd);
      exit(1);
    }
  }

  // Error checking the receive function
  if(byteCount < 0)
  {
    fprintf(stderr, "Client Error: Problem occured while receiving from '%s' \n", serverName);
    close(s);
    close(fd);
    exit(1);
  }*/

  close(s);
  return 0;
}

