/*******************
 * 
 * 
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

// Pins
#define MOTOR_AP      0
#define MOTOR_AN      1
#define MOTOR_BP      2
#define MOTOR_BN      3
#define CLOSE_COFFEE  4
#define OPEN_COFFEE   5
#define OPEN_WATER    6
#define COFFEE_POT    7
#define WATER_TRIG    8
#define WATER_ECHO    9

// Server Info
#define MAX_PENDING   1       // Maximum number of pending connections on the socket
#define MAX_BYTES     1500    // Maximum number of bytes per packet
#define SERVER_PORT   "5000"

// Brew Status
#define ALL_CLEAR     '1'     // Clear to start Brew
#define NO_WATER      '2'     // Not enough water to start brew
#define NO_GROUNDS    '3'     // Not enough coffee grounds to start brew
#define NO_FILTER     '4'     // No clean filter to start brew

int getDistance(unsigned int trigPin, unsigned int echoPin);

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
  pinMode(MOTOR_AP, OUTPUT);
  pinMode(MOTOR_AN, OUTPUT);
  pinMode(MOTOR_BP, OUTPUT);
  pinMode(MOTOR_BN, OUTPUT);
  pinMode(CLOSE_COFFEE, OUTPUT);
  pinMode(OPEN_COFFEE, OUTPUT);
  pinMode(OPEN_WATER, OUTPUT);
  pinMode(COFFEE_POT, OUTPUT);
  pinMode(WATER_TRIG, OUTPUT); 
  pinMode(WATER_ECHO, INPUT); 

  // Motor step sequence
  int motorStep;
  int motorStepArray[8][4] = 
  {
    {0,1,0,1},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {1,0,1,0},
    {1,0,0,0},
    {1,0,0,1},
    {0,0,0,1}
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

    printf("Distance: %d\n",getDistance(WATER_TRIG,WATER_ECHO)); 
    
    // Determine how many cups the user wants to brew
    if(request[0] == '1')
    {
      // User wants 10 cups
      if(request[1] == '0')
      {
        printf("10 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        delay(8000);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        digitalWrite(OPEN_COFFEE,HIGH);
        delay(3000);
        digitalWrite(OPEN_COFFEE,LOW);
      }
      // User wants 12 cups
      else if(request[1] == '2')
      {
        printf("12 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        delay(8800);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        digitalWrite(OPEN_COFFEE,HIGH);
        delay(3000);
        digitalWrite(OPEN_COFFEE,LOW);
      }
    }
    else
    {
      // User wants 2 cups
      if(request[0] == '2')
      {
        printf("2 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        //delay(3300);
        delay(8000);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        //digitalWrite(OPEN_COFFEE,HIGH);
        //delay(3000);
        //digitalWrite(OPEN_COFFEE,LOW);
      }
      // User wants 4 cups
      else if(request[0] == '4')
      {
        printf("4 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        delay(3730);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        digitalWrite(OPEN_COFFEE,HIGH);
        delay(3000);
        digitalWrite(OPEN_COFFEE,LOW);
      }
      // User wants 6 cups
      else if(request[0] == '6')
      {
        printf("6 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        delay(4900);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        digitalWrite(OPEN_COFFEE,HIGH);
        delay(3000);
        digitalWrite(OPEN_COFFEE,LOW);
      }
      // User wants 8 cups
      else if(request[0] == '8')
      {
        printf("8 cups\n");

        // Open Water Valve
        digitalWrite(OPEN_WATER,HIGH);
        delay(6300);
        digitalWrite(OPEN_WATER,LOW);
        // Open Coffee Valve
        digitalWrite(OPEN_COFFEE,HIGH);
        delay(3000);
        digitalWrite(OPEN_COFFEE,LOW);
      }
    }
    // Close Coffee Valve
    //digitalWrite(CLOSE_COFFEE,HIGH);
    //delay(5000);
    //digitalWrite(CLOSE_COFFEE,LOW);

    // Brew Status
    buf[0] = NO_FILTER;
    buf[1] = '\0';

    // Send brew status to the client to start countdown timer
    if( (bytesSent = send(new_s, &buf, bytesRead,0)) < 0)
    {
      close(s);
      close(new_s);
      exit(1);
    }
    close(new_s);

    // Handling 180 degree rotation on the stepper motor
    /*for(int i=0;i < 400;i++)
    {
      motorStep = i%8;
      for( int j=0; j < 4; j++)
      {
        digitalWrite(j,motorStepArray[motorStep][j]);
      }
      delay(5);
    }

    // Turn motor off to save power
    digitalWrite(MOTOR_AP,LOW);
    digitalWrite(MOTOR_AN,LOW);
    digitalWrite(MOTOR_BP,LOW);
    digitalWrite(MOTOR_BN,LOW);

    // Turn on Coffee Pot
    digitalWrite(COFFEE_POT,HIGH);
    delay(3000);
    digitalWrite(COFFEE_POT,LOW);*/
  }

  // Close all file descriptors
  freeaddrinfo(result);
  close(s);
  return 0;
}

/****************************************************************
 * This function will sample a ultrasonic range sensor and return 
 * the closest distance the sensor can sense. The method converts
 * the timing from the sensor into a distance in centimeters.
 *
 * Input:  trigPin  = The associated sensors trigger pin
 *         echoPin  = The associated sensors echo pin
 * Output: distance = The distance calculated by the sensor
 * *************************************************************/
int getDistance(unsigned int trigPin, unsigned int echoPin)
{
  long startTime,travelTime;
  int distance;

  // Send trigger pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  // Wait for the echo from the trigger
  while(digitalRead(echoPin) == LOW);
  
  // Calculate time by waiting for echo to end
  startTime = micros();
  while(digitalRead(echoPin) == HIGH);
  travelTime= micros() - startTime;

  // Convert time to distance in cm
  distance = travelTime/58;

  return distance;
}

