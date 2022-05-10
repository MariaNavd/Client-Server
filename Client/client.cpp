#include "client.h"

Client::Client(key_t key)
{
	this->key = key;
	// Attach to shared memory
	shm_id = shmget(key, buff_size, 0666);
	if (shm_id == -1) {
		perror(ERROR_MSG);
		exit(-1);
	}
	share = shmat(shm_id, NULL, 0);
	if (share == (void*)-1)
	{
		perror(ERROR_MSG);
		exit(-1);
	}
	// Get semaphore
	sem_id = semget(key, 1, 0666);

	sops.sem_num = 0;
	sops.sem_flg = 0;
}

char* Client::getAnswer()
{
	return answer_from_server;
}

// Read answer from server
void Client::readAnswer()
{
	// Waiting for server to send the answer
	sops.sem_op = 0;
	semop(sem_id, &sops, 1);
	
	// Get answer for server
	strcpy(answer_from_server, (char*)share);
	cout << "ANSW: " << answer_from_server << endl << endl;
	// If server is stopped, quit client
	if (!strcmp(answer_from_server, STOP_MSG))
	{
		shmdt(share);
		exit(0);
	}
}

// Send message to server
void Client::sendMessage(char* msg)
{
	// If server is stopped by timeout, quit client
	if (!strcmp((char*)share, STOP_BY_TIMEOUT_MSG))
	{
		cout << (char*)share << endl;
		shmdt(share);
		exit(0);
	}
	// Waiting for server to be ready to receive the message
	sops.sem_op = 0;
	semop(sem_id, &sops, 1);
	// Sending
	cout << "Asking server " << msg << endl;
	strcpy((char*)share, msg);
	// Block client while server is handling the message
	sops.sem_op = 2;
	semop(sem_id, &sops, 1);

	readAnswer();
}

// Quit client
Client::~Client()
{
	delete[] answer_from_server;
	shmdt(share);
}
