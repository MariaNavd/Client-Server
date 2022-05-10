/*
   Client part
   Sends messages to server
*/
#pragma once
#ifndef CLIENT
#define CLIENT
#define STOP_BY_TIMEOUT_MSG  "Server stopped by timeout"
#define STOP_MSG  "Server stopped"
#define ERROR_MSG "Error accessing to shared memory"
#endif // !CLIENT

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

using namespace std;

static int buff_size = 512;

class Client
{
private:
	key_t key;
	int shm_id, sem_id;
	struct sembuf sops;
	char* answer_from_server = new char[buff_size];
public:
	void* share;
	Client(key_t);
	~Client();
	char* getAnswer();
	void readAnswer();
	void sendMessage(char* msg = "QUIT_SERVER");
};
