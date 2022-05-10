#include "server.h"
#include "data.h"

// Current time - for logging in journal
char* currentTime()
{
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	return asctime(timeinfo);
}

// Timer for server
void timerHandler(int signum)
{
	timeout_reached = true;
}

// Launch server
Server::Server(key_t key, int timeout)
{
	this->key = key;
	this->timeout = timeout;
	// Create communication agent - shared memory
	shm_id = shmget(key, buff_size, IPC_CREAT | 0666);
	if (shm_id == -1) {
		logInJournal(ERROR_MSG);
		perror(ERROR_MSG);
		exit(-1);
	}
	// Attach to chared memory
	share = shmat(shm_id, NULL, 0);
	if (share == (void*)-1)
	{
		logInJournal(ERROR_MSG);
		perror(ERROR_MSG);
		exit(-1);
	}
	// Semaphore - for process synchronization
	sem_id = semget(key, 1, IPC_CREAT | 0666);

	sops.sem_num = 0;
	sops.sem_flg = 0;
	// Set semaphore's counter to 1
	semctl(sem_id, 0, SETVAL, 1);

	// Create timer on waiting for client's message
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &timerHandler;
	sigaction(SIGALRM, &sa, NULL);

	// Set timer to timeout value
	timer.it_value.tv_sec = timeout;
	timer.it_value.tv_usec = 0;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;

	// Logging in journal start time
	strcpy((char*)share, START_MSG);
	logInJournal(START_MSG);
	cout << (char*)share << endl;
	
	// Decrease semaphore's counter by 1 - client is getting access to shared memory
	sops.sem_op = -1;
	semop(sem_id, &sops, 1);

	while (true)
	{
		cout << "Server is ready to read " << endl;
		readMessage();
	}
}

// Event log (journal.log)
void Server::logInJournal(string message)
{
	string log_str(currentTime());
	log_str = log_str + "\t" + message + "\n";

	int file = open("journal.log", O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
	write(file, (void*)log_str.c_str(), log_str.size());
	close(file);
}

void Server::readMessage()
{
	// Waiting for client to send a message - server is blocked (does not have access to shared memory)
	setitimer(ITIMER_REAL, &timer, NULL);
	sops.sem_op = -1;
	semop(sem_id, &sops, 1);

	// If timeout exceeded, stop server and tell the client about it
	if (timeout_reached)
	{
		cout << "TIMEOUT" << endl;
		stop();
		exit(0);
	}
	
	// Get message_from_client
	strcpy(message_from_client, (char*)share);
	cout << "READ: " << message_from_client << endl;
	
	// If quit signal is received, stop server
	if (!strcmp(message_from_client, "QUIT_SERVER"))
	{
		sops.sem_op = -1;
		semop(sem_id, &sops, 1);
		stop();
		exit(0);
	} // Else, handle received message
	else
	{
		handleMessage();
	}
}

void Server::handleMessage()
{
	string msg(message_from_client), log_msg, pattern, item;
	auto pos = msg.find(" ");

	if (pos != string::npos)
	{
		pattern = msg.substr(0, pos);
		item = msg.substr(pos + 1);
	}
	// If search by ID or by name is requested
	if (pattern == "ID" || pattern == "NAME")
	{
		log_msg = "Search person by " + pattern;
		if (pattern == "ID")
		{
			strcpy((char*)share, parseData(stoi(item)));
		}
		else
		{
			strcpy((char*)share, parseData(item));
		}
		
	}
	else if (pattern == "ADD") // If adding to data base is requested
	{
		log_msg = "Add a new person " + item;
		string res = "A new person was added with ID " + to_string(addData(item));
		
		strcpy((char*)share, res.c_str());
	}
	else // Handling incorrect request
	{
		log_msg = "Incorrect input - " + msg;
		strcpy((char*)share, "Incorrect input");
	}
	// Log message from client in journal
	logInJournal(log_msg);
	cout << log_msg << endl;
	// Output answer from server
	cout << "ANSW: " << (char*)share << endl << endl;
	// Block server while client is reading the answer
	sops.sem_op = -1;
	semop(sem_id, &sops, 1);
}

// Stop server
void Server::stop()
{
	char* log_msg;

	if (timeout_reached)
	{
		log_msg = STOP_BY_TIMEOUT_MSG;
	}
	else
	{
		log_msg = STOP_MSG;
	}
	// Log in journal
	strcpy((char*)share, log_msg);
	cout << log_msg << endl;
	logInJournal(log_msg);

	delete[] message_from_client;
	// Remove shared memory and connected semaphore
	shmdt(share);
	semctl(sem_id, 0, IPC_RMID, 0);
	shmctl(shm_id, IPC_RMID, NULL);
}
