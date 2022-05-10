#define SHARED_MEMORY_KEY ftok("/tmp", 822)

#include "client.h"

int main(int argc, char* argv[])
{
	bool with_sleep = false;
	int duration;
	// Handling input arguments
	if (argc > 3)
	{
		cerr << "Too many arguments" << endl;
		return 1;
	}
	else if (argc != 1)
	{		
		if (!strcmp(argv[1], "--with-sleep"))
		{
			if (argc == 2)
			{
				cerr << "Specify sleep time (sec)" << endl;
				return 1;
			}
			else
			{
				duration = atoi(argv[2]);
				if (duration <= 0)
				{
					cerr << "Time value assumed to be positive" << endl;
					return 1;
				}
				with_sleep = true;
			}
		}
		else
		{
			cerr << "Incorrect input" << endl;
			return 1;
		}
	}

	Client* c = new Client(SHARED_MEMORY_KEY);

	c->sendMessage("ID 237620"); // Search person by ID
	c->sendMessage("ADD Helen Collins"); // Add a new person
	c->sendMessage("ID 568742");
	// If sleep time is set
	if (with_sleep)
	{
		cout << "/* Sleep " << duration << " sec */" << endl;
		sleep(duration);
	}
	c->sendMessage("NAME Michael Hunt"); // Search person by name
	c->sendMessage("NAME James Lane");
	if (strstr(c->getAnswer(), "Cannot find requested data"))
	{
		c->sendMessage("ADD James Lane");
	}

	c->sendMessage(); // Send quit signal to server

	delete c;
	return 0;
}
