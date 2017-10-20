# Scott White, Ivan Colio
# CSCI 446 Program 2
# Fall 2017

all: client server 

client: client.cpp
	g++ -Wall -o client client.cpp

server: server.cpp
	g++ -Wall -o server server.cpp -lWiringPi

Clean:
	rm -f client server
