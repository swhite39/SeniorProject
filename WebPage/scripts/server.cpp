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
int getWaterDelay(unsigned int cups);

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  char request[200];
  char buf[MAX_BYTES];
  int s, new_s;
  int bytesRead, bytesSent;
  unsigned int waterDelay;

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
    
    // Determine how many cups the user wants to brew
    if(request[0] == '1')
    {
      // User wants 10 cups
      if(request[1] == '0')
      {
        printf("10 cups\n");
        waterDelay = getWaterDelay(10);
      }
      // User wants 12 cups
      else if(request[1] == '2')
      {
        printf("12 cups\n");
        waterDelay = getWaterDelay(12);
      }
    }
    else
    {
      // User wants 4 cups
      if(request[0] == '4')
      {
        printf("4 cups\n");
        waterDelay = getWaterDelay(4);
      }
      // User wants 6 cups
      else if(request[0] == '6')
      {
        printf("6 cups\n");
        waterDelay = getWaterDelay(6);
      }
      // User wants 8 cups
      else if(request[0] == '8')
      {
        printf("8 cups\n");
        waterDelay = getWaterDelay(8);
      }
    }

    // Not enough water
    if(waterDelay == 0)
    {
      // Report error to the user
      buf[0] = NO_WATER;
      buf[1] = '\0';
    }
    // All conditions met to start brew
    else
    {
      // Open Water Valve
      digitalWrite(OPEN_WATER,HIGH);
      delay(waterDelay);
      digitalWrite(OPEN_WATER,LOW);
      
      // Open Coffee Valve
      digitalWrite(OPEN_COFFEE,HIGH);
      delay(3000);
      digitalWrite(OPEN_COFFEE,LOW);
      
      // Close Coffee Valve
      digitalWrite(CLOSE_COFFEE,HIGH);
      delay(5000);
      digitalWrite(CLOSE_COFFEE,LOW);


      // Handling 180 degree rotation on the stepper motor
      for(int i=0;i < 400;i++)
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
      digitalWrite(COFFEE_POT,LOW);
      
      // Report succes to the user
      buf[0] = ALL_CLEAR;
      buf[1] = '\0';
    }


    // Send brew status to the client to start countdown timer
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

/****************************************************************
 * This function will determine the delay time based on the
 * current water level in the reservoir and the number of cups
 * the user is trying to brew. The value return is the delay time
 * the valve should be opened. Return value of zero means brew
 * is not possible
 *
 * Input:  cups     = The amount of cups the user wants to brew
 * Output: distance = The appropraite delay time for the current
 *                    machine state.
 * *************************************************************/
int getWaterDelay(unsigned int cups)
{
  // Take 3 samples to find an accurate water level distance
  int sample1 = getDistance(WATER_TRIG,WATER_ECHO);
  delay(100);
  int sample2 = getDistance(WATER_TRIG,WATER_ECHO);
  delay(100);
  int sample3 = getDistance(WATER_TRIG,WATER_ECHO);
  int average = (sample1 + sample2 + sample3)/3;
  printf("Distance: %i\n", average);

  // Look up table based on cups and water level
  switch(cups)
  {
    case 4:
      if(average < 5)
        return 3100;
      else if(average >= 5 && average < 7)
        return 3300;
      else if(average >= 7 && average < 8)
        return 3400;
      else if(average >= 8 && average < 10)
        return 3500;
      else if(average >= 10 && average < 11)
        return 3600;
      else if(average >= 11 && average < 13)
        return 3700;
      else if(average >= 13 && average < 15)
        return 3800;
      else if(average >= 15 && average < 17)
        return 4000;
      else if(average >= 17 && average < 18)
        return 4100;
      else if(average >= 18 && average < 20)
        return 4400;
      else if(average >= 20 && average <= 21)
        return 5000;
      else
        return 0;
      break;
    case 6:
      if(average < 5)
        return 4500;
      else if(average >= 5 && average < 7)
        return 4700;
      else if(average >= 7 && average < 8)
        return 4800;
      else if(average >= 8 && average < 10)
        return 5000;
      else if(average >= 10 && average < 11)
        return 5200;
      else if(average >= 11 && average < 13)
        return 5300;
      else if(average >= 13 && average < 15)
        return 5600;
      else if(average >= 15 && average < 17)
        return 6000;
      else if(average >= 17 && average < 18)
        return 6400;
      else if(average >= 18 && average <= 20)
        return 6800;
      else
        return 0;
      break;
    case 8:
      if(average < 5)
        return 5800;
      else if(average >= 5 && average < 7)
        return 6000;
      else if(average >= 7 && average < 8)
        return 6200;
      else if(average >= 8 && average < 10)
        return 6400;
      else if(average >= 10 && average < 11)
        return 6600;
      else if(average >= 11 && average < 13)
        return 6800;
      else if(average >= 13 && average < 15)
        return 7200;
      else if(average >= 15 && average < 17)
        return 7700;
      else if(average >= 17 && average <= 18)
        return 8100;
      else
        return 0;
      break;
    case 10:
      if(average < 5)
        return 7200;
      else if(average >= 5 && average < 7)
        return 7500;
      else if(average >= 7 && average < 8)
        return 7700;
      else if(average >= 8 && average < 10)
        return 8000;
      else if(average >= 10 && average < 11)
        return 8300;
      else if(average >= 11 && average < 13)
        return 8600;
      else if(average >= 13 && average < 15)
        return 8900;
      else if(average >= 15 && average <= 17)
        return 9400;
      else
        return 0;
      break;
    case 12:
      if(average < 5)
        return 8600;
      else if(average >= 5 && average < 7)
        return 8800;
      else if(average >= 7 && average < 8)
        return 9000;
      else if(average >= 8 && average < 10)
        return 9300;
      else if(average >= 10 && average < 11)
        return 9600;
      else if(average >= 11 && average < 13)
        return 9900;
      else if(average >= 13 && average <= 15)
        return 10400;
      else
        return 0;
      break;
    default:
      return 0;
  }
  return 0;
}
