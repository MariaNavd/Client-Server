/*
   Server part
   Creates shared memory and handles messages for client
   Stops by the quit signal or by timeout
*/
#pragma once
#ifndef SERVER
#define SERVER
#define START_MSG "Server started"
#define STOP_MSG  "Server stopped"
#define STOP_BY_TIMEOUT_MSG  "Server stopped by timeout"
#define ERROR_MSG "Error creating shared memory"
#endif // !SERVER

#include <iostream>
#include <ctime>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

using namespace std;

static int buff_size = 512;

static bool timeout_reached = false;

char* currentTime();

void timerHandler(int);

class Server
{
private:
	key_t key;
	int shm_id, sem_id, timeout;
	sembuf sops;
	struct sigaction sa;
	itimerval timer;
	char* message_from_client = new char[buff_size];	
public:
	void* share;
	Server(key_t, int);
	~Server() { stop(); };
	void logInJournal(string);
	void readMessage();
	void handleMessage();
	void stop();
};
