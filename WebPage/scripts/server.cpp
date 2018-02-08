/*******************
 * 
 * 
 * Pin0 = A  Motor
 * Pin1 = A' Motor
 * Pin2 = B  Motor
 * Pin3 = B' Motor
 * Pin4 = Close Coffee Valve
 * Pin5 = Open Coffee Valve
 * Pin6 = Open Water Valve
 * Pin7 = Turn on Coffee Pot
 * ******************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <wiringPi.h>
#include <utility>

#define MAX_PENDING 1  // Maximum number of pending connections on the socket
#define MAX_BYTES 1500 // Maximum number of bytes per packet
#define ERROR_MESSAGE "Server Error (-1)" // Error message if file can't be opened
#define SERVER_PORT "5000"

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  char request[200];
  char buf[MAX_BYTES];
  int s, new_s;
  int bytesRead, bytesSent;

  // GPIO initialization
  wiringPiSetup();
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Motor step sequence
  int motorStep;
  int motorStepArray[4][4] = 
  {
    {0,1,0,1},
    {0,1,1,0},
    {1,0,1,0},
    {1,0,0,1}
  };

  // Build address data structure
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  // Obtain a socket on the specified port
  if ((s = getaddrinfo(NULL, SERVER_PORT, &hints, &result)) != 0 )
    exit(1);

  // Iterate through the address list and try to perform passive open
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
      continue;
    if (!bind(s, rp->ai_addr, rp->ai_addrlen))
      break;

    close(s);
  }

  if(rp == NULL)
    exit(1);

  // Wait for connection
  if(listen(s, MAX_PENDING) == -1)
  {
    close(s);
    exit(1);
  }

  // Iterate evertime a new connection is established
  while(1)
  {

    // Extract the first connection request for the listening socket
    if( (new_s = accept(s, rp->ai_addr, &(rp->ai_addrlen))) < 0)
    {
      close(s);
      exit(1);
    }
    
    // Receive the request that was sent from the client
    if ( (bytesRead = recv(new_s, request, sizeof(request), 0)) < 0)
    {
      close(s);
      close(new_s);
      exit(1);
    }
    // Null terminate the request to prevent garbage
    request[bytesRead] = 0;

    printf("%s \n", request);

    // Open Coffee Valve
    digitalWrite(5,HIGH);
    delay(3000);
    digitalWrite(5,LOW);
    // Close Coffee Valve
    digitalWrite(4,HIGH);
    delay(3000);
    digitalWrite(4,LOW);
    // Open Water Valve
    digitalWrite(6,HIGH);
    delay(3000);
    digitalWrite(6,LOW);
    // Turn on Coffee Pot
    digitalWrite(7,HIGH);
    delay(3000);
    digitalWrite(7,LOW);

    // Handling 180 degree rotation on the stepper motor
    for(int i=0;i < 200;i++)
    {
      motorStep = i%4;
      for( int j=0; j < 4; j++)
      {
        digitalWrite(j,motorStepArray[motorStep][j]);
      }
      delay(5);
    }

    // Turn motor off to save power
    digitalWrite(0,LOW);
    digitalWrite(1,LOW);
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);

    buf[0] = '1';

    // Send brew status to the client
    if( (bytesSent = send(new_s, &buf, bytesRead,0)) < 0)
    {
      close(s);
      close(new_s);
      exit(1);
    }

    close(new_s);
  }

  // Close all file descriptors
  freeaddrinfo(result);
  close(s);
  return 0;
}
